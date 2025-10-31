// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingComponent.h"
#include "InventoryComponent.h"

TArray<FCraftingRecipe> UCraftingComponent::ToolRecipes;
bool UCraftingComponent::bToolRecipesInitialized = false;

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bToolRecipesInitialized)
	{
		InitializeToolRecipes();
	}
}

void UCraftingComponent::InitializeToolRecipes()
{
	if (bToolRecipesInitialized)
	{
		return;
	}

	ToolRecipes.Empty();

	// Wood Tools
	// Wood Pickaxe
	FCraftingRecipe WoodPickaxe;
	WoodPickaxe.RecipeName = TEXT("Wood Pickaxe");
	WoodPickaxe.OutputMaterial = EMaterialType::Wood; // Placeholder
	WoodPickaxe.OutputAmount = 1;
	WoodPickaxe.CraftingTime = 5.0f;
	WoodPickaxe.bRequiresForge = false;
	WoodPickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 10});
	WoodPickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(WoodPickaxe);

	// Wood Axe
	FCraftingRecipe WoodAxe;
	WoodAxe.RecipeName = TEXT("Wood Axe");
	WoodAxe.OutputMaterial = EMaterialType::Wood;
	WoodAxe.OutputAmount = 1;
	WoodAxe.CraftingTime = 5.0f;
	WoodAxe.bRequiresForge = false;
	WoodAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 10});
	WoodAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(WoodAxe);

	// Stone Tools
	// Stone Pickaxe
	FCraftingRecipe StonePickaxe;
	StonePickaxe.RecipeName = TEXT("Stone Pickaxe");
	StonePickaxe.OutputMaterial = EMaterialType::Stone;
	StonePickaxe.OutputAmount = 1;
	StonePickaxe.CraftingTime = 8.0f;
	StonePickaxe.bRequiresForge = false;
	StonePickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Stone, 8});
	StonePickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(StonePickaxe);

	// Stone Axe
	FCraftingRecipe StoneAxe;
	StoneAxe.RecipeName = TEXT("Stone Axe");
	StoneAxe.OutputMaterial = EMaterialType::Stone;
	StoneAxe.OutputAmount = 1;
	StoneAxe.CraftingTime = 8.0f;
	StoneAxe.bRequiresForge = false;
	StoneAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Stone, 8});
	StoneAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(StoneAxe);

	// Stone Shovel
	FCraftingRecipe StoneShovel;
	StoneShovel.RecipeName = TEXT("Stone Shovel");
	StoneShovel.OutputMaterial = EMaterialType::Stone;
	StoneShovel.OutputAmount = 1;
	StoneShovel.CraftingTime = 8.0f;
	StoneShovel.bRequiresForge = false;
	StoneShovel.Ingredients.Add(FRecipeIngredient{EMaterialType::Stone, 6});
	StoneShovel.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(StoneShovel);

	// Iron Tools
	// Iron Pickaxe
	FCraftingRecipe IronPickaxe;
	IronPickaxe.RecipeName = TEXT("Iron Pickaxe");
	IronPickaxe.OutputMaterial = EMaterialType::Iron;
	IronPickaxe.OutputAmount = 1;
	IronPickaxe.CraftingTime = 12.0f;
	IronPickaxe.bRequiresForge = true;
	IronPickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Iron, 5});
	IronPickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(IronPickaxe);

	// Iron Axe
	FCraftingRecipe IronAxe;
	IronAxe.RecipeName = TEXT("Iron Axe");
	IronAxe.OutputMaterial = EMaterialType::Iron;
	IronAxe.OutputAmount = 1;
	IronAxe.CraftingTime = 12.0f;
	IronAxe.bRequiresForge = true;
	IronAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Iron, 5});
	IronAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(IronAxe);

	// Iron Shovel
	FCraftingRecipe IronShovel;
	IronShovel.RecipeName = TEXT("Iron Shovel");
	IronShovel.OutputMaterial = EMaterialType::Iron;
	IronShovel.OutputAmount = 1;
	IronShovel.CraftingTime = 12.0f;
	IronShovel.bRequiresForge = true;
	IronShovel.Ingredients.Add(FRecipeIngredient{EMaterialType::Iron, 4});
	IronShovel.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(IronShovel);

	// Iron Hammer
	FCraftingRecipe IronHammer;
	IronHammer.RecipeName = TEXT("Iron Hammer");
	IronHammer.OutputMaterial = EMaterialType::Iron;
	IronHammer.OutputAmount = 1;
	IronHammer.CraftingTime = 12.0f;
	IronHammer.bRequiresForge = true;
	IronHammer.Ingredients.Add(FRecipeIngredient{EMaterialType::Iron, 6});
	IronHammer.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(IronHammer);

	// Steel Tools
	// Steel Pickaxe
	FCraftingRecipe SteelPickaxe;
	SteelPickaxe.RecipeName = TEXT("Steel Pickaxe");
	SteelPickaxe.OutputMaterial = EMaterialType::Steel;
	SteelPickaxe.OutputAmount = 1;
	SteelPickaxe.CraftingTime = 15.0f;
	SteelPickaxe.bRequiresForge = true;
	SteelPickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Steel, 5});
	SteelPickaxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(SteelPickaxe);

	// Steel Axe
	FCraftingRecipe SteelAxe;
	SteelAxe.RecipeName = TEXT("Steel Axe");
	SteelAxe.OutputMaterial = EMaterialType::Steel;
	SteelAxe.OutputAmount = 1;
	SteelAxe.CraftingTime = 15.0f;
	SteelAxe.bRequiresForge = true;
	SteelAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Steel, 5});
	SteelAxe.Ingredients.Add(FRecipeIngredient{EMaterialType::Wood, 5});
	ToolRecipes.Add(SteelAxe);

	bToolRecipesInitialized = true;
}

