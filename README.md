# Voxel Survival - Unreal Engine 5.6.1

A comprehensive voxel-based survival game with RTS elements, featuring territory control, base building, ship construction, and full multiplayer support.

## Quick Start

This project is an Unreal Engine 5.6.1 game with full C++ source code.

### 🎯 New User? Start Here!

**📚 [SETUP_GUIDE.md](SETUP_GUIDE.md) - Complete Visual Setup Guide with Step-by-Step Instructions**

This comprehensive guide includes:
- ✅ Detailed visual diagrams
- ✅ Step-by-step screenshots descriptions
- ✅ Common error solutions
- ✅ Build process flowcharts
- ✅ Directory structure explanations

### Prerequisites
- **Unreal Engine 5.6.1** (or compatible 5.6.x version)
- **Visual Studio 2022** (Windows) with:
  - Desktop development with C++ workload
    - ⚠️ Including **MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)**
  - Game development with C++ workload
  - .NET desktop development
  - Windows 10/11 SDK (latest version)
- **Xcode** (Mac) - for macOS development
- **16GB RAM minimum** (32GB recommended)

### Quick Setup (Experienced Users)

**⚠️ IMPORTANT:** You **MUST** build in Visual Studio BEFORE opening the .uproject file!

1. **Clone the repository**
   ```bash
   git clone https://github.com/shifty81/Unreal-Engine-Game-Dev.git
   cd Unreal-Engine-Game-Dev
   ```

2. **Generate project files** ⚠️ **Don't skip this!**
   
   - Right-click `VoxelSurvival.uproject`
   - Select "Generate Visual Studio project files"
   - Wait for the process to complete (30-60 seconds)
   
   > **Common Error:** If you get "modules are missing or built with a different engine version",
   > you skipped this step! See [SETUP_GUIDE.md](SETUP_GUIDE.md) for detailed instructions.

3. **Build the project** ⚠️ **Required before opening!**
   - Open the generated `VoxelSurvival.sln` file
   - Select configuration: **"Development Editor"**
   - Select platform: **"Win64"**
   - Build the solution (Build → Build Solution or Ctrl+Shift+B)
   - Wait for successful build (5-15 minutes)

4. **Open in Unreal Engine** ✅ **Only after successful build!**
   - Double-click `VoxelSurvival.uproject`
   - First launch takes 5-10 minutes (shader compilation)

## Features

✅ **Voxel World System** - Chunk-based voxel world with procedural mesh generation  
✅ **Procedural Generation** - Customizable terrain generation with noise-based algorithms  
✅ **Multiplayer Support** - Full network replication for online gameplay  
✅ **Modding Support** - Data-driven design with extensibility in mind  
✅ **Survival Mechanics** - Health, hunger, thirst, and stamina systems  
✅ **RTS Elements** - Territory control with influence spreading  
✅ **Base Building** - Constructible buildings that affect territory  
✅ **Ship Building System** - Modular ship construction with component system  
✅ **Hangar System** - Ship storage and construction facilities  
✅ **Resource System** - Complete resource gathering, processing, and crafting  
✅ **Player Character** - Full character with movement, inventory, and equipment  
✅ **Crafting System** - Tool and item crafting with recipes  
✅ **Tool System** - Pickaxes, axes, shovels, and hammers with durability  
✅ **GUI & Hotbar** - Inventory management and hotbar system  

## Documentation

- 🚀 [**Setup Guide (START HERE!)**](SETUP_GUIDE.md) - **Visual step-by-step setup with diagrams**
- 📖 [**Game Documentation**](GAME_DOCUMENTATION.md) - Comprehensive game systems guide
- 🔧 [**Modding Guide**](MODDING_GUIDE.md) - Complete modding and extension guide
- 🛠️ [**Troubleshooting**](TROUBLESHOOTING.md) - Common issues and solutions
- 🔒 [**Security Policy**](SECURITY.md) - Security advisories and vulnerability information

## Most Common Issue ⚠️

**"The following modules are missing or built with a different engine version: VoxelSurvival, ExampleMod"**

This error means you opened the `.uproject` file **before** building in Visual Studio. 

**Solution:**
1. Close the error dialog (click "No")
2. Follow the correct order: Generate Project Files → Build in Visual Studio → Open in Unreal Engine
3. See [SETUP_GUIDE.md](SETUP_GUIDE.md) for detailed visual instructions

**Never click "Yes" to rebuild** - the editor cannot rebuild without proper project files!

## Common Issues

Having trouble building the project? Check these common solutions:

