extends Node
class_name InventoryComponent

## Inventory management component

## Maximum inventory slots
@export var max_slots: int = 40

## Current inventory items (VoxelType -> count)
var items: Dictionary = {}

## Selected item type for placing
var selected_item_type: VoxelData.VoxelType = VoxelData.VoxelType.STONE

## Add item to inventory
func add_item(item_type: VoxelData.VoxelType, count: int) -> bool:
	if item_type == VoxelData.VoxelType.AIR:
		return false
	
	if items.has(item_type):
		items[item_type] += count
	else:
		# Check if we have space
		if items.size() >= max_slots:
			return false
		items[item_type] = count
	
	return true

## Remove item from inventory
func remove_item(item_type: VoxelData.VoxelType, count: int) -> bool:
	if not items.has(item_type):
		return false
	
	if items[item_type] < count:
		return false
	
	items[item_type] -= count
	if items[item_type] <= 0:
		items.erase(item_type)
	
	return true

## Get item count
func get_item_count(item_type: VoxelData.VoxelType) -> int:
	return items.get(item_type, 0)

## Check if has item
func has_item(item_type: VoxelData.VoxelType, count: int = 1) -> bool:
	return get_item_count(item_type) >= count

## Clear inventory
func clear():
	items.clear()

## Get all items
func get_all_items() -> Dictionary:
	return items.duplicate()

## Serialize inventory
func serialize() -> Dictionary:
	return {
		"items": items.duplicate(),
		"selected_item_type": selected_item_type
	}

## Deserialize inventory
func deserialize(data: Dictionary):
	items = data.get("items", {})
	selected_item_type = data.get("selected_item_type", VoxelData.VoxelType.STONE)
