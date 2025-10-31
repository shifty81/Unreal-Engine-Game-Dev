// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelChunk.h"
#include "VoxelWorld.generated.h"

/**
 * Manages the voxel world, including chunk generation and world generation
 * Supports modding through data-driven world generation parameters
 */
UCLASS()
class VOXELSURVIVAL_API AVoxelWorld : public AActor
{
	GENERATED_BODY()

public:
	AVoxelWorld();

	/** Render distance in chunks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Generation")
	int32 RenderDistance = 8;

	/** World generation seed (modifiable for different worlds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Generation")
	int32 WorldSeed = 12345;

	/** Height map scale */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Generation")
	float HeightScale = 10.0f;

	/** Noise frequency for terrain generation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Generation")
	float NoiseFrequency = 0.01f;

	/** Generate or load chunk at world position */
	UFUNCTION(BlueprintCallable, Category = "Voxel World")
	AVoxelChunk* GetOrCreateChunk(FIntVector ChunkCoordinate);

	/** Update visible chunks around player */
	UFUNCTION(BlueprintCallable, Category = "Voxel World")
	void UpdateVisibleChunks(FVector PlayerPosition);

	/** Get voxel at world position */
	UFUNCTION(BlueprintCallable, Category = "Voxel World")
	EVoxelType GetVoxelAtWorldPosition(FVector WorldPosition);

	/** Set voxel at world position */
	UFUNCTION(BlueprintCallable, Category = "Voxel World")
	void SetVoxelAtWorldPosition(FVector WorldPosition, EVoxelType Type);

	/** Save world data for persistence/modding */
	UFUNCTION(BlueprintCallable, Category = "Voxel World")
	void SaveWorldData(const FString& SaveName);

	/** Load world data */
	UFUNCTION(BlueprintCallable, Category = "Voxel World")
	void LoadWorldData(const FString& SaveName);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Map of loaded chunks */
	UPROPERTY()
	TMap<FIntVector, AVoxelChunk*> LoadedChunks;

	/** Generate terrain for a chunk */
	void GenerateChunkTerrain(AVoxelChunk* Chunk);

	/** Perlin noise function for terrain generation */
	float PerlinNoise(float X, float Y, float Z);

	/** Get chunk coordinate from world position */
	FIntVector WorldToChunkCoordinate(FVector WorldPosition) const;

	/** Last player position for chunk loading */
	FVector LastPlayerPosition;
};