- **"The following modules are missing or built with a different engine version"** ⚠️ **MOST COMMON**
  - You opened `.uproject` before building in Visual Studio
  - **DO NOT** click "Yes" to rebuild
  - Close editor, generate project files, build in VS, then open .uproject
  - See [SETUP_GUIDE.md](SETUP_GUIDE.md#issue-1-missing-or-built-with-different-engine-version-) for step-by-step fix

- **"Build tools for Visual Studio 2022 (v143) cannot be found"**
  - Make sure Visual Studio 2022 is installed with C++ workloads
  - ⚠️ **CRITICAL:** Must include **"MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)"** component
  - Open Visual Studio Installer → Modify → Desktop development with C++ → Check Installation details panel for v143
  - Regenerate project files (right-click `.uproject` → Generate Visual Studio project files)
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#visual-studio-2022-build-tools-not-found) for detailed steps

- **"Platform 'Win64' referenced in the project file cannot be found"**
  - This often appears with the v143 error above - check v143 build tools are installed first
  - Delete old project files and regenerate them
  - Make sure you open `VoxelSurvival.sln` in the project root, NOT files in Intermediate folder
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#win64-platform-cannot-be-found)

- **Project won't open in Unreal Engine**
  - Verify Unreal Engine 5.6.1 is installed
  - Right-click `.uproject` → Switch Unreal Engine version
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#missing-unreal-engine-association)

- **"Magick.NET-Q16-HDRI-AnyCPU vulnerability" warning** 🔒
  - This is a **false positive** - the project doesn't use this package
  - No action required - your code is not affected
  - See [SECURITY.md](SECURITY.md) for detailed analysis
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#magick-net-q16-hdri-anycpu-vulnerability-warning)

For more detailed solutions with visual guides, see:
- 🚀 [**SETUP_GUIDE.md**](SETUP_GUIDE.md) - Visual step-by-step setup instructions
- 🛠️ [**TROUBLESHOOTING.md**](TROUBLESHOOTING.md) - Comprehensive troubleshooting guide
- 🔒 [**SECURITY.md**](SECURITY.md) - Security advisories and false positives

## Project Structure

```
VoxelSurvival/
├── Source/              # C++ source code
│   └── VoxelSurvival/   # Main game module
├── Config/              # Configuration files
├── Content/             # Game assets and blueprints
├── GAME_DOCUMENTATION.md
├── MODDING_GUIDE.md
└── VoxelSurvival.uproject
```

## Core Systems

### Voxel System
- Chunk-based world management (16x16x16 voxels per chunk)
- Optimized procedural mesh generation
- Multiple voxel types (Stone, Dirt, Grass, Wood, Iron, Gold)
- Save/load functionality for persistence

### Survival System
- Health with damage and healing
- Hunger and thirst mechanics
- Stamina system
- Inventory management (expandable)

### Territory System
- Grid-based territory ownership
- Influence spreading from buildings
- Real-time territory updates
- Build restrictions based on control

### Ship Building
- Modular component system
- Save/load ship designs
- Hangar construction and storage
- Customizable ship properties

## Development Status

**Current Phase: Core Systems Complete**

- [x] Voxel world implementation
- [x] World generation system
- [x] Character and survival mechanics
- [x] Territory control system
- [x] Building system
- [x] Ship building and hangar
- [x] Multiplayer foundation
- [x] Modding infrastructure

**Next Phase: Enhanced Features**
- [ ] Resource gathering and crafting
- [ ] Combat mechanics
- [ ] AI systems
- [ ] Advanced UI
- [ ] Visual and audio polish

## Multiplayer

The game fully supports multiplayer with:
- Client-server architecture
- Network replication for all game objects
- Territory synchronization
- Ship and building replication

**To host a multiplayer game:**
```bash
# Listen Server (via editor)
Play → Net Mode → "Play As Listen Server"

# Dedicated Server (command line)
UE4Editor.exe "YourPath/VoxelSurvival.uproject" -server -log
```

## Modding

The game is designed for extensive modding:
- Plugin system support
- Data-driven design
- Serializable game data
- Blueprint extensibility

See [MODDING_GUIDE.md](MODDING_GUIDE.md) for complete modding documentation.

## Contributing

Contributions are welcome! Areas for contribution:
- New voxel types and materials
- Enhanced world generation algorithms
- Additional building types
- Ship components and systems
- UI improvements
- Performance optimizations

## License

See [LICENSE](LICENSE) file for details.

## Built With

- Unreal Engine 5.6.1
- C++ for core systems
- Blueprint for gameplay logic (optional)

## Credits

Created for the Unreal Engine Game Development community.

---

For detailed documentation, see [GAME_DOCUMENTATION.md](GAME_DOCUMENTATION.md)
