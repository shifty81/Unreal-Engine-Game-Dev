# Water Physics System Documentation

## Overview

The Voxel Survival game now includes a Minecraft-style water physics system that simulates realistic water flow in a block-based world. This system is implemented in both the Unreal Engine and Godot versions.

## Water Voxel Types

### Water Source (`EVoxelType::WaterSource` / `VoxelType.WATER_SOURCE`)
- Infinite water source that never depletes
- Always maintains a water level of 8 (full)
- Creates flowing water in adjacent blocks
- Ideal for creating lakes, rivers, and water features

### Flowing Water (`EVoxelType::Water` / `VoxelType.WATER`)
- Water that flows and spreads from source blocks
- Water level ranges from 1-7
- Decreases in level as it spreads horizontally
- Eventually disappears if not connected to a source

## Water Flow Mechanics

### Priority System
Water follows a priority-based flow system:

1. **Vertical Flow (Highest Priority)**
   - Water always tries to flow downward first
   - If there's an air block below, water fills it completely
   - Vertical flow always creates level 8 water

2. **Horizontal Spread (Secondary Priority)**
   - If water can't flow down, it spreads horizontally
   - Spreads to all 4 adjacent blocks (North, South, East, West)
   - Each horizontal spread reduces water level by 1
   - Only spreads if current water level is > 1

### Water Level System

