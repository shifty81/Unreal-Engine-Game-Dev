# Resource System Documentation

## Overview

The Voxel Survival game features a comprehensive resource gathering, processing, and crafting system. Resources can be harvested from the world, processed into refined materials, and used to craft advanced items.

## Resource Types

### Natural Resources (Harvestable from World)

1. **Trees** - Provides wood when harvested
   - Oak variant
   - Pine variant
   - Procedurally generated with random heights and sizes
   - Respawns after 10 minutes

2. **Stone/Rock** - Basic building material
   - Found throughout the world
   - Respawns after 5 minutes

3. **Iron Ore** - Raw iron ore deposits
   - Found in rocky areas
   - Must be smelted to create iron

4. **Copper Ore** - Raw copper deposits
   - Found in mountainous regions
   - Smelts into copper ingots

5. **Gold Ore** - Rare precious metal
   - Found deep underground
   - Smelts into gold ingots

6. **Aluminum Ore** - Lightweight metal ore
   - Found in specific biomes
   - Smelts into aluminum ingots

7. **Titanium Ore** - Advanced metal ore
   - Rare resource
   - Smelts into titanium ingots

8. **Coal Ore** - Fuel source
   - Found in caves and underground
   - Used for smelting and crafting steel

## Material Types

### Raw Materials
- Wood (from trees)
- Stone (from rocks)
- Iron Ore
- Copper Ore
- Gold Ore
- Aluminum Ore
- Titanium Ore
- Coal

### Processed Materials
- Iron (smelted from ore)
- Copper (smelted from ore)
- Gold (smelted from ore)
- Aluminum (smelted from ore)
- Titanium (smelted from ore)
- Steel (crafted from iron + coal/charcoal)
- Charcoal (processed from wood)

## Crafting System

### Forge

The **Forge** is used for smelting ores and creating advanced materials.

#### Features:
- Smelt ore into refined metals
- Create steel from iron and carbon sources
- Multiple crafting slots (3 concurrent jobs)
- Automatic processing with timers
- Input/output inventory management

#### Recipes:

**Smelting (Ore → Metal)**
- Iron Ore → Iron (8 seconds)
- Copper Ore → Copper (8 seconds)
- Gold Ore → Gold (10 seconds)
- Aluminum Ore → Aluminum (8 seconds)
- Titanium Ore → Titanium (12 seconds)

**Advanced Crafting**
- Iron (2) + Coal (1) → Steel (15 seconds)
- Iron (2) + Charcoal (1) → Steel (15 seconds)

### Salvage Station

The **Salvage Station** breaks down items into base materials and processes wood into charcoal.

#### Features:
- Convert wood items into charcoal
- Recycle items for partial material recovery
- Chance-based output system
- Automatic batch processing

#### Recipes:

**Primary Salvage**
- Wood (2) → Charcoal (1) @ 80% success rate (5 seconds)
- Steel → Iron (1) @ 100% recovery (8 seconds)

**Additional Salvage**
- Complex items can be salvaged for multiple material outputs
- Salvage recipes are extensible for custom items

## Resource Manager

The **Resource Manager** handles world-wide resource spawning and distribution.

### Features:
- Dynamic resource spawning around players
- Configurable spawn density and distribution
- Minimum spacing between resources
- Height-based spawn restrictions
- Automatic respawn management
- Multiplayer synchronized

### Configuration:
- Spawn Radius: 5000 units around players
- Spawn Check Interval: 10 seconds
- Density: Configurable per resource type
- Height Constraints: Min/Max spawn elevation

## Resource Nodes

All harvestable resources inherit from **AResourceNode** base class.

### Properties:
- **Resource Type**: Tree, Rock, Iron, etc.
- **Max Amount**: Total resources available
- **Current Amount**: Remaining resources
- **Respawn Time**: Seconds until respawn after depletion
- **Respawns**: Whether resource regenerates

### Special Features:

#### Trees (ATreeResource)
- Procedural generation with random variations
- Height: 100-1000 units
- Trunk Radius: 10-100 units
- Canopy Radius: 100-500 units
- Tree Variants: 0-3 (different species)
- Separate trunk and foliage meshes
- Randomized on respawn

## Textures

All resource textures are procedurally generated:

### Wood Textures
- T_Wood_Bark_Oak.png - Oak tree bark
- T_Wood_Bark_Pine.png - Pine tree bark
- T_Leaves_Oak.png - Oak foliage (with alpha)
- T_Leaves_Pine.png - Pine foliage (with alpha)

