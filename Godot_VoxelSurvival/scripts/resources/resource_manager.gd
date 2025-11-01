extends Node
class_name ResourceManager

## Manages resource nodes and spawning

## Resource node scene (would be set in editor)
var resource_node_scene: PackedScene

## Active resource nodes
var resource_nodes: Array = []

## Spawn parameters
@export var spawn_radius: float = 100.0
@export var max_nodes: int = 50

## Reference to voxel world
var voxel_world: VoxelWorld

func _ready():
	# Find voxel world
	voxel_world = get_tree().get_first_node_in_group("voxel_world")

## Spawn resource node at position
func spawn_resource_node(type: ResourceNode.ResourceType, position: Vector3) -> ResourceNode:
	var node = ResourceNode.new()
	node.resource_type = type
	add_child(node)
	node.global_position = position
	resource_nodes.append(node)
	return node

## Spawn random resource nodes
func spawn_random_resources(center: Vector3, count: int):
	for i in range(count):
		if resource_nodes.size() >= max_nodes:
			break
		
		# Random position around center
		var angle = randf() * TAU
		var distance = randf() * spawn_radius
		var offset = Vector3(cos(angle) * distance, 0, sin(angle) * distance)
		var position = center + offset
		
		# Random resource type
		var type = ResourceNode.ResourceType.values()[randi() % ResourceNode.ResourceType.size()]
		
		spawn_resource_node(type, position)

## Get nearest resource node
func get_nearest_resource(position: Vector3, type: ResourceNode.ResourceType = -1) -> ResourceNode:
	var nearest: ResourceNode = null
	var nearest_distance = INF
	
	for node in resource_nodes:
		if node.is_depleted:
			continue
		
		if type != -1 and node.resource_type != type:
			continue
		
		var distance = node.global_position.distance_to(position)
		if distance < nearest_distance:
			nearest_distance = distance
			nearest = node
	
	return nearest

## Remove resource node
func remove_resource_node(node: ResourceNode):
	if node in resource_nodes:
		resource_nodes.erase(node)
		node.queue_free()

## Clear all resource nodes
func clear_resources():
	for node in resource_nodes:
		node.queue_free()
	resource_nodes.clear()
