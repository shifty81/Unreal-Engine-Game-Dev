// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoxelChunk.h"
#include "Engine/Engine.h"

AVoxelChunk::AVoxelChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create procedural mesh component
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("VoxelMesh"));
	RootComponent = MeshComponent;

	// Enable collision
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	
	// Enable replication for multiplayer
	bReplicates = true;
	SetReplicateMovement(false);
}

void AVoxelChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AVoxelChunk::InitializeChunk(FIntVector Coordinate)
{
	ChunkCoordinate = Coordinate;
	
	// Initialize voxel data array
	int32 TotalVoxels = ChunkSize * ChunkSize * ChunkSize;
	VoxelData.SetNum(TotalVoxels);
	
	// Fill with air by default
	for (int32 i = 0; i < TotalVoxels; i++)
	{
		VoxelData[i] = FVoxelData(EVoxelType::Air);
	}
}

int32 AVoxelChunk::GetVoxelIndex(int32 X, int32 Y, int32 Z) const
{
	return X + Y * ChunkSize + Z * ChunkSize * ChunkSize;
}

bool AVoxelChunk::IsValidVoxelCoordinate(int32 X, int32 Y, int32 Z) const
{
	return X >= 0 && X < ChunkSize &&
	       Y >= 0 && Y < ChunkSize &&
	       Z >= 0 && Z < ChunkSize;
}

void AVoxelChunk::SetVoxel(int32 X, int32 Y, int32 Z, EVoxelType Type)
{
	if (!IsValidVoxelCoordinate(X, Y, Z))
		return;

	int32 Index = GetVoxelIndex(X, Y, Z);
	VoxelData[Index].Type = Type;
}

EVoxelType AVoxelChunk::GetVoxel(int32 X, int32 Y, int32 Z) const
{
	if (!IsValidVoxelCoordinate(X, Y, Z))
		return EVoxelType::Air;

	int32 Index = GetVoxelIndex(X, Y, Z);
	return VoxelData[Index].Type;
}

void AVoxelChunk::AddVoxelFace(
	TArray<FVector>& Vertices,
	TArray<int32>& Triangles,
	TArray<FVector>& Normals,
	TArray<FVector2D>& UVs,
	TArray<FColor>& Colors,
	FVector Position,
	FVector Normal,
	EVoxelType Type)
{
	int32 VertexIndex = Vertices.Num();
	
	// Color based on voxel type
	FColor VoxelColor = FColor::White;
	switch (Type)
	{
		case EVoxelType::Stone: VoxelColor = FColor(128, 128, 128); break;
		case EVoxelType::Dirt: VoxelColor = FColor(139, 69, 19); break;
		case EVoxelType::Grass: VoxelColor = FColor(34, 139, 34); break;
		case EVoxelType::Wood: VoxelColor = FColor(160, 82, 45); break;
		case EVoxelType::Iron: VoxelColor = FColor(192, 192, 192); break;
		case EVoxelType::Gold: VoxelColor = FColor(255, 215, 0); break;
		default: VoxelColor = FColor::White; break;
	}

	// Define face vertices based on normal direction
	FVector V1, V2, V3, V4;
	float HalfSize = VoxelSize * 0.5f;

	if (Normal == FVector::UpVector)
	{
		V1 = Position + FVector(-HalfSize, -HalfSize, HalfSize);
		V2 = Position + FVector(HalfSize, -HalfSize, HalfSize);
		V3 = Position + FVector(HalfSize, HalfSize, HalfSize);
		V4 = Position + FVector(-HalfSize, HalfSize, HalfSize);
	}
	else if (Normal == FVector::DownVector)
	{
		V1 = Position + FVector(-HalfSize, HalfSize, -HalfSize);
		V2 = Position + FVector(HalfSize, HalfSize, -HalfSize);
		V3 = Position + FVector(HalfSize, -HalfSize, -HalfSize);
		V4 = Position + FVector(-HalfSize, -HalfSize, -HalfSize);
	}
	else if (Normal == FVector::ForwardVector)
	{
		V1 = Position + FVector(-HalfSize, HalfSize, -HalfSize);
		V2 = Position + FVector(-HalfSize, HalfSize, HalfSize);
		V3 = Position + FVector(HalfSize, HalfSize, HalfSize);
		V4 = Position + FVector(HalfSize, HalfSize, -HalfSize);
	}
	else if (Normal == FVector::BackwardVector)
	{
		V1 = Position + FVector(HalfSize, -HalfSize, -HalfSize);
		V2 = Position + FVector(HalfSize, -HalfSize, HalfSize);
		V3 = Position + FVector(-HalfSize, -HalfSize, HalfSize);
		V4 = Position + FVector(-HalfSize, -HalfSize, -HalfSize);
	}
	else if (Normal == FVector::RightVector)
	{
		V1 = Position + FVector(HalfSize, -HalfSize, -HalfSize);
		V2 = Position + FVector(HalfSize, HalfSize, -HalfSize);
		V3 = Position + FVector(HalfSize, HalfSize, HalfSize);
		V4 = Position + FVector(HalfSize, -HalfSize, HalfSize);
	}
	else // Left
	{
		V1 = Position + FVector(-HalfSize, HalfSize, -HalfSize);
		V2 = Position + FVector(-HalfSize, -HalfSize, -HalfSize);
		V3 = Position + FVector(-HalfSize, -HalfSize, HalfSize);
		V4 = Position + FVector(-HalfSize, HalfSize, HalfSize);
	}

	// Add vertices
	Vertices.Add(V1);
	Vertices.Add(V2);
	Vertices.Add(V3);
	Vertices.Add(V4);

	// Add triangles
	Triangles.Add(VertexIndex);
	Triangles.Add(VertexIndex + 1);
	Triangles.Add(VertexIndex + 2);
	Triangles.Add(VertexIndex);
	Triangles.Add(VertexIndex + 2);
	Triangles.Add(VertexIndex + 3);

	// Add normals
	for (int32 i = 0; i < 4; i++)
	{
		Normals.Add(Normal);
		Colors.Add(VoxelColor);
	}

	// Add UVs
	UVs.Add(FVector2D(0, 0));
	UVs.Add(FVector2D(1, 0));
	UVs.Add(FVector2D(1, 1));
	UVs.Add(FVector2D(0, 1));
}

