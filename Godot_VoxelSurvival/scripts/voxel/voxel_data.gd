extends RefCounted
class_name VoxelData

## Voxel block type enumeration
enum VoxelType {
	AIR = 0,
	STONE = 1,
	DIRT = 2,
	GRASS = 3,
	WOOD = 4,
	IRON = 5,
	GOLD = 6,
	WATER = 7,  # New: Water voxel type for water physics
	WATER_SOURCE = 8,  # New: Water source block (infinite water)
	CUSTOM = 9
}

## Voxel data structure
var type: VoxelType = VoxelType.AIR
var health: int = 100
var custom_data: int = 0
var water_level: int = 0  # New: For water physics (0-8, where 8 is full)

func _init(voxel_type: VoxelType = VoxelType.AIR):
	type = voxel_type
	health = 100
	custom_data = 0
	water_level = 0

## Check if voxel is solid (blocks movement and other voxels)
func is_solid() -> bool:
	return type != VoxelType.AIR and type != VoxelType.WATER and type != VoxelType.WATER_SOURCE

## Check if voxel is transparent (for rendering)
func is_transparent() -> bool:
	return type == VoxelType.AIR or type == VoxelType.WATER or type == VoxelType.WATER_SOURCE

## Check if voxel is water-related
func is_water() -> bool:
	return type == VoxelType.WATER or type == VoxelType.WATER_SOURCE

## Get color for voxel type
static func get_voxel_color(voxel_type: VoxelType) -> Color:
	match voxel_type:
		VoxelType.AIR:
			return Color(0, 0, 0, 0)
		VoxelType.STONE:
			return Color(0.5, 0.5, 0.5, 1.0)
		VoxelType.DIRT:
			return Color(0.4, 0.3, 0.2, 1.0)
		VoxelType.GRASS:
			return Color(0.2, 0.8, 0.2, 1.0)
		VoxelType.WOOD:
			return Color(0.4, 0.25, 0.1, 1.0)
		VoxelType.IRON:
			return Color(0.7, 0.7, 0.7, 1.0)
		VoxelType.GOLD:
			return Color(1.0, 0.84, 0.0, 1.0)
		VoxelType.WATER:
			return Color(0.2, 0.4, 0.8, 0.6)  # Semi-transparent blue
		VoxelType.WATER_SOURCE:
			return Color(0.1, 0.3, 0.9, 0.7)  # Slightly darker blue for source
		VoxelType.CUSTOM:
			return Color(1.0, 0.0, 1.0, 1.0)
		_:
			return Color(1.0, 1.0, 1.0, 1.0)

## Serialize voxel data to dictionary
func to_dict() -> Dictionary:
	return {
		"type": type,
		"health": health,
		"custom_data": custom_data,
		"water_level": water_level
	}

## Deserialize voxel data from dictionary
static func from_dict(data: Dictionary) -> VoxelData:
	var voxel = VoxelData.new(data.get("type", VoxelType.AIR))
	voxel.health = data.get("health", 100)
	voxel.custom_data = data.get("custom_data", 0)
	voxel.water_level = data.get("water_level", 0)
	return voxel
