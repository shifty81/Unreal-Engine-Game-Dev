# Example Mod for Voxel Survival

This is a template plugin that demonstrates how to create mods for Voxel Survival.

## 🎯 Visual Guide Available!

See [../../SETUP_GUIDE.md](../../SETUP_GUIDE.md) for visual instructions on building the main project first.

**⚠️ Important:** You must build the main VoxelSurvival project successfully before this plugin will work!

## What's Included

- **Plugin Structure**: Complete plugin setup with `.uplugin` file
- **Build Configuration**: `ExampleMod.Build.cs` with proper dependencies
- **Module Setup**: Module initialization in `ExampleMod.h/cpp`
- **Example Building**: `AExampleBuilding` class showing how to extend buildings

## Prerequisites

Before using this mod template:
1. ✅ VoxelSurvival must be built successfully in Visual Studio
2. ✅ Unreal Engine 5.6.1 must be able to open VoxelSurvival.uproject
3. ✅ You should understand basic C++ and Unreal Engine concepts

**If you're getting "modules missing" errors:**
- You haven't built VoxelSurvival yet
- See [../../SETUP_GUIDE.md](../../SETUP_GUIDE.md) for step-by-step setup instructions

## How to Use This Template

### 1. Copy the Plugin

Copy the entire `Plugins/ExampleMod` folder and rename it to your mod name:
```
Plugins/ExampleMod → Plugins/MyModName
```

### 2. Rename Files

Rename all files and classes:
- `ExampleMod.uplugin` → `MyModName.uplugin`
- `ExampleMod.Build.cs` → `MyModName.Build.cs`
- Update class names: `FExampleModModule` → `FMyModNameModule`

### 3. Update Module Names

In `.uplugin` file:
```json
"Modules": [
    {
        "Name": "MyModName",  // Changed
        ...
    }
]
```

In `.Build.cs` file:
```csharp
public class MyModName : ModuleRules  // Changed
{
    ...
}
```

In module files:
```cpp
IMPLEMENT_MODULE(FMyModNameModule, MyModName)  // Changed
```

### 4. Add Your Content

Add your custom classes to `Source/MyModName/Public` and `Private` folders.

### 5. Enable the Plugin

1. Open Unreal Editor
2. Edit → Plugins
3. Find your mod
4. Check "Enabled"
5. Restart editor

### 6. Build

Generate project files and rebuild solution.

## What You Can Mod

### Custom Buildings
Extend `ABuilding` like `AExampleBuilding` does.

### Custom Voxel Types
Add new voxel types in your mod header.

### Custom Ship Components
Create new ship parts and systems.

### World Generation
Add custom world generation algorithms.

### Game Modes
Create new game modes with custom rules.

## Example: Adding a Custom Building

```cpp
// MyCustomBuilding.h
UCLASS()
class MYMOD_API AMyCustomBuilding : public ABuilding
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProductionRate = 1.0f;

protected:
    virtual void Tick(float DeltaTime) override;
};

// MyCustomBuilding.cpp
void AMyCustomBuilding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Custom production logic
}
```

## Example: Adding Custom Voxel Type

```cpp
// In your mod header
UENUM(BlueprintType)
enum class EMyVoxelType : uint8
{
    Diamond UMETA(DisplayName = "Diamond"),
    Emerald UMETA(DisplayName = "Emerald")
};
```

## Testing Your Mod

1. Enable plugin in editor
2. Create Blueprint from your C++ class
3. Place in level
4. Test in PIE (Play In Editor)
5. Test in multiplayer

## Distribution

### Package Plugin
1. File → Package Plugin
2. Select output directory
3. Choose target platforms
4. Click "Package"

### Share
- Upload to Unreal Marketplace
- Share on community forums
- Host on GitHub

## Resources

- Main Game Documentation: `../../GAME_DOCUMENTATION.md`
- Modding Guide: `../../MODDING_GUIDE.md`
- Contributing: `../../CONTRIBUTING.md`

## Support

For help with modding:
- Check the modding guide
- Review this example
- Ask in community forums
- Open an issue on GitHub

## License

This example mod template is provided as-is for creating mods for Voxel Survival.