void AVoxelChunk::GenerateMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> Colors;

	// Generate mesh for each solid voxel
	for (int32 Z = 0; Z < ChunkSize; Z++)
	{
		for (int32 Y = 0; Y < ChunkSize; Y++)
		{
			for (int32 X = 0; X < ChunkSize; X++)
			{
				EVoxelType VoxelType = GetVoxel(X, Y, Z);
				
				if (VoxelType == EVoxelType::Air)
					continue;

				FVector VoxelPosition = FVector(X, Y, Z) * VoxelSize;

				// Check each face and add if exposed to air
				if (GetVoxel(X, Y, Z + 1) == EVoxelType::Air)
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::UpVector, VoxelType);
				
				if (GetVoxel(X, Y, Z - 1) == EVoxelType::Air)
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::DownVector, VoxelType);
				
				if (GetVoxel(X, Y + 1, Z) == EVoxelType::Air)
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::ForwardVector, VoxelType);
				
				if (GetVoxel(X, Y - 1, Z) == EVoxelType::Air)
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::BackwardVector, VoxelType);
				
				if (GetVoxel(X + 1, Y, Z) == EVoxelType::Air)
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::RightVector, VoxelType);
				
				if (GetVoxel(X - 1, Y, Z) == EVoxelType::Air)
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::LeftVector, VoxelType);
			}
		}
	}

	// Create procedural mesh
	MeshComponent->ClearAllMeshSections();
	if (Vertices.Num() > 0)
	{
		TArray<FProcMeshTangent> Tangents;
		MeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	}
}

TArray<uint8> AVoxelChunk::SerializeVoxelData()
{
	TArray<uint8> Data;
	Data.SetNum(VoxelData.Num() * 3);
	
	for (int32 i = 0; i < VoxelData.Num(); i++)
	{
		Data[i * 3] = (uint8)VoxelData[i].Type;
		Data[i * 3 + 1] = VoxelData[i].Health;
		Data[i * 3 + 2] = VoxelData[i].CustomData;
	}
	
	return Data;
}

void AVoxelChunk::DeserializeVoxelData(const TArray<uint8>& Data)
{
	int32 VoxelCount = Data.Num() / 3;
	VoxelData.SetNum(VoxelCount);
	
	for (int32 i = 0; i < VoxelCount; i++)
	{
		VoxelData[i].Type = (EVoxelType)Data[i * 3];
		VoxelData[i].Health = Data[i * 3 + 1];
		VoxelData[i].CustomData = Data[i * 3 + 2];
	}
	
	GenerateMesh();
}
