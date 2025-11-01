# Voxel Survival - Godot Edition

This is a Godot 4.3 port of the Voxel Survival game originally developed in Unreal Engine 5.6.1. This version allows parallel development and testing while debugging .NET 8.0 compatibility issues in the Unreal version.

## Features

### Core Voxel System
- **VoxelData**: Voxel type enumeration with support for AIR, STONE, DIRT, GRASS, WOOD, IRON, GOLD, WATER, and WATER_SOURCE
- **VoxelChunk**: Chunk-based world (16³ voxels per chunk) with procedural mesh generation
- **VoxelWorld**: Chunk loading/unloading, Perlin noise terrain generation, save/load system

### Water Physics (NEW!)
- **Minecraft-Style Water Flow**: Water flows down first, then spreads horizontally
- **Water Levels**: 0-8 levels, where 8 is a full source block
- **Source Blocks**: Infinite water sources (WATER_SOURCE type)
- **Flowing Water**: Decreases in level as it spreads (WATER type)
- **Semi-Transparent Rendering**: Water rendered with alpha transparency
- **Automatic Updates**: Water physics updates every 0.1 seconds

### Player Systems
- **SurvivalPlayer**: First-person character controller with mouse look
- **Movement**: WASD movement, sprint, jump with stamina system
- **Mining & Placing**: Left-click to mine, right-click to place voxels
- **Survival Stats**: Health, hunger, thirst, and stamina tracking
- **Camera System**: First-person camera with smooth mouse look

### Inventory & Crafting
- **InventoryComponent**: 40-slot inventory system
- **CraftingComponent**: Recipe-based crafting system
- **Item Management**: Add, remove, and track voxel items

### Building System
- **BuildingSystem**: Place and manage buildings (foundations, walls, hangars, forges)
- **TerritorySystem**: Grid-based territory control with influence spreading
- **Building Types**: Foundation, Wall, Floor, Roof, Door, Window, Hangar, Forge, Salvage Station

### Resource System
- **ResourceNode**: Harvestable resource nodes (Wood, Stone, Iron, Gold, Water)
- **ResourceManager**: Spawns and manages resource nodes
- **Respawning**: Resources respawn after being depleted

## Prerequisites

- **Godot Engine 4.3** or later
- Compatible with Windows, Linux, and macOS

## Quick Start

### 1. Install Godot

Download and install Godot 4.3 from: https://godotengine.org/download

### 2. Open the Project

1. Launch Godot Engine
2. Click "Import"
3. Navigate to the `Godot_VoxelSurvival` folder
4. Select `project.godot`
5. Click "Import & Edit"

### 3. Run the Game

1. Press F5 or click the Play button in the top-right
2. The game will start with the main scene
3. First launch may take a moment to compile shaders

### 4. Controls

- **WASD**: Move forward/backward/left/right
- **Space**: Jump
- **Shift**: Sprint (uses stamina)
- **Mouse**: Look around
- **Left Click**: Mine/destroy voxels
- **Right Click**: Place voxels
- **I**: Open inventory (not yet implemented in UI)
- **C**: Open crafting menu (not yet implemented in UI)
- **ESC**: Toggle mouse capture

## Project Structure

```
Godot_VoxelSurvival/
├── project.godot          # Main project configuration
├── scenes/
│   └── main.tscn         # Main game scene
├── scripts/
│   ├── voxel/            # Core voxel system
│   │   ├── voxel_data.gd
│   │   ├── voxel_chunk.gd
│   │   └── voxel_world.gd
│   ├── player/           # Player controller
│   │   └── survival_player.gd
│   ├── survival/         # Inventory & crafting
│   │   ├── inventory_component.gd
│   │   └── crafting_component.gd
│   ├── building/         # Building & territory
│   │   ├── building_system.gd
│   │   └── territory_system.gd
│   └── resources/        # Resource management
│       ├── resource_node.gd
│       └── resource_manager.gd
└── assets/               # Textures and materials (future)
```

## Water Physics Details

### How It Works

The water system uses a Minecraft-inspired flow algorithm:

1. **Vertical Flow (Priority)**: Water always flows down first if there's space below
2. **Horizontal Spread**: If water can't flow down, it spreads horizontally to adjacent blocks
3. **Level Reduction**: Each horizontal spread reduces water level by 1 (max 8 levels)
4. **Source Blocks**: Water source blocks maintain level 8 and provide infinite water

