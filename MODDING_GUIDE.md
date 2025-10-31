# Modding Guide - Voxel Survival Game

This guide explains how to create mods and expand the game functionality.

## Overview

The game is designed with modding in mind. All major systems are data-driven and can be extended through:
- C++ classes
- Blueprint extensions
- Data files
- Custom content

## Getting Started

### Prerequisites
1. Unreal Engine 5.6.1
2. Visual Studio 2022 (for C++ mods)
3. Basic knowledge of C++ or Blueprints

### Setting Up a Mod

1. **Create a Plugin**
   - In Unreal Editor: Edit → Plugins → New Plugin
   - Choose "Blank" template
   - Name your plugin (e.g., "MyVoxelMod")

2. **Add Dependencies**
   Edit your plugin's `.uplugin` file:
   ```json
   {
     "Modules": [
       {
         "Name": "MyVoxelMod",
         "Type": "Runtime",
         "LoadingPhase": "Default",
         "AdditionalDependencies": [
           "VoxelSurvival"
         ]
       }
     ]
   }
   ```

## Modding Systems

### 1. Custom Voxel Types

Add new voxel block types:

**Step 1: Extend VoxelData.h**
```cpp
// In your mod's header file
UENUM(BlueprintType)
enum class EModVoxelType : uint8
{
    Copper UMETA(DisplayName = "Copper Ore"),
    Silver UMETA(DisplayName = "Silver Ore"),
    Crystal UMETA(DisplayName = "Crystal")
};
```

**Step 2: Register with Voxel System**
```cpp
// In your mod initialization
void FMyVoxelMod::StartupModule()
{
    // Register custom voxel types
    RegisterCustomVoxelType("Copper", FColor::Orange);
    RegisterCustomVoxelType("Silver", FColor::Silver);
}
```

### 2. Custom World Generation

Create unique world generation algorithms:

**Example: Mountain Biome Generator**
```cpp
UCLASS()
class AMyWorldGenerator : public AActor
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void GenerateMountainTerrain(AVoxelChunk* Chunk)
    {
        // Custom generation logic
        for (int32 Z = 0; Z < Chunk->ChunkSize; Z++)
        {
            for (int32 Y = 0; Y < Chunk->ChunkSize; Y++)
            {
                for (int32 X = 0; X < Chunk->ChunkSize; X++)
                {
                    float Height = CalculateMountainHeight(X, Y);
                    if (Z < Height)
                    {
                        Chunk->SetVoxel(X, Y, Z, EVoxelType::Stone);
                    }
                }
            }
        }
    }

private:
    float CalculateMountainHeight(int32 X, int32 Y)
    {
        // Your custom height calculation
        return FMath::PerlinNoise2D(FVector2D(X * 0.01f, Y * 0.01f)) * 50.0f;
    }
};
```

### 3. Custom Buildings

Create new building types:

**Example: Resource Generator**
```cpp
UCLASS()
class AResourceGenerator : public ABuilding
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVoxelType ResourceType = EVoxelType::Iron;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GenerationRate = 1.0f;

protected:
    virtual void Tick(float DeltaTime) override
    {
        Super::Tick(DeltaTime);
        
        if (HasAuthority())
        {
            ResourceTimer += DeltaTime;
            if (ResourceTimer >= GenerationRate)
            {
                GenerateResource();
                ResourceTimer = 0.0f;
            }
        }
    }

private:
    float ResourceTimer = 0.0f;

    void GenerateResource()
    {
        // Spawn resource in world
        UE_LOG(LogTemp, Log, TEXT("Generated resource!"));
    }
};
```

### 4. Custom Ship Components

Add new ship parts:

**Example: Weapon System**
```cpp
UCLASS()
class AShipWeapon : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Range = 1000.0f;

    UFUNCTION(BlueprintCallable)
    void Fire()
    {
        // Implement weapon firing logic
    }
};
```

**Adding to Ship**
```cpp
void AddWeaponToShip(AShip* Ship)
{
    FShipComponent WeaponComponent;
    WeaponComponent.ComponentName = "LaserCannon";
    WeaponComponent.RelativePosition = FVector(100, 0, 0);
    WeaponComponent.ComponentType = 1; // Weapon type
    
    Ship->AddComponent(WeaponComponent);
}
```

### 5. Custom Game Modes

Create new game modes:

```cpp
UCLASS()
class AMyGameMode : public AVoxelSurvivalGameMode
{
    GENERATED_BODY()

public:
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override
    {
        Super::InitGame(MapName, Options, ErrorMessage);
        
        // Custom initialization
        SetupCustomRules();
    }

protected:
    void SetupCustomRules()
    {
        // Implement custom game rules
    }
};
```

