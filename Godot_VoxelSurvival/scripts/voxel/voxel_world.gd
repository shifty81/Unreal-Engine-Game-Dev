extends Node3D
class_name VoxelWorld

## Manages the voxel world, including chunk generation and world generation
## Supports modding through data-driven world generation parameters

## Render distance in chunks
@export var render_distance: int = 8

## World generation seed
@export var world_seed: int = 12345

## Height map scale
@export var height_scale: float = 10.0

## Noise frequency for terrain generation
@export var noise_frequency: float = 0.01

## Chunk size (must match VoxelChunk)
@export var chunk_size: int = 16

## Voxel size (must match VoxelChunk)
@export var voxel_size: float = 1.0

## Map of loaded chunks
var loaded_chunks: Dictionary = {}

## Last player position for chunk loading
var last_player_position: Vector3 = Vector3.ZERO

## Noise generator for terrain
var noise: FastNoiseLite

## Reference to player for chunk loading
var player: Node3D

func _ready():
	# Initialize noise generator
	noise = FastNoiseLite.new()
	noise.seed = world_seed
	noise.frequency = noise_frequency
	noise.noise_type = FastNoiseLite.TYPE_PERLIN

func _process(_delta: float):
	# Update visible chunks around player
	if player:
		var player_pos = player.global_position
		if player_pos.distance_to(last_player_position) > chunk_size * voxel_size:
			update_visible_chunks(player_pos)
			last_player_position = player_pos

## Set the player reference for chunk loading
func set_player(p: Node3D):
	player = p
	if player:
		update_visible_chunks(player.global_position)

## Update visible chunks around player
func update_visible_chunks(player_position: Vector3):
	var player_chunk = world_to_chunk_coordinate(player_position)
	
	# Load chunks in render distance
	for x in range(player_chunk.x - render_distance, player_chunk.x + render_distance + 1):
		for y in range(max(player_chunk.y - 2, -4), min(player_chunk.y + 2, 4)):
			for z in range(player_chunk.z - render_distance, player_chunk.z + render_distance + 1):
				var coord = Vector3i(x, y, z)
				if not loaded_chunks.has(coord):
					get_or_create_chunk(coord)
	
	# Unload chunks outside render distance
	var chunks_to_remove = []
	for coord in loaded_chunks.keys():
		var distance = Vector3(coord.x - player_chunk.x, coord.y - player_chunk.y, coord.z - player_chunk.z).length()
		if distance > render_distance + 2:
			chunks_to_remove.append(coord)
	
	for coord in chunks_to_remove:
		unload_chunk(coord)

## Get or create chunk at coordinate
func get_or_create_chunk(chunk_coordinate: Vector3i) -> VoxelChunk:
	if loaded_chunks.has(chunk_coordinate):
		return loaded_chunks[chunk_coordinate]
	
	# Create new chunk
	var chunk = VoxelChunk.new()
	chunk.chunk_size = chunk_size
	chunk.voxel_size = voxel_size
	add_child(chunk)
	chunk.initialize_chunk(chunk_coordinate)
	
	# Generate terrain
	generate_chunk_terrain(chunk)
	
	# Generate mesh
	chunk.generate_mesh()
	
	# Store chunk
	loaded_chunks[chunk_coordinate] = chunk
	
	return chunk

## Unload chunk
func unload_chunk(chunk_coordinate: Vector3i):
	if loaded_chunks.has(chunk_coordinate):
		var chunk = loaded_chunks[chunk_coordinate]
		chunk.queue_free()
		loaded_chunks.erase(chunk_coordinate)

## Generate terrain for a chunk
func generate_chunk_terrain(chunk: VoxelChunk):
	for x in range(chunk_size):
		for z in range(chunk_size):
			# Get world position
			var world_x = chunk.chunk_coordinate.x * chunk_size + x
			var world_z = chunk.chunk_coordinate.z * chunk_size + z
			
			# Generate height using noise
			var height = perlin_noise(world_x, 0, world_z) * height_scale
			var world_height = int(height)
			
			for y in range(chunk_size):
				var world_y = chunk.chunk_coordinate.y * chunk_size + y
				
				if world_y < world_height - 3:
					# Stone layer
					chunk.set_voxel(x, y, z, VoxelData.VoxelType.STONE)
				elif world_y < world_height - 1:
					# Dirt layer
					chunk.set_voxel(x, y, z, VoxelData.VoxelType.DIRT)
				elif world_y < world_height:
					# Grass layer
					chunk.set_voxel(x, y, z, VoxelData.VoxelType.GRASS)
				elif world_y == 0:
					# Water level at y=0
					chunk.set_voxel(x, y, z, VoxelData.VoxelType.WATER_SOURCE)
				elif world_y < 0:
					# Water below sea level
					chunk.set_voxel(x, y, z, VoxelData.VoxelType.WATER)
				else:
					# Air
					chunk.set_voxel(x, y, z, VoxelData.VoxelType.AIR)