### Water Types

- **WATER_SOURCE (Type 8)**: Infinite water source, never depletes
- **WATER (Type 7)**: Flowing water that spreads and reduces in level

### Usage

To place water in the game:
1. Add water source blocks to your inventory
2. Place them with right-click
3. Watch the water automatically flow and spread

To create water pools:
- Place source blocks at desired locations
- Water will fill downward and spread horizontally

## Differences from Unreal Version

### Advantages of Godot Version
✅ Faster iteration and testing
✅ No .NET 8.0 compatibility issues
✅ Smaller project size and faster load times
✅ Cross-platform development without Visual Studio
✅ Built-in GDScript is easier to debug

### Current Limitations
⚠️ Graphics are more basic (no advanced materials yet)
⚠️ No UI implemented yet (inventory, crafting menus)
⚠️ Multiplayer not implemented (Unreal uses dedicated system)
⚠️ No save/load UI (functions exist but no menu)

### Feature Parity

| Feature | Unreal | Godot | Notes |
|---------|--------|-------|-------|
| Voxel System | ✅ | ✅ | Full parity |
| Water Physics | ❌ | ✅ | **New in Godot!** |
| Player Movement | ✅ | ✅ | Full parity |
| Mining/Placing | ✅ | ✅ | Full parity |
| Inventory | ✅ | ✅ | Backend only (no UI) |
| Crafting | ✅ | ✅ | Backend only (no UI) |
| Building System | ✅ | ✅ | Simplified visuals |
| Territory System | ✅ | ✅ | Full parity |
| Multiplayer | ✅ | ❌ | Not yet implemented |
| Save/Load | ✅ | ✅ | No UI yet |

## Development Roadmap

### Phase 1: Core Systems (COMPLETE) ✅
- [x] Voxel system with chunk management
- [x] Terrain generation with Perlin noise
- [x] Player movement and camera
- [x] Mining and placing voxels
- [x] Water physics implementation
- [x] Inventory and crafting backend
- [x] Building and territory systems

### Phase 2: UI & Polish (IN PROGRESS)
- [ ] Inventory UI
- [ ] Crafting menu UI
- [ ] Health/hunger/thirst bars
- [ ] Hotbar for quick item access
- [ ] Building placement preview
- [ ] Territory visualization

### Phase 3: Advanced Features
- [ ] Advanced water rendering (waves, foam)
- [ ] Improved voxel textures
- [ ] Particle effects
- [ ] Sound effects and music
- [ ] Save/load menu
- [ ] Settings menu

### Phase 4: Multiplayer
- [ ] Client-server architecture
- [ ] Player synchronization
- [ ] Voxel replication
- [ ] Building and territory sync

## Testing Water Physics

To test the new water physics:

1. Start the game (F5)
2. Look down and mine some terrain to create a hole
3. Switch to water source block in inventory (you'll need to add via code for now)
4. Place water source blocks
5. Watch water flow down and spread
6. Experiment with different terrain shapes

## Performance Tips

- Reduce `render_distance` in VoxelWorld if FPS is low (default: 8 chunks)
- Water physics updates can be optimized by increasing `water_update_interval`
- Use chunk_size of 16 for best balance of performance and detail

## Troubleshooting

### Water not flowing
- Check that you placed a WATER_SOURCE block, not just WATER
- Water updates every 0.1 seconds, so there may be a slight delay
- Make sure there's space for water to flow to

### Game runs slowly
- Reduce render_distance in the VoxelWorld node
- Close other applications
- Check Godot debug panel for performance metrics

### Controls not working
- Make sure mouse is captured (ESC to toggle)
- Check input mappings in project.godot
- Verify you're running the main.tscn scene

## Contributing

This is a learning project demonstrating Unreal-to-Godot porting. Contributions are welcome!

Areas for contribution:
- UI implementation (inventory, crafting, HUD)
- Water rendering improvements
- Multiplayer networking
- Performance optimizations
- Additional voxel types and mechanics

## License

Same license as the main Unreal Engine project. See parent directory LICENSE file.

## Credits

- Original Unreal Engine version by shifty81
- Godot port includes new water physics system
- Built with Godot Engine 4.3

---

For the original Unreal Engine version, see the parent directory's README.md
