// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxInventorySlots = 40;
	HotbarSlots = 10;
	SelectedHotbarSlot = 0;

	SetIsReplicatedComponent(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize inventory slots
	InventorySlots.SetNum(MaxInventorySlots);
	for (int32 i = 0; i < MaxInventorySlots; i++)
	{
		InventorySlots[i] = FInventoryItem(); // Empty slot
	}

	// Initialize item library
	UInventoryItemLibrary::InitializeDefaultItems();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddItem(FInventoryItem Item)
{
	if (!Item.IsValid())
	{
		return false;
	}

	// Try to stack with existing items
	if (Item.IsStackable())
	{
		int32 StackSlot = FindStackableSlot(Item);
		if (StackSlot != INDEX_NONE)
		{
			FInventoryItem& ExistingItem = InventorySlots[StackSlot];
			int32 SpaceLeft = ExistingItem.MaxStackSize - ExistingItem.StackCount;
			int32 AmountToAdd = FMath::Min(SpaceLeft, Item.StackCount);
			
			ExistingItem.StackCount += AmountToAdd;
			Item.StackCount -= AmountToAdd;

			NotifyInventoryChanged();
			OnItemAdded.Broadcast(ExistingItem, StackSlot);

			// If still items left, try to add to another slot
			if (Item.StackCount > 0)
			{
				return AddItem(Item);
			}
			return true;
		}
	}

	// Find empty slot
	int32 EmptySlot = GetFirstEmptySlot();
	if (EmptySlot == INDEX_NONE)
	{
		return false; // Inventory full
	}

	InventorySlots[EmptySlot] = Item;
	NotifyInventoryChanged();
	OnItemAdded.Broadcast(Item, EmptySlot);

	return true;
}

int32 UInventoryComponent::AddMaterial(EMaterialType MaterialType, int32 Amount)
{
	FInventoryItem MaterialItem = UInventoryItemLibrary::CreateMaterialItem(MaterialType, Amount);
	
	int32 TotalAdded = 0;
	int32 Remaining = Amount;

	// Try to stack with existing materials
	for (int32 i = 0; i < InventorySlots.Num() && Remaining > 0; i++)
	{
		FInventoryItem& Slot = InventorySlots[i];
		if (Slot.IsValid() && Slot.MaterialType == MaterialType && Slot.IsStackable())
		{
			int32 SpaceLeft = Slot.MaxStackSize - Slot.StackCount;
			if (SpaceLeft > 0)
			{
				int32 AmountToAdd = FMath::Min(SpaceLeft, Remaining);
				Slot.StackCount += AmountToAdd;
				Remaining -= AmountToAdd;
				TotalAdded += AmountToAdd;
				NotifyInventoryChanged();
			}
		}
	}

	// Add to new slots if remaining
	while (Remaining > 0)
	{
		int32 EmptySlot = GetFirstEmptySlot();
		if (EmptySlot == INDEX_NONE)
		{
			break; // Inventory full
		}

		FInventoryItem NewItem = UInventoryItemLibrary::CreateMaterialItem(MaterialType, 1);
		int32 AmountToAdd = FMath::Min(NewItem.MaxStackSize, Remaining);
		NewItem.StackCount = AmountToAdd;

		InventorySlots[EmptySlot] = NewItem;
		Remaining -= AmountToAdd;
		TotalAdded += AmountToAdd;
		NotifyInventoryChanged();
	}

	return TotalAdded;
}

bool UInventoryComponent::RemoveItem(int32 SlotIndex, int32 Amount)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	FInventoryItem& Item = InventorySlots[SlotIndex];
	if (!Item.IsValid())
	{
		return false;
	}

	if (Item.StackCount < Amount)
	{
		return false;
	}

	Item.StackCount -= Amount;

	if (Item.StackCount <= 0)
	{
		FInventoryItem RemovedItem = Item;
		Item = FInventoryItem(); // Clear slot
		OnItemRemoved.Broadcast(RemovedItem, SlotIndex);
	}

	NotifyInventoryChanged();
	return true;
}

int32 UInventoryComponent::RemoveMaterial(EMaterialType MaterialType, int32 Amount)
{
	int32 TotalRemoved = 0;
	int32 Remaining = Amount;

	for (int32 i = 0; i < InventorySlots.Num() && Remaining > 0; i++)
	{
		FInventoryItem& Slot = InventorySlots[i];
		if (Slot.IsValid() && Slot.MaterialType == MaterialType)
		{
			int32 AmountToRemove = FMath::Min(Slot.StackCount, Remaining);
			Slot.StackCount -= AmountToRemove;
			Remaining -= AmountToRemove;
			TotalRemoved += AmountToRemove;

			if (Slot.StackCount <= 0)
			{
				Slot = FInventoryItem(); // Clear slot
			}

			NotifyInventoryChanged();
		}
	}

	return TotalRemoved;
}

FInventoryItem UInventoryComponent::GetItemAtSlot(int32 SlotIndex) const
{
	if (InventorySlots.IsValidIndex(SlotIndex))
	{
		return InventorySlots[SlotIndex];
	}
	return FInventoryItem();
}

FInventoryItem UInventoryComponent::GetEquippedItem() const
{
	if (SelectedHotbarSlot >= 0 && SelectedHotbarSlot < HotbarSlots)
	{
		return GetItemAtSlot(SelectedHotbarSlot);
	}
	return FInventoryItem();
}

bool UInventoryComponent::HasMaterial(EMaterialType MaterialType, int32 Amount) const
{
	return GetMaterialCount(MaterialType) >= Amount;
}

int32 UInventoryComponent::GetMaterialCount(EMaterialType MaterialType) const
{
	int32 Total = 0;
	for (const FInventoryItem& Item : InventorySlots)
	{
		if (Item.IsValid() && Item.MaterialType == MaterialType)
		{
			Total += Item.StackCount;
		}
	}
	return Total;
}

bool UInventoryComponent::MoveItem(int32 FromSlot, int32 ToSlot)
{
	if (!InventorySlots.IsValidIndex(FromSlot) || !InventorySlots.IsValidIndex(ToSlot))
	{
		return false;
	}

	FInventoryItem Temp = InventorySlots[FromSlot];
	InventorySlots[FromSlot] = InventorySlots[ToSlot];
	InventorySlots[ToSlot] = Temp;

	NotifyInventoryChanged();
	return true;
}

void UInventoryComponent::SelectHotbarSlot(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < HotbarSlots)
	{
		SelectedHotbarSlot = SlotIndex;
		NotifyInventoryChanged();
	}
}

bool UInventoryComponent::IsSlotEmpty(int32 SlotIndex) const
{
	if (InventorySlots.IsValidIndex(SlotIndex))
	{
		return !InventorySlots[SlotIndex].IsValid();
	}
	return true;
}

int32 UInventoryComponent::GetFirstEmptySlot() const
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (!InventorySlots[i].IsValid())
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::FindItemSlot(FName ItemID) const
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i].IsValid() && InventorySlots[i].ItemID == ItemID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::FindStackableSlot(const FInventoryItem& Item) const
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		const FInventoryItem& Slot = InventorySlots[i];
		if (Slot.IsValid() && 
			Slot.ItemID == Item.ItemID && 
			Slot.CanAddToStack(Item.StackCount))
		{
			return i;
		}
	}
	return INDEX_NONE;
}

void UInventoryComponent::NotifyInventoryChanged()
{
	OnInventoryChanged.Broadcast();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, InventorySlots);
	DOREPLIFETIME(UInventoryComponent, SelectedHotbarSlot);
}
