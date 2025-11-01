// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceNode.h"
#include "ResourceManager.generated.h"

/**
 * Structure defining resource spawn parameters
 */
USTRUCT(BlueprintType)
struct FResourceSpawnInfo
{
	GENERATED_BODY()

	/** The resource class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Spawn")
	TSubclassOf<AResourceNode> ResourceClass;

	/** Minimum distance between resource spawns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Spawn")
	float MinSpawnDistance;

	/** Maximum number of this resource type in the world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Spawn")
	int32 MaxCount;

	/** Spawn density (resources per square unit area) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Spawn")
	float SpawnDensity;

	/** Minimum height for spawning (world Z coordinate) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Spawn")
	float MinSpawnHeight;

	/** Maximum height for spawning (world Z coordinate) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Spawn")
	float MaxSpawnHeight;

	FResourceSpawnInfo()
		: ResourceClass(nullptr)
		, MinSpawnDistance(500.0f)
		, MaxCount(100)
		, SpawnDensity(0.001f)
		, MinSpawnHeight(-1000.0f)
		, MaxSpawnHeight(1000.0f)
	{}
};

/**
 * Manages resource spawning and distribution across the game world
 * Integrates with VoxelWorld for terrain-aware placement
 */
UCLASS()
class VOXELSURVIVAL_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Array of resource types to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Manager")
	TArray<FResourceSpawnInfo> ResourceSpawnConfigs;

	/** Radius around player to spawn resources */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Manager")
	float SpawnRadius;

	/** How often to check for new resource spawns (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Manager")
	float SpawnCheckInterval;

	/** Currently spawned resources */
	UPROPERTY(BlueprintReadOnly, Category = "Resource Manager")
	TArray<AResourceNode*> SpawnedResources;

	/**
	 * Spawn resources in a given area
	 * @param Center - Center point for spawning
	 * @param Radius - Radius around center to spawn
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource Manager")
	void SpawnResourcesInArea(FVector Center, float Radius);

	/**
	 * Spawn a specific resource at a location
	 * @param ResourceClass - Class of resource to spawn
	 * @param Location - Where to spawn the resource
	 * @return The spawned resource, or nullptr if failed
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource Manager")
	AResourceNode* SpawnResourceAtLocation(TSubclassOf<AResourceNode> ResourceClass, FVector Location);

	/**
	 * Remove all spawned resources
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource Manager")
	void ClearAllResources();

	/**
	 * Get resource count by type
	 */
	UFUNCTION(BlueprintPure, Category = "Resource Manager")
	int32 GetResourceCount(EResourceType ResourceType) const;

	/**
	 * Find nearest resource of a specific type
	 */
	UFUNCTION(BlueprintPure, Category = "Resource Manager")
	AResourceNode* FindNearestResource(FVector Location, EResourceType ResourceType, float MaxDistance) const;

private:
	/** Timer handle for spawn checks */
	FTimerHandle SpawnCheckTimerHandle;

	/** Perform periodic spawn checks */
	void PerformSpawnCheck();

	/** Check if location is valid for spawning */
	bool IsValidSpawnLocation(FVector Location, float MinDistance) const;

	/** Get surface location for spawning (trace down to ground) */
	bool GetSurfaceLocation(FVector Location, FVector& OutSurfaceLocation) const;
};
