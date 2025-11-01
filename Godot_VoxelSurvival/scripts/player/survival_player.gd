extends CharacterBody3D
class_name SurvivalPlayer

## Survival player character with movement, inventory, and survival stats

## Movement speeds
@export var walk_speed: float = 5.0
@export var sprint_speed: float = 8.0
@export var jump_velocity: float = 4.5
@export var mouse_sensitivity: float = 0.002

## Survival stats
@export var max_health: float = 100.0
@export var max_hunger: float = 100.0
@export var max_thirst: float = 100.0
@export var max_stamina: float = 100.0

var health: float = 100.0
var hunger: float = 100.0
var thirst: float = 100.0
var stamina: float = 100.0

## Components
var inventory_component: InventoryComponent
var crafting_component: CraftingComponent

## Camera
var camera: Camera3D
var head: Node3D

## Mining/placing
var raycast: RayCast3D
var max_reach: float = 5.0

## References
var voxel_world: VoxelWorld

## Get the gravity from the project settings
var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")

func _ready():
	# Create head node for camera rotation
	head = Node3D.new()
	add_child(head)
	head.position = Vector3(0, 0.8, 0)  # Eye level
	
	# Create camera
	camera = Camera3D.new()
	head.add_child(camera)
	camera.current = true
	
	# Create raycast for mining/placing
	raycast = RayCast3D.new()
	camera.add_child(raycast)
	raycast.target_position = Vector3(0, 0, -max_reach)
	raycast.enabled = true
	
	# Create inventory component
	inventory_component = InventoryComponent.new()
	add_child(inventory_component)
	
	# Create crafting component
	crafting_component = CraftingComponent.new()
	add_child(crafting_component)
	crafting_component.inventory = inventory_component
	
	# Capture mouse
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	
	# Find voxel world
	voxel_world = get_tree().get_first_node_in_group("voxel_world")
	if voxel_world:
		voxel_world.set_player(self)

func _input(event):
	# Mouse look
	if event is InputEventMouseMotion and Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
		rotate_y(-event.relative.x * mouse_sensitivity)
		head.rotate_x(-event.relative.y * mouse_sensitivity)
		head.rotation.x = clamp(head.rotation.x, -PI/2, PI/2)
	
	# Toggle mouse capture
	if event.is_action_pressed("ui_cancel"):
		if Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
			Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
		else:
			Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	
	# Mining and placing
	if Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
		if event.is_action_pressed("mine"):
			mine_voxel()
		elif event.is_action_pressed("place"):
			place_voxel()

func _physics_process(delta):
	# Add gravity
	if not is_on_floor():
		velocity.y -= gravity * delta
	
	# Jump
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = jump_velocity
		stamina -= 10
	
	# Get input direction
	var input_dir = Input.get_vector("move_left", "move_right", "move_forward", "move_backward")
	var direction = (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	
	# Sprint
	var is_sprinting = Input.is_action_pressed("sprint") and stamina > 0
	var current_speed = sprint_speed if is_sprinting else walk_speed
	
	# Move
	if direction:
		velocity.x = direction.x * current_speed
		velocity.z = direction.z * current_speed
		
		if is_sprinting:
			stamina -= 5.0 * delta
	else:
		velocity.x = move_toward(velocity.x, 0, current_speed)
		velocity.z = move_toward(velocity.z, 0, current_speed)
	
	move_and_slide()
	
	# Update survival stats
	update_survival_stats(delta)

func update_survival_stats(delta):
	# Regenerate stamina when not sprinting
	if not Input.is_action_pressed("sprint"):
		stamina = min(stamina + 10.0 * delta, max_stamina)
	
	# Decrease hunger and thirst over time
	hunger -= 1.0 * delta
	thirst -= 1.5 * delta
	
	# Clamp values
	health = clamp(health, 0, max_health)
	hunger = clamp(hunger, 0, max_hunger)
	thirst = clamp(thirst, 0, max_thirst)
	stamina = clamp(stamina, 0, max_stamina)
	
	# Take damage if hungry or thirsty
	if hunger <= 0 or thirst <= 0:
		health -= 5.0 * delta

func mine_voxel():
	if not voxel_world:
		return
	
	raycast.force_raycast_update()
	if raycast.is_colliding():
		var collision_point = raycast.get_collision_point()
		var normal = raycast.get_collision_normal()
		
		# Get voxel position (slightly inside the hit surface)
		var voxel_pos = collision_point - normal * 0.1
		
		var voxel_type = voxel_world.get_voxel_at_world_position(voxel_pos)
		if voxel_type != VoxelData.VoxelType.AIR:
			# Remove voxel
			voxel_world.set_voxel_at_world_position(voxel_pos, VoxelData.VoxelType.AIR)
			
			# Add to inventory
			if inventory_component:
				inventory_component.add_item(voxel_type, 1)

func place_voxel():
	if not voxel_world:
		return
	
	raycast.force_raycast_update()
	if raycast.is_colliding():
		var collision_point = raycast.get_collision_point()
		var normal = raycast.get_collision_normal()
		
		# Get voxel position (slightly outside the hit surface)
		var voxel_pos = collision_point + normal * 0.1
		
		# Check if position is valid (not inside player)
		if global_position.distance_to(voxel_pos) < 1.0:
			return
		
		var voxel_type = voxel_world.get_voxel_at_world_position(voxel_pos)
		if voxel_type == VoxelData.VoxelType.AIR:
			# Get selected item from inventory
			var selected_type = VoxelData.VoxelType.STONE  # Default
			if inventory_component and inventory_component.selected_item_type != null:
				selected_type = inventory_component.selected_item_type
			
			# Place voxel
			voxel_world.set_voxel_at_world_position(voxel_pos, selected_type)
			
			# Remove from inventory
			if inventory_component:
				inventory_component.remove_item(selected_type, 1)

## Take damage
func take_damage(amount: float):
	health -= amount
	if health <= 0:
		die()

## Heal
func heal(amount: float):
	health = min(health + amount, max_health)

## Die
func die():
	print("Player died!")
	# Respawn logic would go here
	health = max_health
	hunger = max_hunger
	thirst = max_thirst
	stamina = max_stamina
