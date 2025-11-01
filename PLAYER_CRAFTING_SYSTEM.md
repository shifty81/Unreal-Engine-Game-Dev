# Player Character and Crafting System Documentation

## Overview

The Voxel Survival game now features a complete player character system with movement, inventory management, resource gathering, crafting, and equipment. Players can harvest resources, craft tools, and use those tools to gather more advanced materials.

## Player Character (ASurvivalPlayerCharacter)

### Movement System
- **WASD Movement** - Standard character movement
- **Mouse Look** - Camera control with mouse
- **Jump** - Space bar to jump
- **Sprint** - Hold Shift to sprint (consumes stamina)
- **Third-Person Camera** - Spring arm camera with 400 unit distance
- **Walk Speed**: 600 units/second
- **Sprint Speed**: 900 units/second

### Player Stats
All stats are networked and replicated to all clients:

- **Health**: 0-100 (damaged by hunger/thirst depletion)
- **Stamina**: 0-100 (consumed by sprinting, regenerates when walking/idle)
- **Hunger**: 0-100 (decays over time, causes damage when depleted)
- **Thirst**: 0-100 (decays faster than hunger, causes damage when depleted)

### Interaction System
- **Interaction Range**: 300 units
- **Resource Harvesting**: Click on resources to harvest with equipped tool
- **Visual Feedback**: Current interaction target is tracked
- **Server-Authoritative**: All interactions validated on server

## Inventory System (UInventoryComponent)

### Features
- **40 Inventory Slots** - Main inventory storage
- **10 Hotbar Slots** - Quick-access slots (subset of main inventory)
- **Stack Management** - Resources stack up to 999
- **Item Categories**: Resource, Tool, Weapon, Consumable, Building, Miscellaneous
- **Network Replication**: Full multiplayer support

### Hotbar Controls
- **1-9, 0 Keys** - Select hotbar slots 1-10
- **Selected Item** - Automatically equips in player's hand
- **Visual Indicator**: Shows currently selected slot

### Item System
Items are defined with the following properties:
- Name and description
- Category and type
- Stack size
- Durability (for tools/weapons)
- Harvest power (for tools)
- Damage (for weapons)
- Icon and 3D mesh references

## Crafting System (UCraftingComponent)

### Crafting Mechanics
- **Recipe-Based** - All items crafted from defined recipes
- **Material Requirements** - Check inventory for required materials
- **Instant Crafting** - Items crafted immediately (not time-based in menu)
- **Forge Required** - Some recipes require a forge building

### Tool Recipes

#### Wood Tools (No Forge Required)
- **Wood Pickaxe**: 10 Wood + 5 Wood handle (5 seconds)
- **Wood Axe**: 10 Wood + 5 Wood handle (5 seconds)

#### Stone Tools (No Forge Required)
- **Stone Pickaxe**: 8 Stone + 5 Wood (8 seconds)
- **Stone Axe**: 8 Stone + 5 Wood (8 seconds)
- **Stone Shovel**: 6 Stone + 5 Wood (8 seconds)

#### Iron Tools (Forge Required)
- **Iron Pickaxe**: 5 Iron + 5 Wood (12 seconds)
- **Iron Axe**: 5 Iron + 5 Wood (12 seconds)
- **Iron Shovel**: 4 Iron + 5 Wood (12 seconds)
- **Iron Hammer**: 6 Iron + 5 Wood (12 seconds)

#### Steel Tools (Forge Required)
- **Steel Pickaxe**: 5 Steel + 5 Wood (15 seconds)
- **Steel Axe**: 5 Steel + 5 Wood (15 seconds)

### Material Processing
See RESOURCE_SYSTEM.md for complete material processing recipes including smelting and steel production.

## Tool System

### Tool Types

#### Pickaxe
- **Best For**: Mining stone, ores
- **Damage**: 10 (base)
- **Harvest Power**: 2.0 (base)
- **Durability**:
  - Stone: 100
  - Iron: 200 (1.5x power)
  - Steel: 400 (2.0x power)

