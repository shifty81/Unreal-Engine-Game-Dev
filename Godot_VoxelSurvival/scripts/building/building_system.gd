extends Node3D
class_name BuildingSystem

## Building system for base construction and territory control

## Building types
enum BuildingType {
	FOUNDATION,
	WALL,
	FLOOR,
	ROOF,
	DOOR,
	WINDOW,
	HANGAR,
	FORGE,
	SALVAGE_STATION
}

## Building data structure
class Building:
	var type: BuildingType
	var position: Vector3
	var rotation: float
	var health: float = 100.0
	var owner_id: int = 0
	
	func _init(t: BuildingType, pos: Vector3, rot: float = 0.0):
		type = t
		position = pos
		rotation = rot

## Placed buildings
var buildings: Array = []

## Territory system reference
var territory_system: TerritorySystem

func _ready():
	# Find territory system
	territory_system = get_tree().get_first_node_in_group("territory_system")

## Place building
func place_building(type: BuildingType, position: Vector3, rotation: float = 0.0, owner_id: int = 0) -> bool:
	# Check if can build at position
	if not can_build_at(position):
		return false
	
	# Create building
	var building = Building.new(type, position, rotation)
	building.owner_id = owner_id
	buildings.append(building)
	
	# Update territory if applicable
	if territory_system:
		territory_system.add_building_influence(position, owner_id)
	
	# Spawn visual representation
	spawn_building_visual(building)
	
	return true

## Check if can build at position
func can_build_at(position: Vector3) -> bool:
	# Check if too close to other buildings
	for building in buildings:
		if building.position.distance_to(position) < 2.0:
			return false
	
	# Check territory control if needed
	if territory_system:
		return territory_system.can_build_at(position)
	
	return true

## Remove building
func remove_building(position: Vector3) -> bool:
	for i in range(buildings.size()):
		if buildings[i].position.distance_to(position) < 1.0:
			var building = buildings[i]
			buildings.remove_at(i)
			
			# Update territory
			if territory_system:
				territory_system.remove_building_influence(position, building.owner_id)
			
			return true
	
	return false

## Spawn visual representation of building
func spawn_building_visual(building: Building):
	var mesh_instance = MeshInstance3D.new()
	add_child(mesh_instance)
	
	# Create simple box mesh for now
	var mesh = BoxMesh.new()
	mesh.size = Vector3(2, 3, 2)
	mesh_instance.mesh = mesh
	
	# Set position
	mesh_instance.global_position = building.position
	mesh_instance.rotation.y = building.rotation
	
	# Set color based on type
	var material = StandardMaterial3D.new()
	match building.type:
		BuildingType.FOUNDATION:
			material.albedo_color = Color(0.4, 0.4, 0.4)
		BuildingType.WALL:
			material.albedo_color = Color(0.6, 0.5, 0.4)
		BuildingType.HANGAR:
			material.albedo_color = Color(0.3, 0.3, 0.5)
		BuildingType.FORGE:
			material.albedo_color = Color(0.8, 0.3, 0.1)
		_:
			material.albedo_color = Color(0.5, 0.5, 0.5)
	
	mesh_instance.material_override = material

## Get buildings in radius
func get_buildings_in_radius(position: Vector3, radius: float) -> Array:
	var result = []
	for building in buildings:
		if building.position.distance_to(position) <= radius:
			result.append(building)
	return result

## Serialize buildings
func serialize() -> Array:
	var data = []
	for building in buildings:
		data.append({
			"type": building.type,
			"position": var_to_str(building.position),
			"rotation": building.rotation,
			"health": building.health,
			"owner_id": building.owner_id
		})
	return data

## Deserialize buildings
func deserialize(data: Array):
	buildings.clear()
	for building_data in data:
		var building = Building.new(
			building_data.type,
			str_to_var(building_data.position),
			building_data.rotation
		)
		building.health = building_data.health
		building.owner_id = building_data.owner_id
		buildings.append(building)
		spawn_building_visual(building)
