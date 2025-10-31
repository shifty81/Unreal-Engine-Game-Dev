// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "InventoryComponent.generated.h"

/**
 * Delegate for inventory changes
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, FInventoryItem, Item, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, FInventoryItem, Item, int32, SlotIndex);

/**
 * Inventory component for managing player items
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOXELSURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
	TArray<FInventoryItem> InventorySlots;

	/** Maximum inventory size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySlots;

	/** Hotbar size (subset of inventory) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 HotbarSlots;

	/** Currently selected hotbar slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
	int32 SelectedHotbarSlot;

	/** Inventory changed event */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	/** Item added event */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemAdded OnItemAdded;

	/** Item removed event */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemRemoved OnItemRemoved;

	/**
	 * Add item to inventory
	 * @param Item - Item to add
	 * @return True if item was added successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FInventoryItem Item);

	/**
	 * Add material to inventory
	 * @param MaterialType - Type of material
	 * @param Amount - Amount to add
	 * @return Amount actually added
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddMaterial(EMaterialType MaterialType, int32 Amount);

	/**
	 * Remove item from slot
	 * @param SlotIndex - Slot to remove from
	 * @param Amount - Amount to remove
	 * @return True if removed successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(int32 SlotIndex, int32 Amount = 1);

	/**
	 * Remove material from inventory
	 * @param MaterialType - Type of material
	 * @param Amount - Amount to remove
	 * @return Amount actually removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 RemoveMaterial(EMaterialType MaterialType, int32 Amount);

	/**
	 * Get item at slot
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	FInventoryItem GetItemAtSlot(int32 SlotIndex) const;

	/**
	 * Get currently equipped item (from selected hotbar slot)
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	FInventoryItem GetEquippedItem() const;

	/**
	 * Check if inventory has material
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasMaterial(EMaterialType MaterialType, int32 Amount) const;

	/**
	 * Get material count
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetMaterialCount(EMaterialType MaterialType) const;

	/**
	 * Move item between slots
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MoveItem(int32 FromSlot, int32 ToSlot);

	/**
	 * Select hotbar slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectHotbarSlot(int32 SlotIndex);

	/**
	 * Check if slot is empty
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool IsSlotEmpty(int32 SlotIndex) const;

	/**
	 * Get first empty slot
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetFirstEmptySlot() const;

	/**
	 * Find item slot by ID
	 */
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 FindItemSlot(FName ItemID) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Find or create slot for stackable item */
	int32 FindStackableSlot(const FInventoryItem& Item) const;

	/** Notify inventory changed */
	void NotifyInventoryChanged();
};
