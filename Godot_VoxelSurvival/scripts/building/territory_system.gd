extends Node3D
class_name TerritorySystem

## Territory control system with influence spreading

## Grid size for territory
@export var grid_size: int = 100
@export var cell_size: float = 10.0

## Territory grid (position -> owner_id)
var territory_grid: Dictionary = {}

## Influence from buildings
var building_influences: Dictionary = {}

## Update territory every frame
func _process(_delta: float):
	# Update territory influence (could be optimized to only update on changes)
	pass

## Check if can build at position
func can_build_at(position: Vector3, owner_id: int = 0) -> bool:
	var grid_pos = world_to_grid(position)
	var current_owner = territory_grid.get(grid_pos, -1)
	
	# Can build in unclaimed territory or own territory
	return current_owner == -1 or current_owner == owner_id

## Add building influence
func add_building_influence(position: Vector3, owner_id: int, radius: float = 5.0):
	var grid_pos = world_to_grid(position)
	building_influences[grid_pos] = {"owner_id": owner_id, "radius": radius}
	
	# Spread influence
	spread_influence(grid_pos, owner_id, int(radius / cell_size))

## Remove building influence
func remove_building_influence(position: Vector3, owner_id: int):
	var grid_pos = world_to_grid(position)
	building_influences.erase(grid_pos)
	
	# Recalculate territory
	recalculate_territory()

## Spread influence from position
func spread_influence(grid_pos: Vector2i, owner_id: int, radius: int):
	for x in range(grid_pos.x - radius, grid_pos.x + radius + 1):
		for y in range(grid_pos.y - radius, grid_pos.y + radius + 1):
			var pos = Vector2i(x, y)
			var distance = grid_pos.distance_to(pos)
			
			if distance <= radius:
				# Claim territory
				territory_grid[pos] = owner_id

## Recalculate entire territory
func recalculate_territory():
	territory_grid.clear()
	
	# Reapply all building influences
	for grid_pos in building_influences.keys():
		var influence = building_influences[grid_pos]
		spread_influence(grid_pos, influence.owner_id, int(influence.radius / cell_size))

## Convert world position to grid position
func world_to_grid(world_pos: Vector3) -> Vector2i:
	return Vector2i(
		int(floor(world_pos.x / cell_size)),
		int(floor(world_pos.z / cell_size))
	)

## Convert grid position to world position
func grid_to_world(grid_pos: Vector2i) -> Vector3:
	return Vector3(
		grid_pos.x * cell_size,
		0,
		grid_pos.y * cell_size
	)

## Get owner at position
func get_owner_at(position: Vector3) -> int:
	var grid_pos = world_to_grid(position)
	return territory_grid.get(grid_pos, -1)

## Get territory count for owner
func get_territory_count(owner_id: int) -> int:
	var count = 0
	for owner in territory_grid.values():
		if owner == owner_id:
			count += 1
	return count

## Serialize territory
func serialize() -> Dictionary:
	return {
		"territory_grid": territory_grid.duplicate(),
		"building_influences": building_influences.duplicate()
	}

## Deserialize territory
func deserialize(data: Dictionary):
	territory_grid = data.get("territory_grid", {})
	building_influences = data.get("building_influences", {})
