// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoxelChunk.h"
#include "Engine/Engine.h"

AVoxelChunk::AVoxelChunk()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AVoxelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update water physics
	WaterUpdateTimer += DeltaTime;
	if (WaterUpdateTimer >= WaterUpdateInterval)
	{
		WaterUpdateTimer = 0.0f;
		UpdateWaterPhysics();
	}
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
		case EVoxelType::Water: VoxelColor = FColor(51, 102, 204, 153); break; // Semi-transparent blue
		case EVoxelType::WaterSource: VoxelColor = FColor(25, 76, 230, 179); break; // Slightly darker blue
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
				FVoxelData* CurrentVoxel = GetVoxelData(X, Y, Z);
				if (!CurrentVoxel || CurrentVoxel->Type == EVoxelType::Air)
					continue;

				FVector VoxelPosition = FVector(X, Y, Z) * VoxelSize;

				// Helper lambda to check if face should be rendered
				auto ShouldRenderFace = [&](int32 NX, int32 NY, int32 NZ) -> bool
				{
					FVoxelData* Neighbor = GetVoxelData(NX, NY, NZ);
					if (!Neighbor || Neighbor->IsTransparent())
					{
						// Don't render water faces between water blocks of same level
						if (CurrentVoxel->IsWater() && Neighbor && Neighbor->IsWater() 
							&& CurrentVoxel->WaterLevel == Neighbor->WaterLevel)
						{
							return false;
						}
						return true;
					}
					return false;
				};

				// Check each face and add if exposed to transparent block
				if (ShouldRenderFace(X, Y, Z + 1))
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::UpVector, CurrentVoxel->Type);
				
				if (ShouldRenderFace(X, Y, Z - 1))
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::DownVector, CurrentVoxel->Type);
				
				if (ShouldRenderFace(X, Y + 1, Z))
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::ForwardVector, CurrentVoxel->Type);
				
				if (ShouldRenderFace(X, Y - 1, Z))
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::BackwardVector, CurrentVoxel->Type);
				
				if (ShouldRenderFace(X + 1, Y, Z))
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::RightVector, CurrentVoxel->Type);
				
				if (ShouldRenderFace(X - 1, Y, Z))
					AddVoxelFace(Vertices, Triangles, Normals, UVs, Colors, VoxelPosition, FVector::LeftVector, CurrentVoxel->Type);
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

FVoxelData* AVoxelChunk::GetVoxelData(int32 X, int32 Y, int32 Z)
{
	if (!IsValidVoxelCoordinate(X, Y, Z))
		return nullptr;

	int32 Index = GetVoxelIndex(X, Y, Z);
	return &VoxelData[Index];
}

void AVoxelChunk::UpdateWaterPhysics()
{
	TArray<TPair<FIntVector, FVoxelData>> WaterChanges;

	// Scan for water blocks
	for (int32 X = 0; X < ChunkSize; X++)
	{
		for (int32 Y = 0; Y < ChunkSize; Y++)
		{
			for (int32 Z = 0; Z < ChunkSize; Z++)
			{
				FVoxelData* Voxel = GetVoxelData(X, Y, Z);
				if (Voxel && Voxel->IsWater())
				{
					// Water flows down first
					FVoxelData* Below = GetVoxelData(X, Y, Z - 1);
					if (Below && !Below->IsSolid() && !Below->IsWater())
					{
						// Flow down
						FVoxelData NewWater(EVoxelType::Water);
						NewWater.WaterLevel = 8;
						WaterChanges.Add(TPair<FIntVector, FVoxelData>(FIntVector(X, Y, Z - 1), NewWater));

						// Reduce source water if not a source block
						if (Voxel->Type != EVoxelType::WaterSource)
						{
							Voxel->WaterLevel -= 1;
							if (Voxel->WaterLevel <= 0)
							{
								FVoxelData Air(EVoxelType::Air);
								WaterChanges.Add(TPair<FIntVector, FVoxelData>(FIntVector(X, Y, Z), Air));
							}
						}
					}
					// Water spreads horizontally if can't flow down
					else if (Voxel->WaterLevel > 1)
					{
						int32 SpreadLevel = Voxel->WaterLevel - 1;

						// Check all 4 horizontal directions
						TArray<FIntVector> Directions = {
							FIntVector(1, 0, 0),
							FIntVector(-1, 0, 0),
							FIntVector(0, 1, 0),
							FIntVector(0, -1, 0)
						};

						for (const FIntVector& Dir : Directions)
						{
							int32 NX = X + Dir.X;
							int32 NY = Y + Dir.Y;
							int32 NZ = Z + Dir.Z;

							FVoxelData* Neighbor = GetVoxelData(NX, NY, NZ);
							if (Neighbor && !Neighbor->IsSolid())
							{
								if (!Neighbor->IsWater() || Neighbor->WaterLevel < SpreadLevel)
								{
									FVoxelData NewWater(EVoxelType::Water);
									NewWater.WaterLevel = SpreadLevel;
									WaterChanges.Add(TPair<FIntVector, FVoxelData>(FIntVector(NX, NY, NZ), NewWater));
								}
							}
						}
					}
				}
			}
		}
	}

	// Apply water changes
	bool bMeshNeedsUpdate = false;
	for (const TPair<FIntVector, FVoxelData>& Change : WaterChanges)
	{
		if (IsValidVoxelCoordinate(Change.Key.X, Change.Key.Y, Change.Key.Z))
		{
			int32 Index = GetVoxelIndex(Change.Key.X, Change.Key.Y, Change.Key.Z);
			VoxelData[Index] = Change.Value;
			bMeshNeedsUpdate = true;
		}
	}

	// Regenerate mesh if water changed
	if (bMeshNeedsUpdate)
	{
		GenerateMesh();
	}
}
