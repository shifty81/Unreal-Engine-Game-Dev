// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingRecipe.h"
#include "SalvageStation.generated.h"

/**
 * Structure defining salvage output
 */
USTRUCT(BlueprintType)
struct FSalvageOutput
{
	GENERATED_BODY()

	/** Material produced */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	EMaterialType MaterialType;

	/** Amount produced */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	int32 Amount;

	/** Chance to get this output (0.0 to 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	float DropChance;

	FSalvageOutput()
		: MaterialType(EMaterialType::Wood)
		, Amount(1)
		, DropChance(1.0f)
	{}
};

/**
 * Structure defining what materials can be salvaged from an item
 */
USTRUCT(BlueprintType)
struct FSalvageRecipe
{
	GENERATED_BODY()

	/** Input material to salvage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	EMaterialType InputMaterial;

	/** Possible outputs from salvaging */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	TArray<FSalvageOutput> PossibleOutputs;

	/** Time to salvage in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	float SalvageTime;

	FSalvageRecipe()
		: InputMaterial(EMaterialType::Wood)
		, SalvageTime(3.0f)
	{}
};

/**
 * Salvage station for breaking down items into base materials
 * Can convert wood items into charcoal
 * Can break down complex items into components
 */
UCLASS()
class VOXELSURVIVAL_API ASalvageStation : public AActor
{
	GENERATED_BODY()
	
public:	
	ASalvageStation();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Visual mesh for the salvage station */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Salvage")
	class UStaticMeshComponent* StationMeshComponent;

	/** Available salvage recipes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salvage")
	TArray<FSalvageRecipe> SalvageRecipes;

	/** Input inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Salvage", Replicated)
	TMap<EMaterialType, int32> InputInventory;

	/** Output inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Salvage", Replicated)
	TMap<EMaterialType, int32> OutputInventory;

	/** Currently processing salvage job */
	UPROPERTY(BlueprintReadOnly, Category = "Salvage", Replicated)
	bool bIsProcessing;

	/** Time remaining for current job */
	UPROPERTY(BlueprintReadOnly, Category = "Salvage", Replicated)
	float ProcessingTimeRemaining;

	/**
	 * Add material to input inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Salvage")
	void AddInputMaterial(EMaterialType MaterialType, int32 Amount);

	/**
	 * Start salvaging a material
	 */
	UFUNCTION(BlueprintCallable, Category = "Salvage")
	bool StartSalvaging(EMaterialType InputMaterial);

	/**
	 * Collect output materials
	 */
	UFUNCTION(BlueprintCallable, Category = "Salvage")
	TMap<EMaterialType, int32> CollectOutput();

	/**
	 * Get salvage recipe for input material
	 */
	UFUNCTION(BlueprintPure, Category = "Salvage")
	bool GetSalvageRecipe(EMaterialType InputMaterial, FSalvageRecipe& OutRecipe) const;

	/**
	 * Initialize default salvage recipes
	 */
	UFUNCTION(BlueprintCallable, Category = "Salvage")
	void InitializeDefaultSalvageRecipes();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Current salvage recipe being processed */
	FSalvageRecipe CurrentRecipe;

	/** Process salvaging */
	void ProcessSalvaging(float DeltaTime);

	/** Complete salvaging and generate output */
	void CompleteSalvaging();
};