#### Axe
- **Best For**: Chopping trees, wood
- **Damage**: 15 (base)
- **Harvest Power**: 3.0 (base)
- **Durability**:
  - Wood: 50
  - Stone: 100
  - Iron: 200 (1.5x power)
  - Steel: 400 (2.0x power)

#### Shovel
- **Best For**: Digging dirt, sand
- **Damage**: 5 (base)
- **Harvest Power**: 2.5 (base)
- **Durability**:
  - Stone: 100
  - Iron: 200 (1.5x power)

#### Hammer
- **Best For**: Building, combat
- **Damage**: 20 (base)
- **Harvest Power**: 1.5 (base)
- **Durability**:
  - Iron: 200

### Tool Durability
- Tools lose 1 durability per use
- Broken tools are automatically removed from inventory
- Better materials = higher durability and harvest power
- Harvest power multiplies base harvest amount

## Equipment System

### Equipping Items
1. Add tool to hotbar slot (slots 1-10)
2. Press number key to select slot
3. Tool automatically equips in character's hand
4. Tool mesh becomes visible

### Using Equipped Items
- **Left Click / Use Button** - Use equipped tool
- **Target Resources** - Look at resource nodes to harvest
- **Auto-Harvest** - Tool automatically applies to targeted resource
- **Durability Loss** - Each use consumes 1 durability

### Visual Feedback
- Equipped tools display 3D mesh in character's hand
- Mesh rotates and positions based on tool type
- Visible to all players in multiplayer

## Input Mapping

### Movement
- **W** - Move Forward
- **S** - Move Backward
- **A** - Move Left
- **D** - Move Right
- **Space** - Jump
- **Shift** - Sprint (hold)
- **Mouse** - Look Around

### Actions
- **1-9, 0** - Select Hotbar Slot
- **E** - Interact
- **Left Click** - Use Equipped Item
- **Tab** - Toggle Inventory (UI)

### Camera
- **Mouse X** - Horizontal Look
- **Mouse Y** - Vertical Look
- **Mouse Wheel** - Zoom (if implemented)

## Asset Organization

### Textures
- **Resources**: `/Content/Textures/Resources/`
  - 20 resource textures (ores, metals, wood, etc.)
- **Tools**: `/Content/Textures/Tools/`
  - 11 tool textures (pickaxes, axes, shovels, hammers)

### Meshes
- **Tools**: `/Content/Meshes/Tools/`
  - 11 tool meshes in OBJ format
  - SM_WoodPickaxe.obj
  - SM_StonePickaxe.obj
  - SM_IronPickaxe.obj
  - SM_SteelPickaxe.obj
  - SM_WoodAxe.obj
  - SM_StoneAxe.obj
  - SM_IronAxe.obj
  - SM_SteelAxe.obj
  - SM_StoneShovel.obj
  - SM_IronShovel.obj
  - SM_IronHammer.obj

### Source Code
- **Player**: `Source/VoxelSurvival/SurvivalPlayerCharacter.h/cpp`
- **Inventory**: `Source/VoxelSurvival/InventoryComponent.h/cpp`
- **Items**: `Source/VoxelSurvival/InventoryItem.h/cpp`
- **Crafting**: `Source/VoxelSurvival/CraftingComponent.h/cpp`
- **Recipes**: `Source/VoxelSurvival/CraftingRecipe.h/cpp`

## Multiplayer Considerations

### Replicated Properties
- Player stats (Health, Stamina, Hunger, Thirst)
- Equipped item
- Inventory slots
- Selected hotbar slot

### Server Authority
- All resource harvesting validated on server
- Tool durability tracked server-side
- Crafting recipes verified on server
- Inventory changes authorized by server

### Client Prediction
- Movement is client-predicted
- Camera control is local
- UI updates immediately for responsiveness

## Blueprint Integration

All systems are fully exposed to Blueprint:

