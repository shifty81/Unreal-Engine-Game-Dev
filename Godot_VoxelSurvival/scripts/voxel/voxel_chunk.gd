extends Node3D
class_name VoxelChunk

## Represents a chunk of voxels in the world
## Chunks are the basic unit of voxel management and rendering

## Size of chunk in voxels per dimension
@export var chunk_size: int = 16

## Size of each voxel in world units
@export var voxel_size: float = 1.0

## Chunk coordinates in world grid
var chunk_coordinate: Vector3i = Vector3i.ZERO

## Voxel data array
var voxel_data: Array = []

## Mesh instance for rendering
var mesh_instance: MeshInstance3D

## Collision shape for physics
var collision_shape: CollisionShape3D
var static_body: StaticBody3D

## Water update timer for physics
var water_update_timer: float = 0.0
var water_update_interval: float = 0.1  # Update water every 0.1 seconds

func _ready():
	# Create mesh instance
	mesh_instance = MeshInstance3D.new()
	add_child(mesh_instance)
	
	# Create collision body
	static_body = StaticBody3D.new()
	add_child(static_body)
	
	collision_shape = CollisionShape3D.new()
	static_body.add_child(collision_shape)
	
	# Initialize voxel data
	voxel_data.resize(chunk_size * chunk_size * chunk_size)
	for i in range(voxel_data.size()):
		voxel_data[i] = VoxelData.new()

func _process(delta: float):
	# Update water physics
	water_update_timer += delta
	if water_update_timer >= water_update_interval:
		water_update_timer = 0.0
		update_water_physics()

## Initialize chunk with coordinates
func initialize_chunk(coordinate: Vector3i):
	chunk_coordinate = coordinate
	position = Vector3(coordinate.x * chunk_size * voxel_size, 
					   coordinate.y * chunk_size * voxel_size, 
					   coordinate.z * chunk_size * voxel_size)

## Get voxel index from coordinates
func get_voxel_index(x: int, y: int, z: int) -> int:
	return x + y * chunk_size + z * chunk_size * chunk_size

## Check if coordinates are valid
func is_valid_voxel_coordinate(x: int, y: int, z: int) -> bool:
	return x >= 0 and x < chunk_size and y >= 0 and y < chunk_size and z >= 0 and z < chunk_size

## Set voxel at local position
func set_voxel(x: int, y: int, z: int, type: VoxelData.VoxelType):
	if not is_valid_voxel_coordinate(x, y, z):
		return
	
	var index = get_voxel_index(x, y, z)
	voxel_data[index].type = type
	
	# If setting water source, set water level to max
	if type == VoxelData.VoxelType.WATER_SOURCE:
		voxel_data[index].water_level = 8
	elif type == VoxelData.VoxelType.WATER:
		voxel_data[index].water_level = 7

## Get voxel at local position
func get_voxel(x: int, y: int, z: int) -> VoxelData.VoxelType:
	if not is_valid_voxel_coordinate(x, y, z):
		return VoxelData.VoxelType.AIR
	
	var index = get_voxel_index(x, y, z)
	return voxel_data[index].type

## Get voxel data at local position
func get_voxel_data(x: int, y: int, z: int) -> VoxelData:
	if not is_valid_voxel_coordinate(x, y, z):
		return null
	
	var index = get_voxel_index(x, y, z)
	return voxel_data[index]

## Update water physics (Minecraft-style)
func update_water_physics():
	var water_changes = []
	
	# Scan for water blocks
	for x in range(chunk_size):
		for y in range(chunk_size):
			for z in range(chunk_size):
				var voxel = get_voxel_data(x, y, z)
				if voxel and voxel.is_water():
					# Water flows down first
					var below = get_voxel_data(x, y - 1, z)
					if below and not below.is_solid() and not below.is_water():
						water_changes.append({"x": x, "y": y - 1, "z": z, "type": VoxelData.VoxelType.WATER, "level": 8})
						if voxel.type != VoxelData.VoxelType.WATER_SOURCE:
							voxel.water_level -= 1
							if voxel.water_level <= 0:
								water_changes.append({"x": x, "y": y, "z": z, "type": VoxelData.VoxelType.AIR, "level": 0})
					
					# Water spreads horizontally if can't flow down
					elif voxel.water_level > 1:
						var spread_level = voxel.water_level - 1
						for dir in [Vector3i(1, 0, 0), Vector3i(-1, 0, 0), Vector3i(0, 0, 1), Vector3i(0, 0, -1)]:
							var nx = x + dir.x
							var nz = z + dir.z
							var neighbor = get_voxel_data(nx, y, nz)
							if neighbor and not neighbor.is_solid():
								if not neighbor.is_water() or neighbor.water_level < spread_level:
									water_changes.append({"x": nx, "y": y, "z": nz, "type": VoxelData.VoxelType.WATER, "level": spread_level})
	
	# Apply water changes
	for change in water_changes:
		if is_valid_voxel_coordinate(change.x, change.y, change.z):
			var index = get_voxel_index(change.x, change.y, change.z)
			voxel_data[index].type = change.type
			voxel_data[index].water_level = change.level
	
	# Regenerate mesh if water changed
	if water_changes.size() > 0:
		generate_mesh()