### Stone Textures
- T_Stone.png - Basic stone
- T_Rock_Gray.png - Gray rock variant

### Ore Textures (with mineral veins)
- T_IronOre.png - Iron ore with brown veins
- T_CopperOre.png - Copper ore with orange veins
- T_GoldOre.png - Gold ore with golden veins
- T_AluminumOre.png - Aluminum ore with silver veins
- T_TitaniumOre.png - Titanium ore with gray veins
- T_CoalOre.png - Coal deposits

### Refined Material Textures (metallic)
- T_Iron_Ingot.png - Refined iron
- T_Copper_Ingot.png - Refined copper
- T_Gold_Ingot.png - Refined gold
- T_Aluminum_Ingot.png - Refined aluminum
- T_Titanium_Ingot.png - Refined titanium
- T_Steel_Ingot.png - Steel alloy
- T_Charcoal.png - Processed charcoal
- T_Coal.png - Raw coal

Location: `Content/Textures/Resources/`

## Integration with Existing Systems

### Voxel World
- Resources spawn on terrain surface
- Surface detection via raytrace
- Respects voxel chunk boundaries

### Multiplayer
- All resource nodes replicate
- Server-authoritative spawning
- Client-side visual updates
- Synchronized harvesting

### Territory System
- Resources can be restricted to territory owners
- Building placement affects resource spawns
- Territory control can affect resource density

## Usage Examples

### Spawning Resources
```cpp
// In game mode or level blueprint
AResourceManager* ResourceManager = GetWorld()->SpawnActor<AResourceManager>();

// Configure tree spawning
FResourceSpawnInfo TreeSpawn;
TreeSpawn.ResourceClass = ATreeResource::StaticClass();
TreeSpawn.MinSpawnDistance = 500.0f;
TreeSpawn.MaxCount = 100;
TreeSpawn.SpawnDensity = 0.001f;

ResourceManager->ResourceSpawnConfigs.Add(TreeSpawn);
```

### Harvesting Resources
```cpp
// When player interacts with resource node
AResourceNode* Resource = /* Get from trace/overlap */;
int32 Harvested = Resource->HarvestResource(10);

// Add to player inventory
PlayerInventory->AddMaterial(EMaterialType::Wood, Harvested);
```

### Using the Forge
```cpp
// Add materials to forge
AForge* Forge = /* Get forge reference */;
Forge->AddMaterial(EMaterialType::IronOre, 10);

// Start smelting
FCraftingRecipe IronRecipe;
if (UCraftingRecipeLibrary::GetRecipeForMaterial(EMaterialType::Iron, IronRecipe))
{
    Forge->StartCrafting(IronRecipe);
}

// Collect results after crafting time
TMap<EMaterialType, int32> Results = Forge->CollectCompletedItems();
```

### Using the Salvage Station
```cpp
// Add wood to salvage station
ASalvageStation* Station = /* Get station reference */;
Station->AddInputMaterial(EMaterialType::Wood, 5);

// Start salvaging to create charcoal
Station->StartSalvaging(EMaterialType::Wood);

// Collect charcoal after processing
TMap<EMaterialType, int32> Output = Station->CollectOutput();
```

## Blueprint Integration

All resource system classes are exposed to Blueprint:

- **ResourceNode** - Harvestable resources
- **TreeResource** - Specialized tree implementation
- **ResourceManager** - World resource spawning
- **Forge** - Smelting and advanced crafting
- **SalvageStation** - Item breakdown and processing
- **CraftingRecipeLibrary** - Recipe definitions

All functions marked with `UFUNCTION(BlueprintCallable)` can be called from Blueprint.

## Future Enhancements

Potential additions to the resource system:
- Mining tools affecting harvest speed
- Resource quality/grades
- Rare resource variants
- Advanced alloys (bronze, brass, etc.)
- Refinery buildings for bulk processing
- Resource trading system
- Automated resource gathering
- Resource depletion mechanics
- Seasonal resource availability

## Performance Considerations

- Resources only spawn near players (5000 unit radius)
- Periodic spawn checks (10 second intervals)
- Depleted resources hidden, not destroyed
- Server-authoritative to prevent exploits
- Efficient distance checks using squared distance
- Resource count limits prevent over-spawning

## Modding Support

The resource system is fully extensible:
- Create custom resource types by inheriting from AResourceNode
- Define new crafting recipes in UCraftingRecipeLibrary
- Add salvage recipes to ASalvageStation
- Create custom processing buildings
- All enums are BlueprintType for easy extension
