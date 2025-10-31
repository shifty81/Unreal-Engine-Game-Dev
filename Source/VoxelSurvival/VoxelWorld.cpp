// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoxelWorld.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

AVoxelWorld::AVoxelWorld()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Enable replication for multiplayer
	bReplicates = true;
}

void AVoxelWorld::BeginPlay()
{
	Super::BeginPlay();
	LastPlayerPosition = FVector::ZeroVector;
}

void AVoxelWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update chunks around player
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector PlayerPosition = PlayerPawn->GetActorLocation();
		if (FVector::Distance(PlayerPosition, LastPlayerPosition) > 1000.0f)
		{
			UpdateVisibleChunks(PlayerPosition);
			LastPlayerPosition = PlayerPosition;
		}
	}
}

FIntVector AVoxelWorld::WorldToChunkCoordinate(FVector WorldPosition) const
{
	int32 ChunkSize = 16;
	float VoxelSize = 100.0f;
	float ChunkWorldSize = ChunkSize * VoxelSize;

	return FIntVector(
		FMath::FloorToInt(WorldPosition.X / ChunkWorldSize),
		FMath::FloorToInt(WorldPosition.Y / ChunkWorldSize),
		FMath::FloorToInt(WorldPosition.Z / ChunkWorldSize)
	);
}

float AVoxelWorld::PerlinNoise(float X, float Y, float Z)
{
	// Simple noise implementation - can be replaced with more sophisticated noise
	float Noise = FMath::Sin(X * NoiseFrequency + WorldSeed) * 
	              FMath::Cos(Y * NoiseFrequency + WorldSeed) * 
	              FMath::Sin(Z * NoiseFrequency + WorldSeed);
	
	return FMath::Clamp((Noise + 1.0f) * 0.5f, 0.0f, 1.0f);
}

void AVoxelWorld::GenerateChunkTerrain(AVoxelChunk* Chunk)
{
	if (!Chunk)
		return;

	int32 ChunkSize = Chunk->ChunkSize;
	FIntVector ChunkCoord = Chunk->ChunkCoordinate;
	
	for (int32 Z = 0; Z < ChunkSize; Z++)
	{
		for (int32 Y = 0; Y < ChunkSize; Y++)
		{
			for (int32 X = 0; X < ChunkSize; X++)
			{
				// World position
				FVector WorldPos = FVector(
					(ChunkCoord.X * ChunkSize + X) * Chunk->VoxelSize,
					(ChunkCoord.Y * ChunkSize + Y) * Chunk->VoxelSize,
					(ChunkCoord.Z * ChunkSize + Z) * Chunk->VoxelSize
				);

				// Generate height using noise
				float Height = PerlinNoise(WorldPos.X, WorldPos.Y, 0) * HeightScale;
				float WorldHeight = WorldPos.Z / Chunk->VoxelSize;

				EVoxelType VoxelType = EVoxelType::Air;

				if (WorldHeight < Height)
				{
					if (WorldHeight < Height - 3)
					{
						VoxelType = EVoxelType::Stone;
					}
					else if (WorldHeight < Height - 1)
					{
						VoxelType = EVoxelType::Dirt;
					}
					else
					{
						VoxelType = EVoxelType::Grass;
					}
				}

				Chunk->SetVoxel(X, Y, Z, VoxelType);
			}
		}
	}

	Chunk->GenerateMesh();
}

AVoxelChunk* AVoxelWorld::GetOrCreateChunk(FIntVector ChunkCoordinate)
{
	// Check if chunk already exists
	if (LoadedChunks.Contains(ChunkCoordinate))
	{
		return LoadedChunks[ChunkCoordinate];
	}

	// Create new chunk
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	AVoxelChunk* NewChunk = GetWorld()->SpawnActor<AVoxelChunk>(AVoxelChunk::StaticClass(), SpawnParams);
	if (NewChunk)
	{
		NewChunk->InitializeChunk(ChunkCoordinate);
		
		// Set chunk world position
		float ChunkWorldSize = NewChunk->ChunkSize * NewChunk->VoxelSize;
		FVector ChunkWorldPosition = FVector(
			ChunkCoordinate.X * ChunkWorldSize,
			ChunkCoordinate.Y * ChunkWorldSize,
			ChunkCoordinate.Z * ChunkWorldSize
		);
		NewChunk->SetActorLocation(ChunkWorldPosition);

		// Generate terrain
		GenerateChunkTerrain(NewChunk);

		LoadedChunks.Add(ChunkCoordinate, NewChunk);
	}

	return NewChunk;
}