TArray<FCraftingRecipe> UCraftingComponent::GetAvailableRecipes()
{
	if (!bToolRecipesInitialized)
	{
		InitializeToolRecipes();
	}

	TArray<FCraftingRecipe> AllRecipes;
	AllRecipes.Append(ToolRecipes);
	AllRecipes.Append(UCraftingRecipeLibrary::GetAllRecipes());
	
	return AllRecipes;
}

TArray<FCraftingRecipe> UCraftingComponent::GetToolRecipes()
{
	if (!bToolRecipesInitialized)
	{
		InitializeToolRecipes();
	}
	
	return ToolRecipes;
}

bool UCraftingComponent::CanCraftRecipe(const FCraftingRecipe& Recipe, UInventoryComponent* Inventory) const
{
	if (!Inventory)
	{
		return false;
	}

	// Check all ingredients
	for (const FRecipeIngredient& Ingredient : Recipe.Ingredients)
	{
		if (!Inventory->HasMaterial(Ingredient.MaterialType, Ingredient.Amount))
		{
			return false;
		}
	}

	return true;
}

bool UCraftingComponent::CraftItem(const FCraftingRecipe& Recipe, UInventoryComponent* Inventory)
{
	if (!Inventory || !CanCraftRecipe(Recipe, Inventory))
	{
		return false;
	}

	// Consume ingredients
	for (const FRecipeIngredient& Ingredient : Recipe.Ingredients)
	{
		Inventory->RemoveMaterial(Ingredient.MaterialType, Ingredient.Amount);
	}

	// Add output item
	// For tools, create the appropriate tool item
	if (Recipe.RecipeName.Contains("Pickaxe"))
	{
		FInventoryItem Tool = UInventoryItemLibrary::CreateToolItem(EToolType::Pickaxe, Recipe.OutputMaterial);
		Inventory->AddItem(Tool);
	}
	else if (Recipe.RecipeName.Contains("Axe"))
	{
		FInventoryItem Tool = UInventoryItemLibrary::CreateToolItem(EToolType::Axe, Recipe.OutputMaterial);
		Inventory->AddItem(Tool);
	}
	else if (Recipe.RecipeName.Contains("Shovel"))
	{
		FInventoryItem Tool = UInventoryItemLibrary::CreateToolItem(EToolType::Shovel, Recipe.OutputMaterial);
		Inventory->AddItem(Tool);
	}
	else if (Recipe.RecipeName.Contains("Hammer"))
	{
		FInventoryItem Tool = UInventoryItemLibrary::CreateToolItem(EToolType::Hammer, Recipe.OutputMaterial);
		Inventory->AddItem(Tool);
	}
	else
	{
		// For materials, add directly
		Inventory->AddMaterial(Recipe.OutputMaterial, Recipe.OutputAmount);
	}

	return true;
}

bool UCraftingComponent::GetToolCraftingRecipe(EToolType ToolType, EMaterialType MaterialType, FCraftingRecipe& OutRecipe)
{
	if (!bToolRecipesInitialized)
	{
		InitializeToolRecipes();
	}

	FString ToolName;
	switch (ToolType)
	{
	case EToolType::Pickaxe: ToolName = "Pickaxe"; break;
	case EToolType::Axe: ToolName = "Axe"; break;
	case EToolType::Shovel: ToolName = "Shovel"; break;
	case EToolType::Hammer: ToolName = "Hammer"; break;
	default: return false;
	}

	FString MaterialName;
	switch (MaterialType)
	{
	case EMaterialType::Wood: MaterialName = "Wood"; break;
	case EMaterialType::Stone: MaterialName = "Stone"; break;
	case EMaterialType::Iron: MaterialName = "Iron"; break;
	case EMaterialType::Steel: MaterialName = "Steel"; break;
	default: return false;
	}

	FString RecipeName = MaterialName + " " + ToolName;

	for (const FCraftingRecipe& Recipe : ToolRecipes)
	{
		if (Recipe.RecipeName.Equals(RecipeName))
		{
			OutRecipe = Recipe;
			return true;
		}
	}

	return false;
}
