// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryItem.h"

TMap<FName, FInventoryItem> UInventoryItemLibrary::ItemDefinitions;
bool UInventoryItemLibrary::bItemsInitialized = false;

void UInventoryItemLibrary::InitializeDefaultItems()
{
	if (bItemsInitialized)
	{
		return;
	}

	ItemDefinitions.Empty();

	// Material items
	TArray<EMaterialType> Materials = {
		EMaterialType::Wood, EMaterialType::Stone, EMaterialType::Iron,
		EMaterialType::Copper, EMaterialType::Gold, EMaterialType::Aluminum,
		EMaterialType::Titanium, EMaterialType::Steel, EMaterialType::Charcoal,
		EMaterialType::Coal, EMaterialType::IronOre, EMaterialType::CopperOre,
		EMaterialType::GoldOre, EMaterialType::AluminumOre, EMaterialType::TitaniumOre
	};

	for (EMaterialType Mat : Materials)
	{
		FInventoryItem Item;
		Item.Category = EItemCategory::Resource;
		Item.MaterialType = Mat;
		Item.MaxStackSize = 999;
		Item.bCanEquip = false;
		
		switch (Mat)
		{
		case EMaterialType::Wood:
			Item.ItemID = FName("Wood");
			Item.DisplayName = FText::FromString("Wood");
			Item.Description = FText::FromString("Basic building material from trees");
			break;
		case EMaterialType::Stone:
			Item.ItemID = FName("Stone");
			Item.DisplayName = FText::FromString("Stone");
			Item.Description = FText::FromString("Common stone material");
			break;
		case EMaterialType::Iron:
			Item.ItemID = FName("Iron");
			Item.DisplayName = FText::FromString("Iron Ingot");
			Item.Description = FText::FromString("Refined iron metal");
			break;
		case EMaterialType::Steel:
			Item.ItemID = FName("Steel");
			Item.DisplayName = FText::FromString("Steel Ingot");
			Item.Description = FText::FromString("Strong steel alloy");
			break;
		case EMaterialType::Copper:
			Item.ItemID = FName("Copper");
			Item.DisplayName = FText::FromString("Copper Ingot");
			Item.Description = FText::FromString("Refined copper metal");
			break;
		case EMaterialType::Gold:
			Item.ItemID = FName("Gold");
			Item.DisplayName = FText::FromString("Gold Ingot");
			Item.Description = FText::FromString("Precious gold metal");
			break;
		case EMaterialType::Aluminum:
			Item.ItemID = FName("Aluminum");
			Item.DisplayName = FText::FromString("Aluminum Ingot");
			Item.Description = FText::FromString("Lightweight aluminum metal");
			break;
		case EMaterialType::Titanium:
			Item.ItemID = FName("Titanium");
			Item.DisplayName = FText::FromString("Titanium Ingot");
			Item.Description = FText::FromString("Advanced titanium metal");
			break;
		case EMaterialType::Charcoal:
			Item.ItemID = FName("Charcoal");
			Item.DisplayName = FText::FromString("Charcoal");
			Item.Description = FText::FromString("Processed fuel from wood");
			break;
		case EMaterialType::Coal:
			Item.ItemID = FName("Coal");
			Item.DisplayName = FText::FromString("Coal");
			Item.Description = FText::FromString("Fuel for forges");
			break;
		default:
			Item.ItemID = FName(TEXT("Material_") + FString::FromInt((int32)Mat));
			Item.DisplayName = FText::FromString("Material");
			break;
		}
		
		ItemDefinitions.Add(Item.ItemID, Item);
	}

	// Tool definitions
	TArray<TPair<EToolType, EMaterialType>> Tools = {
		{EToolType::Pickaxe, EMaterialType::Stone},
		{EToolType::Pickaxe, EMaterialType::Iron},
		{EToolType::Pickaxe, EMaterialType::Steel},
		{EToolType::Axe, EMaterialType::Stone},
		{EToolType::Axe, EMaterialType::Iron},
		{EToolType::Axe, EMaterialType::Steel},
		{EToolType::Shovel, EMaterialType::Stone},
		{EToolType::Shovel, EMaterialType::Iron},
		{EToolType::Hammer, EMaterialType::Iron}
	};

	for (const auto& Tool : Tools)
	{
		FInventoryItem Item = CreateToolItem(Tool.Key, Tool.Value);
		ItemDefinitions.Add(Item.ItemID, Item);
	}

	bItemsInitialized = true;
}

bool UInventoryItemLibrary::GetItemDefinition(FName ItemID, FInventoryItem& OutItem)
{
	if (!bItemsInitialized)
	{
		InitializeDefaultItems();
	}

	if (ItemDefinitions.Contains(ItemID))
	{
		OutItem = ItemDefinitions[ItemID];
		return true;
	}

	return false;
}

FInventoryItem UInventoryItemLibrary::CreateMaterialItem(EMaterialType MaterialType, int32 Amount)
{
	FInventoryItem Item;
	Item.Category = EItemCategory::Resource;
	Item.MaterialType = MaterialType;
	Item.StackCount = Amount;
	Item.MaxStackSize = 999;
	
	// Set ID based on material type
	Item.ItemID = FName(*FString::Printf(TEXT("Mat_%d"), (int32)MaterialType));
	
	return Item;
}

FInventoryItem UInventoryItemLibrary::CreateToolItem(EToolType ToolType, EMaterialType MaterialType)
{
	FInventoryItem Item;
	Item.Category = EItemCategory::Tool;
	Item.ToolType = ToolType;
	Item.MaterialType = MaterialType;
	Item.MaxStackSize = 1;
	Item.StackCount = 1;
	Item.bCanEquip = true;

	// Set properties based on tool type and material
	FString ToolName;
	FString MaterialName;
	
	switch (ToolType)
	{
	case EToolType::Pickaxe:
		ToolName = "Pickaxe";
		Item.HarvestPower = 2.0f;
		Item.Damage = 10.0f;
		break;
	case EToolType::Axe:
		ToolName = "Axe";
		Item.HarvestPower = 3.0f;
		Item.Damage = 15.0f;
		break;
	case EToolType::Shovel:
		ToolName = "Shovel";
		Item.HarvestPower = 2.5f;
		Item.Damage = 5.0f;
		break;
	case EToolType::Hammer:
		ToolName = "Hammer";
		Item.HarvestPower = 1.5f;
		Item.Damage = 20.0f;
		break;
	default:
		ToolName = "Tool";
		break;
	}

	switch (MaterialType)
	{
	case EMaterialType::Stone:
		MaterialName = "Stone";
		Item.Durability = 100.0f;
		Item.HarvestPower *= 1.0f;
		break;
	case EMaterialType::Iron:
		MaterialName = "Iron";
		Item.Durability = 200.0f;
		Item.HarvestPower *= 1.5f;
		break;
	case EMaterialType::Steel:
		MaterialName = "Steel";
		Item.Durability = 400.0f;
		Item.HarvestPower *= 2.0f;
		break;
	default:
		MaterialName = "Basic";
		Item.Durability = 50.0f;
		break;
	}

	Item.MaxDurability = Item.Durability;
	Item.ItemID = FName(*(MaterialName + ToolName));
	Item.DisplayName = FText::FromString(MaterialName + " " + ToolName);
	Item.Description = FText::FromString("A " + MaterialName.ToLower() + " " + ToolName.ToLower() + " for harvesting resources");

	return Item;
}
