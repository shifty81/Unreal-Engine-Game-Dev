// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingRecipe.h"
#include "Forge.generated.h"

/**
 * Structure for tracking active crafting jobs
 */
USTRUCT(BlueprintType)
struct FCraftingJob
{
	GENERATED_BODY()

	/** Recipe being crafted */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	FCraftingRecipe Recipe;

	/** Time remaining for crafting */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	float TimeRemaining;

	/** Whether this job is complete */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	bool bComplete;

	FCraftingJob()
		: TimeRemaining(0.0f)
		, bComplete(false)
	{}
};

/**
 * Forge building for smelting ores and crafting advanced materials
 * Can create Steel from Iron + Coal/Charcoal
 * Can smelt all ore types into refined materials
 */
UCLASS()
class VOXELSURVIVAL_API AForge : public AActor
{
	GENERATED_BODY()
	
public:	
	AForge();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Visual mesh for the forge */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Forge")
	class UStaticMeshComponent* ForgeMeshComponent;

	/** Queue of active crafting jobs */
	UPROPERTY(BlueprintReadOnly, Category = "Forge", Replicated)
	TArray<FCraftingJob> CraftingQueue;

	/** Maximum number of concurrent crafting jobs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forge")
	int32 MaxCraftingSlots;

	/** Current inventory of materials in the forge */
	UPROPERTY(BlueprintReadOnly, Category = "Forge", Replicated)
	TMap<EMaterialType, int32> MaterialInventory;

	/**
	 * Add materials to the forge inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Forge")
	void AddMaterial(EMaterialType MaterialType, int32 Amount);

	/**
	 * Remove materials from the forge inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Forge")
	bool RemoveMaterial(EMaterialType MaterialType, int32 Amount);

	/**
	 * Check if forge has required materials for a recipe
	 */
	UFUNCTION(BlueprintPure, Category = "Forge")
	bool HasMaterials(const FCraftingRecipe& Recipe) const;

	/**
	 * Start crafting a recipe
	 */
	UFUNCTION(BlueprintCallable, Category = "Forge")
	bool StartCrafting(const FCraftingRecipe& Recipe);

	/**
	 * Cancel a crafting job
	 */
	UFUNCTION(BlueprintCallable, Category = "Forge")
	void CancelCrafting(int32 JobIndex);

	/**
	 * Collect completed items
	 */
	UFUNCTION(BlueprintCallable, Category = "Forge")
	TMap<EMaterialType, int32> CollectCompletedItems();

	/**
	 * Get material count in inventory
	 */
	UFUNCTION(BlueprintPure, Category = "Forge")
	int32 GetMaterialCount(EMaterialType MaterialType) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Completed items ready to collect */
	TMap<EMaterialType, int32> CompletedItems;

	/** Process crafting jobs */
	void ProcessCraftingJobs(float DeltaTime);

	/** Complete a crafting job */
	void CompleteCraftingJob(int32 JobIndex);
};