### Player Character
- `GetEquippedItem()` - Get currently equipped item
- `EquipItem(SlotIndex)` - Equip item from slot
- `UseEquippedItem()` - Use current tool/weapon
- `HarvestResource(ResourceNode)` - Harvest from node
- `CanSprint()` - Check if player can sprint

### Inventory
- `AddItem(Item)` - Add item to inventory
- `RemoveItem(SlotIndex, Amount)` - Remove items
- `AddMaterial(MaterialType, Amount)` - Add resources
- `HasMaterial(MaterialType, Amount)` - Check resources
- `MoveItem(FromSlot, ToSlot)` - Rearrange inventory

### Crafting
- `GetAvailableRecipes()` - Get all recipes
- `CanCraftRecipe(Recipe)` - Check if craftable
- `CraftItem(Recipe)` - Craft from recipe

## Usage Examples

### Harvesting a Tree
```cpp
// Player looks at tree and presses Use
1. UpdateInteractionTarget() detects tree in range
2. UseEquippedItem() called
3. Checks if equipped item is a tool (axe preferred)
4. Calls HarvestResource(TreeNode)
5. TreeNode->HarvestResource(Amount * HarvestPower)
6. Wood added to inventory
7. Tool durability decreased
```

### Crafting a Stone Pickaxe
```cpp
// Player opens crafting menu
1. CraftingComponent->GetAvailableRecipes() displays options
2. Player selects "Stone Pickaxe"
3. CraftingComponent->CanCraftRecipe() checks: 8 Stone + 5 Wood
4. If materials available, CraftItem() called
5. Materials consumed: RemoveMaterial(Stone, 8), RemoveMaterial(Wood, 5)
6. Tool created: CreateToolItem(Pickaxe, Stone)
7. Tool added to inventory
```

### Using a Tool
```cpp
// Player equips Stone Pickaxe and mines Iron Ore
1. Player presses "1" key
2. SelectHotbarSlot(0) called
3. EquipItem(0) loads Stone Pickaxe
4. UpdateEquippedItemMesh() shows pickaxe in hand
5. Player aims at Iron Ore node
6. Left click -> UseEquippedItem()
7. HarvestResource() called with IronOre node
8. Harvest amount = 5 (base) * 1.0 (stone multiplier) = 5
9. 5 Iron Ore added to inventory
10. Pickaxe durability: 100 -> 99
```

## Performance Considerations

- **Inventory Updates**: Only send network updates when items actually change
- **Tool Meshes**: Meshes loaded on-demand, not pre-cached
- **Recipe Checking**: Cached recipe list, not recalculated each frame
- **Interaction Traces**: Only performed on locally controlled pawns
- **Stats Update**: Server-only, 60 FPS friendly

## Future Enhancements

Potential additions to the player/crafting system:
- Animation system for tool use
- Weapon combat system
- Food/drink consumables
- Building placement mode
- Quick-craft from hotbar
- Recipe discovery system
- Blueprint tool customization
- Advanced weapon types
- Armor/clothing system
- Experience and skill progression

## Troubleshooting

### Tools Not Appearing in Hand
- Check that tool mesh is properly assigned in item definition
- Verify hand socket exists on character skeleton ("hand_rSocket")
- Ensure mesh component is set to visible when equipped

### Cannot Craft Items
- Verify player has required materials in inventory
- Check if recipe requires forge and player is near one
- Ensure CraftingComponent is properly initialized
- Check console for error messages

### Resources Not Harvesting
- Verify resource node is not depleted
- Check interaction range (default 300 units)
- Ensure player has tool equipped
- Verify resource node has correct collision

### Inventory Not Updating
- Check network replication is enabled
- Verify server authority for inventory changes
- Ensure InventoryComponent is properly attached
- Check OnInventoryChanged delegate is broadcasting

## Related Documentation

- **RESOURCE_SYSTEM.md** - Complete resource gathering and processing guide
- **GAME_DOCUMENTATION.md** - Overall game systems documentation
- **MODDING_GUIDE.md** - How to extend the crafting system
