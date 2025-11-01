extends Node3D
class_name ResourceNode

## Resource node that can be harvested

## Resource types
enum ResourceType {
	WOOD,
	STONE,
	IRON,
	GOLD,
	WATER
}

## Resource node properties
@export var resource_type: ResourceType = ResourceType.STONE
@export var max_yield: int = 100
@export var current_yield: int = 100
@export var respawn_time: float = 60.0

## Visual mesh
var mesh_instance: MeshInstance3D

## Respawn timer
var respawn_timer: float = 0.0
var is_depleted: bool = false

func _ready():
	# Create visual representation
	mesh_instance = MeshInstance3D.new()
	add_child(mesh_instance)
	
	var mesh = BoxMesh.new()
	mesh.size = Vector3(2, 2, 2)
	mesh_instance.mesh = mesh
	
	# Set color based on resource type
	var material = StandardMaterial3D.new()
	match resource_type:
		ResourceType.WOOD:
			material.albedo_color = Color(0.4, 0.25, 0.1)
		ResourceType.STONE:
			material.albedo_color = Color(0.5, 0.5, 0.5)
		ResourceType.IRON:
			material.albedo_color = Color(0.7, 0.7, 0.7)
		ResourceType.GOLD:
			material.albedo_color = Color(1.0, 0.84, 0.0)
		ResourceType.WATER:
			material.albedo_color = Color(0.2, 0.4, 0.8, 0.6)
	
	mesh_instance.material_override = material

func _process(delta: float):
	# Handle respawn
	if is_depleted:
		respawn_timer += delta
		if respawn_timer >= respawn_time:
			respawn()

## Harvest resources
func harvest(amount: int = 10) -> Dictionary:
	if is_depleted:
		return {"type": resource_type, "amount": 0}
	
	var harvested = min(amount, current_yield)
	current_yield -= harvested
	
	if current_yield <= 0:
		deplete()
	
	return {"type": resource_type, "amount": harvested}

## Deplete resource node
func deplete():
	is_depleted = true
	respawn_timer = 0.0
	mesh_instance.visible = false

## Respawn resource node
func respawn():
	is_depleted = false
	current_yield = max_yield
	mesh_instance.visible = true

## Get resource type as voxel type
func get_voxel_type() -> VoxelData.VoxelType:
	match resource_type:
		ResourceType.WOOD:
			return VoxelData.VoxelType.WOOD
		ResourceType.STONE:
			return VoxelData.VoxelType.STONE
		ResourceType.IRON:
			return VoxelData.VoxelType.IRON
		ResourceType.GOLD:
			return VoxelData.VoxelType.GOLD
		ResourceType.WATER:
			return VoxelData.VoxelType.WATER_SOURCE
		_:
			return VoxelData.VoxelType.STONE
