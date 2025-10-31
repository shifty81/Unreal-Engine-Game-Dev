# Maps

This directory contains the game levels/maps.

## Recommended Maps

### VoxelWorld
The main gameplay map. Should include:
- VoxelWorld actor (for chunk generation)
- TerritorySystem actor (for territory control)
- Player Start locations
- Light source (Directional Light)
- Post Process Volume
- Sky Atmosphere (optional)

### TestMap
Testing environment with:
- Smaller render distance for quick iteration
- Pre-built structures for testing
- Multiple player starts for multiplayer testing

## Creating the Main Map

1. **Create New Level**
   - File → New Level → Empty Level
   - Save as "VoxelWorld" in Content/Maps/

2. **Add Essential Actors**
   ```
   - Place Modes → Basic → Player Start
   - Place Modes → Lights → Directional Light
   - Place Modes → Visual Effects → Post Process Volume
   ```

3. **Add Game Actors**
   - Find VoxelWorld class in Place Actors
   - Drag into level
   - Configure properties in Details panel
   
   - Find TerritorySystem class
   - Drag into level

4. **Configure Lighting**
   - Set Directional Light to Movable
   - Adjust light angle for desired time of day
   - Enable atmospheric fog (optional)

5. **Set as Default Map**
   - Project Settings → Maps & Modes
   - Set Game Default Map to "VoxelWorld"
   - Set Editor Startup Map to "VoxelWorld"

## Map Configuration

### VoxelWorld Actor Settings
- Render Distance: 8 (for testing, increase for release)
- World Seed: 12345 (change for different worlds)
- Height Scale: 10.0
- Noise Frequency: 0.01

### Performance Optimization
- Use occlusion culling
- Enable distance culling
- Set appropriate LOD levels
- Use light baking for static objects

## Multiplayer Testing

To test multiplayer in editor:
1. Play → Number of Players: 2 (or more)
2. Play → Net Mode: "Play As Listen Server"
3. Window → Developer Tools → Network Emulation
