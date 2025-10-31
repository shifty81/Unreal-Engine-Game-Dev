// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingRecipe.h"

TArray<FCraftingRecipe> UCraftingRecipeLibrary::DefaultRecipes;
bool UCraftingRecipeLibrary::bRecipesInitialized = false;

void UCraftingRecipeLibrary::InitializeDefaultRecipes()
{
	if (bRecipesInitialized)
	{
		return;
	}

	DefaultRecipes.Empty();

	// Charcoal from Wood (Salvage/Processing)
	FCraftingRecipe CharcoalRecipe;
	CharcoalRecipe.RecipeName = TEXT("Charcoal");
	CharcoalRecipe.OutputMaterial = EMaterialType::Charcoal;
	CharcoalRecipe.OutputAmount = 1;
	CharcoalRecipe.CraftingTime = 10.0f;
	CharcoalRecipe.bRequiresForge = true;
	CharcoalRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::Wood, 2 });
	DefaultRecipes.Add(CharcoalRecipe);

	// Steel from Iron and Coal
	FCraftingRecipe SteelFromCoalRecipe;
	SteelFromCoalRecipe.RecipeName = TEXT("Steel (Coal)");
	SteelFromCoalRecipe.OutputMaterial = EMaterialType::Steel;
	SteelFromCoalRecipe.OutputAmount = 1;
	SteelFromCoalRecipe.CraftingTime = 15.0f;
	SteelFromCoalRecipe.bRequiresForge = true;
	SteelFromCoalRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::Iron, 2 });
	SteelFromCoalRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::Coal, 1 });
	DefaultRecipes.Add(SteelFromCoalRecipe);

	// Steel from Iron and Charcoal
	FCraftingRecipe SteelFromCharcoalRecipe;
	SteelFromCharcoalRecipe.RecipeName = TEXT("Steel (Charcoal)");
	SteelFromCharcoalRecipe.OutputMaterial = EMaterialType::Steel;
	SteelFromCharcoalRecipe.OutputAmount = 1;
	SteelFromCharcoalRecipe.CraftingTime = 15.0f;
	SteelFromCharcoalRecipe.bRequiresForge = true;
	SteelFromCharcoalRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::Iron, 2 });
	SteelFromCharcoalRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::Charcoal, 1 });
	DefaultRecipes.Add(SteelFromCharcoalRecipe);

	// Smelting recipes - Convert ores to refined materials
	// Iron
	FCraftingRecipe IronRecipe;
	IronRecipe.RecipeName = TEXT("Iron");
	IronRecipe.OutputMaterial = EMaterialType::Iron;
	IronRecipe.OutputAmount = 1;
	IronRecipe.CraftingTime = 8.0f;
	IronRecipe.bRequiresForge = true;
	IronRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::IronOre, 1 });
	DefaultRecipes.Add(IronRecipe);

	// Copper
	FCraftingRecipe CopperRecipe;
	CopperRecipe.RecipeName = TEXT("Copper");
	CopperRecipe.OutputMaterial = EMaterialType::Copper;
	CopperRecipe.OutputAmount = 1;
	CopperRecipe.CraftingTime = 8.0f;
	CopperRecipe.bRequiresForge = true;
	CopperRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::CopperOre, 1 });
	DefaultRecipes.Add(CopperRecipe);

	// Gold
	FCraftingRecipe GoldRecipe;
	GoldRecipe.RecipeName = TEXT("Gold");
	GoldRecipe.OutputMaterial = EMaterialType::Gold;
	GoldRecipe.OutputAmount = 1;
	GoldRecipe.CraftingTime = 10.0f;
	GoldRecipe.bRequiresForge = true;
	GoldRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::GoldOre, 1 });
	DefaultRecipes.Add(GoldRecipe);

	// Aluminum
	FCraftingRecipe AluminumRecipe;
	AluminumRecipe.RecipeName = TEXT("Aluminum");
	AluminumRecipe.OutputMaterial = EMaterialType::Aluminum;
	AluminumRecipe.OutputAmount = 1;
	AluminumRecipe.CraftingTime = 8.0f;
	AluminumRecipe.bRequiresForge = true;
	AluminumRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::AluminumOre, 1 });
	DefaultRecipes.Add(AluminumRecipe);

	// Titanium
	FCraftingRecipe TitaniumRecipe;
	TitaniumRecipe.RecipeName = TEXT("Titanium");
	TitaniumRecipe.OutputMaterial = EMaterialType::Titanium;
	TitaniumRecipe.OutputAmount = 1;
	TitaniumRecipe.CraftingTime = 12.0f;
	TitaniumRecipe.bRequiresForge = true;
	TitaniumRecipe.Ingredients.Add(FRecipeIngredient{ EMaterialType::TitaniumOre, 1 });
	DefaultRecipes.Add(TitaniumRecipe);

	bRecipesInitialized = true;
}

TArray<FCraftingRecipe> UCraftingRecipeLibrary::GetAllRecipes()
{
	if (!bRecipesInitialized)
	{
		InitializeDefaultRecipes();
	}
	return DefaultRecipes;
}

bool UCraftingRecipeLibrary::GetRecipeForMaterial(EMaterialType MaterialType, FCraftingRecipe& OutRecipe)
{
	if (!bRecipesInitialized)
	{
		InitializeDefaultRecipes();
	}

	for (const FCraftingRecipe& Recipe : DefaultRecipes)
	{
		if (Recipe.OutputMaterial == MaterialType)
		{
			OutRecipe = Recipe;
			return true;
		}
	}

	return false;
}