## Generate the chunk mesh from voxel data
func generate_mesh():
	var surface_tool = SurfaceTool.new()
	surface_tool.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	# Generate mesh for each voxel
	for x in range(chunk_size):
		for y in range(chunk_size):
			for z in range(chunk_size):
				var voxel = get_voxel_data(x, y, z)
				if voxel and voxel.type != VoxelData.VoxelType.AIR:
					add_voxel_faces(surface_tool, x, y, z, voxel)
	
	# Create mesh
	var array_mesh = surface_tool.commit()
	mesh_instance.mesh = array_mesh
	
	# Create collision shape
	if array_mesh:
		var shape = array_mesh.create_trimesh_shape()
		collision_shape.shape = shape

## Add faces for a voxel
func add_voxel_faces(surface_tool: SurfaceTool, x: int, y: int, z: int, voxel: VoxelData):
	var pos = Vector3(x, y, z) * voxel_size
	var color = VoxelData.get_voxel_color(voxel.type)
	
	# Check each face and only render if neighbor is transparent
	var faces = [
		{"normal": Vector3.UP, "check": Vector3i(x, y + 1, z)},
		{"normal": Vector3.DOWN, "check": Vector3i(x, y - 1, z)},
		{"normal": Vector3.RIGHT, "check": Vector3i(x + 1, y, z)},
		{"normal": Vector3.LEFT, "check": Vector3i(x - 1, y, z)},
		{"normal": Vector3.FORWARD, "check": Vector3i(x, y, z + 1)},
		{"normal": Vector3.BACK, "check": Vector3i(x, y, z - 1)}
	]
	
	for face in faces:
		var check_pos = face.check
		var neighbor = get_voxel_data(check_pos.x, check_pos.y, check_pos.z)
		
		# Render face if neighbor is air or transparent
		var should_render = false
		if neighbor == null or neighbor.is_transparent():
			should_render = true
		# Don't render water faces between water blocks of same level
		elif voxel.is_water() and neighbor.is_water() and voxel.water_level == neighbor.water_level:
			should_render = false
		else:
			should_render = not neighbor.is_solid()
		
		if should_render:
			add_face(surface_tool, pos, face.normal, color, voxel_size)

## Add a single face to the mesh
func add_face(surface_tool: SurfaceTool, pos: Vector3, normal: Vector3, color: Color, size: float):
	var vertices = []
	var uvs = [Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(0, 1)]
	
	# Define vertices based on normal
	if normal == Vector3.UP:
		vertices = [
			pos + Vector3(0, size, 0),
			pos + Vector3(size, size, 0),
			pos + Vector3(size, size, size),
			pos + Vector3(0, size, size)
		]
	elif normal == Vector3.DOWN:
		vertices = [
			pos + Vector3(0, 0, size),
			pos + Vector3(size, 0, size),
			pos + Vector3(size, 0, 0),
			pos + Vector3(0, 0, 0)
		]
	elif normal == Vector3.RIGHT:
		vertices = [
			pos + Vector3(size, 0, 0),
			pos + Vector3(size, 0, size),
			pos + Vector3(size, size, size),
			pos + Vector3(size, size, 0)
		]
	elif normal == Vector3.LEFT:
		vertices = [
			pos + Vector3(0, 0, size),
			pos + Vector3(0, 0, 0),
			pos + Vector3(0, size, 0),
			pos + Vector3(0, size, size)
		]
	elif normal == Vector3.FORWARD:
		vertices = [
			pos + Vector3(size, 0, size),
			pos + Vector3(0, 0, size),
			pos + Vector3(0, size, size),
			pos + Vector3(size, size, size)
		]
	elif normal == Vector3.BACK:
		vertices = [
			pos + Vector3(0, 0, 0),
			pos + Vector3(size, 0, 0),
			pos + Vector3(size, size, 0),
			pos + Vector3(0, size, 0)
		]
	
	# Add two triangles for the face
	var indices = [0, 1, 2, 0, 2, 3]
	for i in indices:
		surface_tool.set_normal(normal)
		surface_tool.set_color(color)
		surface_tool.set_uv(uvs[i])
		surface_tool.add_vertex(vertices[i])

## Serialize voxel data
func serialize_voxel_data() -> Array:
	var serialized = []
	for voxel in voxel_data:
		serialized.append(voxel.to_dict())
	return serialized

## Deserialize voxel data
func deserialize_voxel_data(data: Array):
	if data.size() != voxel_data.size():
		push_error("Invalid voxel data size")
		return
	
	for i in range(data.size()):
		voxel_data[i] = VoxelData.from_dict(data[i])
	
	generate_mesh()
