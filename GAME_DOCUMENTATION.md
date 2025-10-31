# Voxel Survival Game - Unreal Engine 5.6.1

A voxel-based survival game with RTS elements, built in Unreal Engine 5.6.1. Features procedural world generation, territory control, base building, ship construction, and full multiplayer support.

## Features

### Core Systems
- **Voxel World System**: Fully functional voxel-based world with chunk management
- **Procedural Generation**: Generic world generation that can be modified and expanded
- **Multiplayer Support**: Full network replication for multiplayer gameplay
- **Modding Support**: Data-driven design with serialization for custom content

### Gameplay Elements
- **Survival Mechanics**: Health, hunger, thirst, and stamina systems
- **RTS Elements**: Territory control with influence system
- **Base Building**: Constructible buildings that generate territory influence
- **Ship Building**: Modular ship construction system
- **Hangar System**: Ship storage and construction facilities
- **Wireframe Characters**: Basic character system ready for expansion

### Territory Control
- Grid-based territory system
- Influence spreading from buildings
- Player ownership of territories
- Building restrictions based on territory control

### Ship Building
- Modular component-based ship design
- Save and load ship designs
- Hangar construction and storage
- Ship deployment system

## Project Structure

```
VoxelSurvival/
├── Source/
│   ├── VoxelSurvival/
│   │   ├── VoxelData.h              # Voxel data structures
│   │   ├── VoxelChunk.h/cpp         # Chunk management
│   │   ├── VoxelWorld.h/cpp         # World generation
│   │   ├── WireframeCharacter.h/cpp # Player character
│   │   ├── TerritorySystem.h/cpp    # Territory control
│   │   ├── BuildingSystem.h/cpp     # Base building
│   │   ├── Ship.h/cpp               # Ship system
│   │   ├── Hangar.h/cpp             # Hangar system
│   │   └── VoxelSurvivalGameMode.h/cpp
│   ├── VoxelSurvivalTarget.cs
│   └── VoxelSurvivalEditorTarget.cs
├── Config/
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   ├── DefaultEditor.ini
│   └── DefaultInput.ini
├── Content/
│   ├── Maps/
│   ├── Blueprints/
│   ├── Materials/
│   └── Meshes/
└── VoxelSurvival.uproject
```

## Requirements

- Unreal Engine 5.6.1
- C++ compiler (Visual Studio 2022 recommended for Windows)
- 16GB RAM minimum (32GB recommended)
- GPU with DirectX 12 support

## Setup Instructions

### 1. Generate Project Files

Right-click on `VoxelSurvival.uproject` and select "Generate Visual Studio project files"

### 2. Build the Project

Open `VoxelSurvival.sln` in Visual Studio and build the solution:
- Set build configuration to "Development Editor"
- Build the solution (Ctrl+Shift+B)

### 3. Open in Unreal Editor

Double-click `VoxelSurvival.uproject` to open in Unreal Editor

### 4. Create a Map

1. Create a new level: File → New Level → Empty Level
2. Add a `VoxelWorld` actor to the level
3. Add a `TerritorySystem` actor to the level
4. Add a Player Start actor
5. Save the map as `VoxelWorld` in `Content/Maps/`

## Multiplayer Setup

### Listen Server
1. Play → Number of Players: 2+
2. Play → Net Mode: "Play As Listen Server"
3. Click Play

### Dedicated Server
Use command line:
```
UE4Editor.exe "path/to/VoxelSurvival.uproject" -server -log
```

## Modding Support

### Custom Voxel Types
Add new voxel types in `VoxelData.h`:
```cpp
enum class EVoxelType : uint8
{
    // ... existing types
    Custom1 UMETA(DisplayName = "Custom Type 1"),
    Custom2 UMETA(DisplayName = "Custom Type 2")
};
```

### World Generation
Modify world generation parameters in the editor:
- Select `VoxelWorld` actor
- Adjust `WorldSeed`, `HeightScale`, `NoiseFrequency`
- Experiment with different values for unique worlds

### Ship Components
Create ship designs programmatically:
```cpp
FShipComponent Component;
Component.ComponentName = "Engine";
Component.RelativePosition = FVector(0, 0, -100);
Ship->AddComponent(Component);
```

### Building Custom Buildings
Extend `ABuilding` class:
```cpp
UCLASS()
class AMyCustomBuilding : public ABuilding
{
    GENERATED_BODY()
    // Add custom functionality
};
```

## Gameplay Systems

### Voxel System
- **Chunks**: 16x16x16 voxels per chunk
- **Procedural Mesh**: Optimized mesh generation
- **Collision**: Full physics collision support
- **Serialization**: Save/load voxel data

### Survival System
- Health: Takes damage from starvation/dehydration
- Hunger: Decreases over time
- Thirst: Decreases faster than hunger
- Stamina: Regenerates when idle

### Territory System
- Influence-based control
- Buildings spread influence over time
- Territory updates every 5 seconds
- Build restrictions based on ownership

### Ship System
- Modular component-based design
- Serializable ship designs
- Hangar storage (up to 10 ships)
- Construction progress system

## Development Roadmap

### Phase 1: Core Systems (Current)
- [x] Voxel world generation
- [x] Basic character system
- [x] Territory control
- [x] Building system
- [x] Ship building
- [x] Multiplayer foundation

### Phase 2: Enhanced Gameplay
- [ ] Resource gathering
- [ ] Crafting system
- [ ] Combat mechanics
- [ ] AI enemies
- [ ] Advanced ship controls

### Phase 3: Polish
- [ ] UI/UX improvements
- [ ] Visual effects
- [ ] Sound design
- [ ] Performance optimization

### Phase 4: Modding Tools
- [ ] Visual ship editor
- [ ] World generation editor
- [ ] Custom content pipeline
- [ ] Mod loading system

## Configuration

### World Generation Parameters
- `WorldSeed`: Random seed for world generation
- `HeightScale`: Vertical scale of terrain
- `NoiseFrequency`: Detail level of terrain
- `RenderDistance`: Chunks to load around player

### Performance Settings
- Adjust `RenderDistance` for better performance
- Reduce `ChunkSize` for more chunks but smaller size
- Modify `VoxelSize` for visual scale

## Troubleshooting

### Build Errors
- Ensure Unreal Engine 5.6.1 is installed
- Regenerate project files
- Clean and rebuild solution

### Runtime Issues
- Check `VoxelWorld` is placed in level
- Verify `TerritorySystem` exists
- Ensure GameMode is set correctly

### Multiplayer Issues
- Check firewall settings
- Verify port forwarding (7777 default)
- Use OnlineSubsystemNull for LAN

## Contributing

This project is designed to be easily extensible. Key areas for expansion:

1. **Voxel Types**: Add new block types in `VoxelData.h`
2. **World Generation**: Modify `VoxelWorld::GenerateChunkTerrain()`
3. **Building Types**: Create new building classes extending `ABuilding`
4. **Ship Components**: Add ship component types
5. **Survival Mechanics**: Extend `WireframeCharacter` with new stats

## License

See LICENSE file for details.

## Credits

Built with Unreal Engine 5.6.1
Copyright Epic Games, Inc. All Rights Reserved.