## Data-Driven Modding

### Creating Data Tables

1. **Define Structure**
```cpp
USTRUCT(BlueprintType)
struct FBuildingData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BuildingName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BuildTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResourceCost;
};
```

2. **Create Data Table in Editor**
   - Content Browser → Right Click → Miscellaneous → Data Table
   - Choose `FBuildingData` as row structure
   - Add rows with building data

3. **Load in Code**
```cpp
UDataTable* BuildingTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Buildings"));
if (BuildingTable)
{
    FBuildingData* BuildingData = BuildingTable->FindRow<FBuildingData>("Tower", TEXT(""));
    if (BuildingData)
    {
        // Use building data
    }
}
```

## Blueprint Modding

### Extending Character

1. Create Blueprint based on `AWireframeCharacter`
2. Add custom variables and functions
3. Override tick and events
4. Example: Add jetpack functionality

### Custom Buildings via Blueprint

1. Create Blueprint based on `ABuilding`
2. Add visual meshes
3. Implement custom behavior in Event Graph
4. Set up in level

## Serialization for Mods

### Saving Custom Data

```cpp
UCLASS()
class UMyModSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FString> CustomData;

    UPROPERTY()
    TMap<FString, int32> ModSettings;
};

// Saving
UMyModSaveGame* SaveGame = Cast<UMyModSaveGame>(UGameplayStatics::CreateSaveGameObject(UMyModSaveGame::StaticClass()));
SaveGame->CustomData.Add("MyData");
UGameplayStatics::SaveGameToSlot(SaveGame, "ModSave", 0);

// Loading
UMyModSaveGame* LoadedGame = Cast<UMyModSaveGame>(UGameplayStatics::LoadGameFromSlot("ModSave", 0));
```

## Networking for Mods

### Replicating Custom Data

```cpp
UCLASS()
class AMyModActor : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(Replicated)
    float CustomValue;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
    {
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        DOREPLIFETIME(AMyModActor, CustomValue);
    }
};
```

### RPC Functions

```cpp
UFUNCTION(Server, Reliable)
void ServerDoSomething();

UFUNCTION(NetMulticast, Reliable)
void MulticastDoSomething();

// Implementation
void AMyModActor::ServerDoSomething_Implementation()
{
    // Server-side logic
}

void AMyModActor::MulticastDoSomething_Implementation()
{
    // Runs on all clients
}
```

## Performance Considerations

### Optimization Tips

1. **Chunk Loading**
   - Don't modify chunks outside render distance
   - Use async loading for large structures

2. **Replication**
   - Only replicate essential data
   - Use `COND_OwnerOnly` for player-specific data

3. **Mesh Generation**
   - Cache generated meshes
   - Update only when voxels change

4. **Building Influence**
   - Batch territory updates
   - Use spatial partitioning

## Testing Your Mod

### Local Testing

1. Enable your plugin in Unreal Editor
2. Create test map with your content
3. Play in editor (PIE)

### Multiplayer Testing

1. Launch with command: `-game -log`
2. Use multiple instances for testing
3. Test client-server synchronization

## Distribution

### Packaging Your Mod

1. **Plugin Method**
   - Package plugin: File → Package Plugin
   - Distribute `.uplugin` and binaries

2. **Pak File Method**
   - Cook content: File → Package Project
   - Extract content to `.pak` file
   - Place in game's Paks folder

### Mod Structure

```
MyVoxelMod/
├── MyVoxelMod.uplugin
├── Source/
│   └── MyVoxelMod/
├── Content/
│   ├── Blueprints/
│   ├── Data/
│   └── Materials/
└── README.md
```

## Example Mods

### 1. Ore Expansion Mod

Adds 5 new ore types with custom colors and generation patterns.

### 2. Advanced Ships Mod

Adds weapon systems, shields, and more ship components.

### 3. Biome Overhaul

Replaces default terrain generation with multiple biomes.

### 4. PvP Arena Mod

Adds dedicated PvP zones with custom rules.

## Troubleshooting

### Common Issues

**Mod Not Loading**
- Check plugin dependencies
- Verify module name in `.uplugin`
- Check for compilation errors

**Network Desync**
- Ensure proper replication setup
- Use RPCs for actions
- Test authority checks

**Performance Issues**
- Profile with Unreal Insights
- Check tick frequency
- Optimize mesh generation

## Resources

- Unreal Engine Documentation: https://docs.unrealengine.com
- Game Source Code: See `Source/VoxelSurvival/`
- Community Discord: [Your Discord Link]
- Mod Examples: [GitHub Repository]

## Support

For modding support:
1. Check game documentation
2. Review example mods
3. Ask in community forums
4. Submit issues on GitHub

Happy modding!
