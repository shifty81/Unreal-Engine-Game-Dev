// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingRecipe.h"
#include "InventoryItem.h"
#include "CraftingComponent.generated.h"

/**
 * Component for handling player crafting
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOXELSURVIVAL_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCraftingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	/**
	 * Get all available crafting recipes
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	TArray<FCraftingRecipe> GetAvailableRecipes();

	/**
	 * Get tool crafting recipes
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	TArray<FCraftingRecipe> GetToolRecipes();

	/**
	 * Check if player can craft recipe
	 */
	UFUNCTION(BlueprintPure, Category = "Crafting")
	bool CanCraftRecipe(const FCraftingRecipe& Recipe, class UInventoryComponent* Inventory) const;

	/**
	 * Craft item from recipe
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool CraftItem(const FCraftingRecipe& Recipe, class UInventoryComponent* Inventory);

	/**
	 * Get tool recipes list
	 */
	static void InitializeToolRecipes();

	/**
	 * Get tool recipe for specific tool
	 */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	static bool GetToolCraftingRecipe(EToolType ToolType, EMaterialType MaterialType, FCraftingRecipe& OutRecipe);

private:
	static TArray<FCraftingRecipe> ToolRecipes;
	static bool bToolRecipesInitialized;
};