void AVoxelWorld::UpdateVisibleChunks(FVector PlayerPosition)
{
	FIntVector PlayerChunk = WorldToChunkCoordinate(PlayerPosition);

	// Load chunks in render distance
	for (int32 Z = -1; Z <= 1; Z++)
	{
		for (int32 Y = -RenderDistance; Y <= RenderDistance; Y++)
		{
			for (int32 X = -RenderDistance; X <= RenderDistance; X++)
			{
				FIntVector ChunkCoord = PlayerChunk + FIntVector(X, Y, Z);
				GetOrCreateChunk(ChunkCoord);
			}
		}
	}

	// Unload far chunks (simplified - can be optimized)
	TArray<FIntVector> ChunksToRemove;
	for (auto& Pair : LoadedChunks)
	{
		FIntVector ChunkCoord = Pair.Key;
		float Distance = FVector::Distance(
			FVector(ChunkCoord),
			FVector(PlayerChunk)
		);

		if (Distance > RenderDistance + 2)
		{
			ChunksToRemove.Add(ChunkCoord);
		}
	}

	for (FIntVector ChunkCoord : ChunksToRemove)
	{
		AVoxelChunk* Chunk = LoadedChunks[ChunkCoord];
		if (Chunk)
		{
			Chunk->Destroy();
		}
		LoadedChunks.Remove(ChunkCoord);
	}
}

EVoxelType AVoxelWorld::GetVoxelAtWorldPosition(FVector WorldPosition)
{
	FIntVector ChunkCoord = WorldToChunkCoordinate(WorldPosition);
	AVoxelChunk* Chunk = GetOrCreateChunk(ChunkCoord);
	
	if (!Chunk)
		return EVoxelType::Air;

	// Convert to local voxel coordinates
	float ChunkWorldSize = Chunk->ChunkSize * Chunk->VoxelSize;
	FVector LocalPos = WorldPosition - Chunk->GetActorLocation();
	
	int32 X = FMath::FloorToInt(LocalPos.X / Chunk->VoxelSize);
	int32 Y = FMath::FloorToInt(LocalPos.Y / Chunk->VoxelSize);
	int32 Z = FMath::FloorToInt(LocalPos.Z / Chunk->VoxelSize);

	return Chunk->GetVoxel(X, Y, Z);
}

void AVoxelWorld::SetVoxelAtWorldPosition(FVector WorldPosition, EVoxelType Type)
{
	FIntVector ChunkCoord = WorldToChunkCoordinate(WorldPosition);
	AVoxelChunk* Chunk = GetOrCreateChunk(ChunkCoord);
	
	if (!Chunk)
		return;

	// Convert to local voxel coordinates
	FVector LocalPos = WorldPosition - Chunk->GetActorLocation();
	
	int32 X = FMath::FloorToInt(LocalPos.X / Chunk->VoxelSize);
	int32 Y = FMath::FloorToInt(LocalPos.Y / Chunk->VoxelSize);
	int32 Z = FMath::FloorToInt(LocalPos.Z / Chunk->VoxelSize);

	Chunk->SetVoxel(X, Y, Z, Type);
	Chunk->GenerateMesh();
}

void AVoxelWorld::SaveWorldData(const FString& SaveName)
{
	// Implement save functionality for persistence
	// This allows modders to save/load custom worlds
	UE_LOG(LogTemp, Log, TEXT("Saving world data: %s"), *SaveName);
}

void AVoxelWorld::LoadWorldData(const FString& SaveName)
{
	// Implement load functionality
	UE_LOG(LogTemp, Log, TEXT("Loading world data: %s"), *SaveName);
}
