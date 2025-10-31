# Materials

This directory contains material assets for the game.

## Recommended Materials

### Voxel Materials
- `M_Stone` - Stone voxel material
- `M_Dirt` - Dirt voxel material  
- `M_Grass` - Grass voxel material
- `M_Wood` - Wood voxel material
- `M_Iron` - Iron voxel material
- `M_Gold` - Gold voxel material

### Character Materials
- `M_Wireframe` - Wireframe character material
- `M_PlayerIndicator` - Player team color material

### Building Materials
- `M_Building_Base` - Base building material
- `M_Building_Damaged` - Damaged state material
- `M_Hangar` - Hangar-specific material

### Ship Materials
- `M_Ship_Hull` - Ship hull material
- `M_Ship_Engine` - Engine glow material
- `M_Ship_Weapon` - Weapon material

### UI Materials
- `M_TerritoryOverlay` - Territory visualization
- `M_InfluenceRadius` - Influence area indicator

## Creating Materials

1. Content Browser → Right Click → Material
2. Name the material (e.g., M_Stone)
3. Double-click to open Material Editor
4. Add nodes for desired effect
5. Connect to output nodes
6. Save and apply

## Material Features

### Voxel Materials
- Simple color-based for performance
- Optional normal maps for detail
- Support for vertex colors from chunk generation

### Wireframe Material
```
Material Expression:
- Set Material Domain to Surface
- Set Blend Mode to Opaque
- Set Shading Model to Unlit
- In Pixel Shader:
  - Add Fresnel node
  - Multiply with color
  - Connect to Emissive Color
```

### Territory Overlay
- Translucent blend mode
- Color parameter for team colors
- Pulsing animation for active territories
