// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResourceNode.h"
#include "TreeResource.generated.h"

/**
 * Tree resource node that provides wood when harvested
 * Features procedural generation and visual variation
 */
UCLASS()
class VOXELSURVIVAL_API ATreeResource : public AResourceNode
{
	GENERATED_BODY()
	
public:
	ATreeResource();

protected:
	virtual void BeginPlay() override;

public:
	/** Height of the tree in units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float TreeHeight;

	/** Radius of the tree trunk */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree", meta = (ClampMin = "10.0", ClampMax = "100.0"))
	float TrunkRadius;

	/** Radius of the tree canopy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree", meta = (ClampMin = "100.0", ClampMax = "500.0"))
	float CanopyRadius;

	/** Tree type/species for visual variation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	int32 TreeVariant;

	/** Foliage mesh component for leaves */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tree")
	class UStaticMeshComponent* FoliageMeshComponent;

	/**
	 * Generate procedural tree geometry
	 */
	UFUNCTION(BlueprintCallable, Category = "Tree")
	void GenerateTreeGeometry();

	/**
	 * Randomize tree properties for natural variation
	 */
	UFUNCTION(BlueprintCallable, Category = "Tree")
	void RandomizeTreeProperties();

	virtual void OnDepleted_Implementation() override;
	virtual void OnRespawn_Implementation() override;
};
