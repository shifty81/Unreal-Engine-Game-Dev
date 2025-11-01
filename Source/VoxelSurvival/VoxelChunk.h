// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VoxelData.h"
#include "VoxelChunk.generated.h"

/**
 * Represents a chunk of voxels in the world
 * Chunks are the basic unit of voxel management and rendering
 */
UCLASS()
class VOXELSURVIVAL_API AVoxelChunk : public AActor
{
	GENERATED_BODY()

public:
	AVoxelChunk();

	/** Size of chunk in voxels per dimension */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	int32 ChunkSize = 16;

	/** Size of each voxel in world units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	float VoxelSize = 100.0f;

	/** Chunk coordinates in world grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voxel")
	FIntVector ChunkCoordinate;

	/** Generate the chunk mesh from voxel data */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void GenerateMesh();

	/** Set voxel at local position */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void SetVoxel(int32 X, int32 Y, int32 Z, EVoxelType Type);

	/** Get voxel at local position */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	EVoxelType GetVoxel(int32 X, int32 Y, int32 Z) const;

	/** Initialize chunk with coordinates */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void InitializeChunk(FIntVector Coordinate);

	/** Serialize voxel data for saving/modding */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	TArray<uint8> SerializeVoxelData();

	/** Deserialize voxel data from saved data */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void DeserializeVoxelData(const TArray<uint8>& Data);

	/** Update water physics (Minecraft-style) */
	UFUNCTION(BlueprintCallable, Category = "Voxel|Water")
	void UpdateWaterPhysics();

	/** Get voxel data at position */
	FVoxelData* GetVoxelData(int32 X, int32 Y, int32 Z);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Water update timer */
	float WaterUpdateTimer = 0.0f;

	/** Water update interval in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Water")
	float WaterUpdateInterval = 0.1f;

	/** Procedural mesh component for rendering */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voxel")
	UProceduralMeshComponent* MeshComponent;

	/** Voxel data array */
	TArray<FVoxelData> VoxelData;

	/** Get voxel index from coordinates */
	int32 GetVoxelIndex(int32 X, int32 Y, int32 Z) const;

	/** Check if coordinates are valid */
	bool IsValidVoxelCoordinate(int32 X, int32 Y, int32 Z) const;

	/** Create mesh face for voxel */
	void AddVoxelFace(
		TArray<FVector>& Vertices,
		TArray<int32>& Triangles,
		TArray<FVector>& Normals,
		TArray<FVector2D>& UVs,
		TArray<FColor>& Colors,
		FVector Position,
		FVector Normal,
		EVoxelType Type
	);
};
