// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResourceNode.h"
#include "CraftingRecipe.generated.h"

/**
 * Structure defining an ingredient for crafting
 */
USTRUCT(BlueprintType)
struct FRecipeIngredient
{
	GENERATED_BODY()

	/** Material type required */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	EMaterialType MaterialType;

	/** Amount required */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int32 Amount;

	FRecipeIngredient()
		: MaterialType(EMaterialType::Wood)
		, Amount(1)
	{}
};

/**
 * Structure defining a crafting recipe
 */
USTRUCT(BlueprintType)
struct FCraftingRecipe
{
	GENERATED_BODY()

	/** Name of the recipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FString RecipeName;

	/** Materials required to craft */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	TArray<FRecipeIngredient> Ingredients;

	/** Material produced */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	EMaterialType OutputMaterial;

	/** Amount produced */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int32 OutputAmount;

	/** Crafting time in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	float CraftingTime;

	/** Whether this requires a forge */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	bool bRequiresForge;

	FCraftingRecipe()
		: RecipeName(TEXT("Unknown Recipe"))
		, OutputMaterial(EMaterialType::Wood)
		, OutputAmount(1)
		, CraftingTime(5.0f)
		, bRequiresForge(false)
	{}
};

/**
 * Helper class containing crafting recipe definitions
 */
UCLASS()
class VOXELSURVIVAL_API UCraftingRecipeLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Get all available crafting recipes
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	static TArray<FCraftingRecipe> GetAllRecipes();

	/**
	 * Get recipe for a specific output material
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	static bool GetRecipeForMaterial(EMaterialType MaterialType, FCraftingRecipe& OutRecipe);

	/**
	 * Initialize default recipes
	 */
	static void InitializeDefaultRecipes();

private:
	static TArray<FCraftingRecipe> DefaultRecipes;
	static bool bRecipesInitialized;
};
