// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CraftingRecipe.h"
#include "InventoryItem.generated.h"

/**
 * Enum defining item categories
 */
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Resource		UMETA(DisplayName = "Resource"),
	Tool			UMETA(DisplayName = "Tool"),
	Weapon			UMETA(DisplayName = "Weapon"),
	Consumable		UMETA(DisplayName = "Consumable"),
	Building		UMETA(DisplayName = "Building"),
	Miscellaneous	UMETA(DisplayName = "Miscellaneous")
};

/**
 * Enum defining tool types
 */
UENUM(BlueprintType)
enum class EToolType : uint8
{
	None			UMETA(DisplayName = "None"),
	Pickaxe			UMETA(DisplayName = "Pickaxe"),
	Axe				UMETA(DisplayName = "Axe"),
	Shovel			UMETA(DisplayName = "Shovel"),
	Hammer			UMETA(DisplayName = "Hammer")
};

/**
 * Structure defining an inventory item
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	/** Unique identifier for this item type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText DisplayName;

	/** Description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Description;

	/** Item category */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemCategory Category;

	/** Material type (for resources) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EMaterialType MaterialType;

	/** Tool type (for tools) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EToolType ToolType;

	/** Stack size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;

	/** Current stack count */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 StackCount;

	/** Icon texture path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString IconPath;

	/** Mesh for the item when equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	/** Can this item be equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bCanEquip;

	/** Durability (for tools/weapons) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Durability;

	/** Max durability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MaxDurability;

	/** Damage (for weapons) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Damage;

	/** Harvest power (for tools) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float HarvestPower;

	FInventoryItem()
		: ItemID(NAME_None)
		, DisplayName(FText::FromString("Unknown Item"))
		, Description(FText::FromString("No description"))
		, Category(EItemCategory::Miscellaneous)
		, MaterialType(EMaterialType::Wood)
		, ToolType(EToolType::None)
		, MaxStackSize(99)
		, StackCount(1)
		, IconPath("")
		, bCanEquip(false)
		, Durability(100.0f)
		, MaxDurability(100.0f)
		, Damage(0.0f)
		, HarvestPower(1.0f)
	{}

	/** Check if item is valid */
	bool IsValid() const { return ItemID != NAME_None; }

	/** Check if item is stackable */
	bool IsStackable() const { return MaxStackSize > 1; }

	/** Check if can add to stack */
	bool CanAddToStack(int32 Amount) const { return (StackCount + Amount) <= MaxStackSize; }
};

/**
 * Helper class for item definitions
 */
UCLASS()
class VOXELSURVIVAL_API UInventoryItemLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Get item definition by ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool GetItemDefinition(FName ItemID, FInventoryItem& OutItem);

	/**
	 * Create material item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FInventoryItem CreateMaterialItem(EMaterialType MaterialType, int32 Amount);

	/**
	 * Create tool item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FInventoryItem CreateToolItem(EToolType ToolType, EMaterialType MaterialType);

	/**
	 * Initialize default item definitions
	 */
	static void InitializeDefaultItems();

private:
	static TMap<FName, FInventoryItem> ItemDefinitions;
	static bool bItemsInitialized;
};