Water blocks have levels from 0-8:
- **Level 8**: Full water block (source or freshly flowed down)
- **Level 7-2**: Progressively lower water levels
- **Level 1**: Minimal water (won't spread further)
- **Level 0**: Air (water removed)

### Flow Algorithm

```
For each water block:
    1. Check block directly below
       If below is air or empty:
           Create water (level 8) below
           If source block: maintain level
           Else: reduce level by 1
    
    2. Else if water level > 1:
           Check 4 horizontal neighbors
           For each neighbor that is air or lower water:
               Create water (level = current - 1)
```

## Implementation Details

### Unreal Engine Implementation

**Files Modified:**
- `Source/VoxelSurvival/VoxelData.h`
  - Added `EVoxelType::Water` and `EVoxelType::WaterSource`
  - Added `WaterLevel` field to `FVoxelData` struct
  - Added `IsTransparent()` and `IsWater()` helper methods

- `Source/VoxelSurvival/VoxelChunk.h`
  - Added `UpdateWaterPhysics()` method
  - Added `GetVoxelData()` helper method
  - Added water update timer system
  - Enabled tick for water updates

- `Source/VoxelSurvival/VoxelChunk.cpp`
  - Implemented water physics update loop
  - Added water color rendering (semi-transparent blue)
  - Updated mesh generation to handle transparent blocks
  - Water updates every 0.1 seconds

**Key Features:**
- Timer-based updates (configurable interval)
- Batched water changes for performance
- Automatic mesh regeneration after water updates
- Semi-transparent rendering for water blocks

### Godot Implementation

**Files Created:**
- `Godot_VoxelSurvival/scripts/voxel/voxel_data.gd`
  - `VoxelType` enum with `WATER` and `WATER_SOURCE`
  - `water_level` property
  - `is_water()` helper method

- `Godot_VoxelSurvival/scripts/voxel/voxel_chunk.gd`
  - `update_water_physics()` method
  - `_process()` loop for water updates
  - Procedural mesh generation with SurfaceTool

- `Godot_VoxelSurvival/scripts/voxel/voxel_world.gd`
  - Chunk management with water support
  - Terrain generation with water level

**Key Features:**
- GDScript-based implementation
- Delta-time based updates
- Color-coded water rendering
- Automatic chunk mesh updates

## Usage Examples

### Creating a Water Source

**Unreal (C++):**
```cpp
VoxelWorld->SetVoxelAtWorldPosition(Position, EVoxelType::WaterSource);
```

**Godot (GDScript):**
```gdscript
voxel_world.set_voxel_at_world_position(position, VoxelData.VoxelType.WATER_SOURCE)
```

### Creating a Pool

Place multiple water source blocks in a depression. Water will:
1. Fill downward to the bottom
2. Spread horizontally across the bottom
3. Continue filling upward until reaching source blocks

### Creating a Waterfall

1. Place water source blocks at the top of a cliff
2. Water will flow down the cliff face
3. Collect in a pool at the bottom
4. Spread outward from the pool

### Creating a River

1. Place water source blocks at the high point
2. Create a channel downhill
3. Water will flow along the channel
4. Add source blocks periodically to maintain flow

## Performance Considerations

### Update Frequency
- Default: Updates every 0.1 seconds
- Can be adjusted via `WaterUpdateInterval` property
- Lower values = faster flow, higher CPU usage
- Higher values = slower flow, better performance

### Chunk-Based Updates
- Water physics only update in loaded chunks
- Chunks outside render distance don't update
- Reduces CPU load for large worlds

### Optimization Tips

1. **Limit Water Sources**
   - Each source creates continuous flow calculations
   - Use sparingly for best performance

2. **Adjust Update Interval**
   - Increase interval for better FPS
   - Decrease for more responsive water

3. **Contain Water**
   - Build containers/pools to limit spread
   - Prevents infinite spreading

## Visual Rendering

### Unreal Engine
- Water uses semi-transparent blue color
- Alpha value: 153/255 (~60% opacity)
- Water source: Slightly darker blue (alpha 179/255)
- Vertex coloring for performance

### Godot
- Water uses Color(0.2, 0.4, 0.8, 0.6)
- Source: Color(0.1, 0.3, 0.9, 0.7)
- SurfaceTool-based mesh generation
- Per-vertex colors

## Known Limitations

1. **No Cross-Chunk Flow** (Current Version)
   - Water doesn't flow between chunks yet
   - Will be added in future update

2. **No Diagonal Flow**
   - Water only flows in 4 cardinal directions
   - No diagonal spreading

3. **Simple Transparency**
   - Basic alpha blending
   - No advanced water effects (waves, reflections)

4. **Performance at Scale**
   - Many water blocks can impact FPS
   - Optimize by reducing update frequency

## Future Enhancements

### Planned Features
- [ ] Cross-chunk water flow
- [ ] Water pressure system
- [ ] Flowing water particles
- [ ] Water damage to structures
- [ ] Swimming mechanics
- [ ] Buoyancy for objects
- [ ] Advanced water shaders
- [ ] Sound effects (flowing, splashing)

### Advanced Water Physics
- [ ] Current/flow direction
- [ ] Wave simulation
- [ ] Whirlpools and eddies
- [ ] Ice freezing mechanics
- [ ] Steam generation from heat

## Testing Water Physics

### Test Scenario 1: Basic Pool
1. Dig a 5x5x3 hole in terrain
2. Place single water source at center
3. Watch water spread to fill hole
4. Result: Level pool of water

### Test Scenario 2: Waterfall
1. Create tall pillar of blocks
2. Place water source on top
3. Remove blocks below source
4. Result: Water falls and pools at bottom

### Test Scenario 3: River
1. Create sloped channel
2. Place water sources at top
3. Result: Water flows downhill

### Test Scenario 4: Water Level
1. Place water source
2. Let it spread 4 blocks horizontally
3. Observe water level decreasing
4. Result: Level 8 → 7 → 6 → 5 → 4 (stops at level 1)

## Debugging

### Common Issues

**Water not flowing:**
- Verify block is `WaterSource` not `Water`
- Check update interval isn't too high
- Ensure chunk is loaded and ticking

**Water disappearing:**
- Flowing water naturally depletes
- Add more source blocks
- Check water level values

**Performance issues:**
- Reduce number of water sources
- Increase update interval
- Limit water spread area

### Debug Commands

**Unreal Blueprint:**
```
Get Voxel World -> Get Voxel At World Position -> Print
```

**Godot:**
```gdscript
print(voxel_world.get_voxel_at_world_position(position))
```

## Conclusion

The water physics system provides realistic, Minecraft-style water flow for your voxel world. It's designed to be performant, extensible, and easy to use. Experiment with different configurations to achieve the desired water behavior for your game!

## Credits

- Inspired by Minecraft's water mechanics
- Implemented for both Unreal Engine 5.6.1 and Godot 4.3
- Part of the Voxel Survival project
