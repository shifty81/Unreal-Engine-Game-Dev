# 🚀 Quick Start Guide

## Getting Started in 5 Minutes

### 1. Open the Project ✅
```bash
# The rebuild error has been FIXED!
# Simply open VoxelSurvival.uproject in Unreal Engine 5.4+
```

### 2. Key Files to Know

**C++ Classes** (ready to use):
- `SurvivalPlayerCharacter` - Your playable character
- `InventoryComponent` - Manages items (40 + 10 slots)
- `CraftingComponent` - Handles tool crafting
- `ResourceNode` - Harvestable resources in world
- `ResourceManager` - Spawns resources around player

**Assets** (already generated):
- `Content/Textures/Resources/` - 20 resource textures
- `Content/Textures/Tools/` - 11 tool textures
- `Content/Meshes/Tools/` - 11 tool 3D meshes (.obj)

### 3. Basic Gameplay Loop

```
1. Harvest wood from trees (punch or use axe)
   🌲 → 🪵 Wood

2. Craft basic tools
   🪵 Wood (15) → 🪓 Wood Axe

3. Use axe to get more wood
   🪓 + 🌲 → 🪵🪵🪵 More Wood

4. Harvest stone
   ⛏️ + 🪨 → 🪨 Stone

5. Craft better tools
   🪨 Stone (8) + 🪵 Wood (5) → ⛏️ Stone Pickaxe

6. Mine ores
   ⛏️ + ⛰️ → ⚙️ Iron Ore

7. Smelt in forge
   ⚙️ Iron Ore → 🛡️ Iron

8. Craft advanced tools
   🛡️ Iron (5) + 🪵 Wood (5) → ⛏️ Iron Pickaxe
```

### 4. Controls

**Movement**:
- `WASD` - Move
- `Space` - Jump
- `Shift` - Sprint
- `Mouse` - Look around

**Actions**:
- `E` - Interact with objects
- `Left Click` - Use equipped tool
- `Tab` - Open inventory
- `1-0` - Select hotbar slot

### 5. Import Assets to Unreal

**Tool Meshes**:
1. In Unreal: Content Browser → Right-click → Import
2. Navigate to: `Content/Meshes/Tools/`
3. Select all .obj files → Import
4. Set scale to 100 (or adjust as needed)

**Textures**:
1. Drag PNG files from `Content/Textures/` folders
2. Or use File → Import
3. Create materials from textures
4. Apply to tool meshes

### 6. Set Up Input (Required!)

Go to: **Edit → Project Settings → Input**

Add these mappings (see INPUT_CONFIGURATION.md for complete list):

**Action Mappings**:
- Jump → Space
- Sprint → Left Shift
- Use → Left Mouse Button
- Interact → E
- Inventory → Tab
- Hotbar1-0 → Keys 1-0

**Axis Mappings**:
- MoveForward → W (1.0), S (-1.0)
- MoveRight → D (1.0), A (-1.0)
- Turn → Mouse X (1.0)
- LookUp → Mouse Y (-1.0)

### 7. Place in Level

**Required Actors**:
1. Place `SurvivalPlayerCharacter` in level
2. Set as default pawn in GameMode
3. Place `ResourceManager` in level
4. Configure resource spawn settings

**Optional**:
- Place `Forge` actors for smelting
- Place `SalvageStation` for recycling
- Add resource nodes manually (or let manager spawn them)

### 8. Test Multiplayer

**Host Game**:
```
Play → Net Mode → "Play As Listen Server"
Number of Players: 2+
```

**Join Game**:
- Second player connects automatically
- All inventory/crafting synced
- Resources shared across clients

### 9. Extend with Blueprint

All C++ classes are exposed to Blueprint:

**Create BP_PlayerCharacter** (inherits SurvivalPlayerCharacter):
- Add animations
- Customize stats
- Override functions
- Add custom behavior

**Create BP_ResourceNode** (inherits ResourceNode):
- Set custom meshes
- Configure drop amounts
- Add visual effects
- Custom interaction

**Create BP_Widgets**:
- Inventory UI
- Crafting menu
- Hotbar display
- Stats HUD

### 10. Common Recipes

**Early Game** (no forge):
- Wood Axe: Wood (15)
- Wood Pickaxe: Wood (15)
- Stone Axe: Stone (8) + Wood (5)
- Stone Pickaxe: Stone (8) + Wood (5)

**Mid Game** (requires forge):
- Iron → Smelt Iron Ore
- Iron Axe: Iron (5) + Wood (5)
- Iron Pickaxe: Iron (5) + Wood (5)

**Advanced** (requires forge + coal):
- Charcoal: Wood (2) → Charcoal
- Steel: Iron (2) + Coal (1)
- Steel Axe: Steel (5) + Wood (5)

## 📚 Full Documentation

- **BUILD_SUMMARY.md** - What we built
- **RESOURCE_SYSTEM.md** - Complete resource guide
- **PLAYER_CRAFTING_SYSTEM.md** - Player & crafting
- **INPUT_CONFIGURATION.md** - Input setup details

## 🆘 Troubleshooting

**"Module not found" error**:
- Right-click .uproject → Generate Visual Studio Files
- Build in Development Editor configuration

**Input not working**:
- Check INPUT_CONFIGURATION.md
- Verify input mappings in Project Settings
- Ensure player controller has input enabled

**Tools not appearing in hand**:
- Import tool meshes first
- Check that mesh is assigned in item definition
- Verify character has hand socket ("hand_rSocket")

**Resources not spawning**:
- Place ResourceManager in level
- Configure spawn settings
- Check spawn radius (default 5000 units)

## 🎮 Have Fun!

You now have a complete survival game foundation. Start building, crafting, and expanding!

Need help? Check the full documentation files.
