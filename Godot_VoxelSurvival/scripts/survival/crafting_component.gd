extends Node
class_name CraftingComponent

## Crafting system component

## Crafting recipes
var recipes: Array = []

## Reference to inventory
var inventory: InventoryComponent

func _ready():
	# Initialize default recipes
	init_default_recipes()

## Initialize default crafting recipes
func init_default_recipes():
	# Example recipes
	recipes.append({
		"name": "Wood Planks",
		"result": {"type": VoxelData.VoxelType.WOOD, "count": 4},
		"ingredients": [
			{"type": VoxelData.VoxelType.WOOD, "count": 1}
		]
	})
	
	recipes.append({
		"name": "Iron Tools",
		"result": {"type": VoxelData.VoxelType.IRON, "count": 1},
		"ingredients": [
			{"type": VoxelData.VoxelType.IRON, "count": 3},
			{"type": VoxelData.VoxelType.WOOD, "count": 2}
		]
	})

## Check if recipe can be crafted
func can_craft(recipe: Dictionary) -> bool:
	if not inventory:
		return false
	
	for ingredient in recipe.ingredients:
		if not inventory.has_item(ingredient.type, ingredient.count):
			return false
	
	return true

## Craft item from recipe
func craft(recipe: Dictionary) -> bool:
	if not can_craft(recipe):
		return false
	
	# Remove ingredients
	for ingredient in recipe.ingredients:
		if not inventory.remove_item(ingredient.type, ingredient.count):
			return false
	
	# Add result
	inventory.add_item(recipe.result.type, recipe.result.count)
	
	return true

## Get all available recipes
func get_recipes() -> Array:
	return recipes

## Get craftable recipes
func get_craftable_recipes() -> Array:
	var craftable = []
	for recipe in recipes:
		if can_craft(recipe):
			craftable.append(recipe)
	return craftable
