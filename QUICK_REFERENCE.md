# Quick Reference - Voxel Survival Game

## 🚀 First Time Setup

**New to this project? Start here:**
- 📚 [SETUP_GUIDE.md](SETUP_GUIDE.md) - **Complete visual setup guide with diagrams**
- 🛠️ [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common issues and solutions

---

## ⚠️ Critical Build Order

```
┌─────────────────────────────────────────────────┐
│  CORRECT BUILD ORDER (MUST FOLLOW!)             │
├─────────────────────────────────────────────────┤
│  1. Clone Repository                            │
│  2. Generate Project Files (right-click)        │
│  3. Build in Visual Studio (Ctrl+Shift+B)       │
│  4. Open in Unreal Engine (.uproject)           │
└─────────────────────────────────────────────────┘

❌ WRONG: Open .uproject first → Error about missing modules
✅ RIGHT: Generate → Build in VS → Then open .uproject
```

**Most Common Error:** "modules are missing or built with a different engine version"
- **Cause:** You opened `.uproject` before building in Visual Studio
- **Fix:** See [SETUP_GUIDE.md](SETUP_GUIDE.md#issue-1-missing-or-built-with-different-engine-version-)

---

## Building the Project

### Windows
```bash
# Generate project files
Right-click VoxelSurvival.uproject → "Generate Visual Studio project files"

# Build
Open VoxelSurvival.sln in Visual Studio
Build → Build Solution (Ctrl+Shift+B)
Configuration: Development Editor
```

### Linux
```bash
# Generate project files
/path/to/UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh -project="/path/to/VoxelSurvival.uproject" -game -engine

# Build
make VoxelSurvivalEditor
```

## Key Classes

### Voxel System
- `FVoxelData` - Single voxel data structure
- `AVoxelChunk` - 16³ chunk of voxels
- `AVoxelWorld` - World manager, chunk loading

### Gameplay
- `AWireframeCharacter` - Player character with survival stats
- `ABuilding` - Base building class
- `AHangar` - Ship construction building
- `AShip` - Modular ship class

### RTS
- `ATerritorySystem` - Territory control manager
- Influence spreading from buildings
- Grid-based ownership

## Common Tasks

### Add New Voxel Type
1. Edit `Source/VoxelSurvival/VoxelData.h`
2. Add to `EVoxelType` enum
3. Add color case in `VoxelChunk.cpp` → `AddVoxelFace()`
4. Recompile

### Create New Building Type
```cpp
UCLASS()
class AMyBuilding : public ABuilding
{
    GENERATED_BODY()
    // Custom properties and methods
};
```

### Add Ship Component
```cpp
FShipComponent Component;
Component.ComponentName = "NewComponent";
Component.RelativePosition = FVector(X, Y, Z);
Ship->AddComponent(Component);
```

### Modify World Generation
Edit `VoxelWorld.cpp` → `GenerateChunkTerrain()`
- Adjust `HeightScale`, `NoiseFrequency`
- Modify terrain logic

## Multiplayer Commands

### In Editor
- Play → Number of Players: 2+
- Play → Net Mode: "Play As Listen Server"

### Command Line
```bash
# Dedicated Server
UE4Editor.exe "C:/Path/VoxelSurvival.uproject" -server -log

# Client
UE4Editor.exe "C:/Path/VoxelSurvival.uproject" 127.0.0.1 -game
```

## Configuration Files

- `Config/DefaultEngine.ini` - Engine settings
- `Config/DefaultGame.ini` - Game settings  
- `Config/DefaultEditor.ini` - Editor settings
- `Config/DefaultInput.ini` - Input bindings

## Blueprint Classes

Create in Editor from these C++ classes:
- `BP_WireframeCharacter` from `AWireframeCharacter`
- `BP_VoxelWorld` from `AVoxelWorld`
- `BP_TerritorySystem` from `ATerritorySystem`
- `BP_Building` from `ABuilding`
- `BP_Hangar` from `AHangar`
- `BP_Ship` from `AShip`

## Debugging

### Enable Logging
```cpp
UE_LOG(LogTemp, Log, TEXT("Message"));
UE_LOG(LogTemp, Warning, TEXT("Warning"));
UE_LOG(LogTemp, Error, TEXT("Error"));
```

### Network Debugging
```
net.PackageMap.Dump
net.RepLayout.Dump
```

### Console Commands
```
stat FPS          # Show FPS
stat Unit         # Show frame times
stat Memory       # Show memory usage
r.ScreenPercentage 50  # Reduce render scale
```

## Performance Tips

1. **Chunk Loading**
   - Adjust `RenderDistance` in VoxelWorld
   - Lower for better performance
   - Higher for more visible world

2. **Mesh Generation**
   - Only regenerate changed chunks
   - Use mesh sections for materials

3. **Networking**
   - Limit replication frequency
   - Use relevancy for distant objects

## File Locations

```
Source/VoxelSurvival/        # C++ source
Content/Maps/                # Game levels
Content/Blueprints/          # Blueprint assets
Content/Materials/           # Materials
Config/                      # Configuration
```

## Useful Links

- [Unreal C++ API](https://docs.unrealengine.com/5.0/en-US/API/)
- [Network Replication](https://docs.unrealengine.com/5.0/en-US/networking-overview-for-unreal-engine/)
- [ProceduralMeshComponent](https://docs.unrealengine.com/5.0/en-US/API/Plugins/ProceduralMeshComponent/)

## Common Issues

**Build Error: "Cannot find module"**
- Regenerate project files
- Clean solution and rebuild

**Voxels Not Appearing**
- Check VoxelWorld is in level
- Verify RenderDistance > 0
- Check player position

**Multiplayer Not Working**
- Verify replication enabled (bReplicates = true)
- Check HasAuthority() for server code
- Test in Listen Server mode first

**Performance Issues**
- Reduce RenderDistance
- Simplify mesh generation
- Profile with Unreal Insights
