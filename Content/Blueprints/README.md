# Blueprint Structure

This directory contains the game's Blueprint assets that extend the C++ classes.

## Recommended Blueprints to Create

### Characters
- `BP_WireframeCharacter` - Extends AWireframeCharacter
  - Add visual mesh or wireframe material
  - Configure input mappings
  - Add UI widgets for survival stats

### World
- `BP_VoxelWorld` - Extends AVoxelWorld
  - Configure world generation parameters
  - Set chunk size and render distance
  - Customize voxel materials

- `BP_VoxelChunk` - Extends AVoxelChunk
  - Set voxel size and materials
  - Configure collision settings

### Territory
- `BP_TerritorySystem` - Extends ATerritorySystem
  - Configure territory grid size
  - Set influence parameters
  - Add visual territory indicators

### Buildings
- `BP_BaseBuilding` - Extends ABuilding
  - Add building mesh
  - Configure health and influence
  - Add construction effects

- `BP_Hangar` - Extends AHangar
  - Add hangar mesh and interior
  - Configure ship construction UI
  - Set storage capacity

### Ships
- `BP_Ship` - Extends AShip
  - Add ship mesh components
  - Configure movement
  - Add ship controls and UI

### GameMode
- `BP_VoxelSurvivalGameMode` - Extends AVoxelSurvivalGameMode
  - Set default character class
  - Configure multiplayer settings
  - Set win/loss conditions

## Creating Blueprints in Editor

1. Open Unreal Editor
2. Content Browser → Right Click → Blueprint Class
3. Select parent class (e.g., AWireframeCharacter)
4. Name the blueprint (e.g., BP_WireframeCharacter)
5. Double-click to edit
6. Add components and configure in Details panel
7. Implement logic in Event Graph
8. Compile and save

## Best Practices

- Use descriptive names with BP_ prefix
- Keep blueprint logic simple (complex logic in C++)
- Use data tables for configuration
- Document blueprint nodes with comments
- Test in multiplayer mode

## Example Setups

### Wireframe Character Mesh
1. Create material: M_Wireframe
2. Set material to wireframe rendering mode
3. Apply to skeletal mesh in BP_WireframeCharacter

### Territory Visualization
1. Create material: M_TerritoryOverlay
2. Add decal components to BP_TerritorySystem
3. Update colors based on owner

### Ship Components
1. Create static meshes for ship parts
2. Add to BP_Ship as static mesh components
3. Configure attachment points