## Perlin noise function for terrain generation
func perlin_noise(x: float, y: float, z: float) -> float:
	return noise.get_noise_3d(x, y, z)

## Get chunk coordinate from world position
func world_to_chunk_coordinate(world_position: Vector3) -> Vector3i:
	var chunk_x = int(floor(world_position.x / (chunk_size * voxel_size)))
	var chunk_y = int(floor(world_position.y / (chunk_size * voxel_size)))
	var chunk_z = int(floor(world_position.z / (chunk_size * voxel_size)))
	return Vector3i(chunk_x, chunk_y, chunk_z)

## Get local voxel coordinate within chunk
func world_to_local_voxel(world_position: Vector3) -> Dictionary:
	var chunk_coord = world_to_chunk_coordinate(world_position)
	var local_x = int(floor(world_position.x / voxel_size)) % chunk_size
	var local_y = int(floor(world_position.y / voxel_size)) % chunk_size
	var local_z = int(floor(world_position.z / voxel_size)) % chunk_size
	
	# Handle negative coordinates
	if local_x < 0:
		local_x += chunk_size
	if local_y < 0:
		local_y += chunk_size
	if local_z < 0:
		local_z += chunk_size
	
	return {
		"chunk": chunk_coord,
		"local": Vector3i(local_x, local_y, local_z)
	}

## Get voxel at world position
func get_voxel_at_world_position(world_position: Vector3) -> VoxelData.VoxelType:
	var coords = world_to_local_voxel(world_position)
	var chunk = loaded_chunks.get(coords.chunk)
	if chunk:
		return chunk.get_voxel(coords.local.x, coords.local.y, coords.local.z)
	return VoxelData.VoxelType.AIR

## Set voxel at world position
func set_voxel_at_world_position(world_position: Vector3, type: VoxelData.VoxelType):
	var coords = world_to_local_voxel(world_position)
	var chunk = loaded_chunks.get(coords.chunk)
	if chunk:
		chunk.set_voxel(coords.local.x, coords.local.y, coords.local.z, type)
		chunk.generate_mesh()

## Save world data
func save_world_data(save_name: String):
	var save_data = {
		"seed": world_seed,
		"chunks": {}
	}
	
	# Save all loaded chunks
	for coord in loaded_chunks.keys():
		var chunk = loaded_chunks[coord]
		save_data.chunks[str(coord)] = chunk.serialize_voxel_data()
	
	# Save to file
	var save_path = "user://saves/" + save_name + ".json"
	var dir = DirAccess.open("user://")
	if not dir.dir_exists("saves"):
		dir.make_dir("saves")
	
	var file = FileAccess.open(save_path, FileAccess.WRITE)
	if file:
		file.store_string(JSON.stringify(save_data))
		file.close()
		print("World saved to: ", save_path)
	else:
		push_error("Failed to save world data")

## Load world data
func load_world_data(save_name: String):
	var save_path = "user://saves/" + save_name + ".json"
	
	var file = FileAccess.open(save_path, FileAccess.READ)
	if not file:
		push_error("Failed to load world data")
		return
	
	var json_string = file.get_as_text()
	file.close()
	
	var json = JSON.new()
	var parse_result = json.parse(json_string)
	if parse_result != OK:
		push_error("Failed to parse save data")
		return
	
	var save_data = json.data
	
	# Load world seed
	world_seed = save_data.get("seed", world_seed)
	
	# Load chunks
	for coord_str in save_data.chunks.keys():
		var coord = str_to_var(coord_str)
		var chunk = get_or_create_chunk(coord)
		chunk.deserialize_voxel_data(save_data.chunks[coord_str])
	
	print("World loaded from: ", save_path)
