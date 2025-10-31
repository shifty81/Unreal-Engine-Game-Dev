# Voxel Survival - Unreal Engine 5.6.1

A comprehensive voxel-based survival game with RTS elements, featuring territory control, base building, ship construction, and full multiplayer support.

## Quick Start

This project is an Unreal Engine 5.6.1 game with full C++ source code.

### Prerequisites
- **Unreal Engine 5.6.1** (or compatible 5.6.x version)
- **Visual Studio 2022** (Windows) with:
  - Desktop development with C++ workload
  - Game development with C++ workload
  - .NET desktop development
  - Windows 10/11 SDK
- **Xcode** (Mac) - for macOS development
- **16GB RAM minimum** (32GB recommended)

### Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/shifty81/Unreal-Engine-Game-Dev.git
   cd Unreal-Engine-Game-Dev
   ```

2. **Generate project files**
   
   **⚠️ IMPORTANT:** You must generate Visual Studio project files before building!
   
   - Right-click `VoxelSurvival.uproject`
   - Select "Generate Visual Studio project files"
   - Wait for the process to complete
   
   > **Note:** If you get build errors about missing build tools or Win64 platform,
   > you likely skipped this step. See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for help.

3. **Build the project**
   - Open the generated `VoxelSurvival.sln` file
   - Select configuration: **"Development Editor"**
   - Select platform: **"Win64"**
   - Build the solution (Build → Build Solution or Ctrl+Shift+B)

4. **Open in Unreal Engine**
   - Double-click `VoxelSurvival.uproject`

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
✅ **Wireframe Characters** - Basic character system ready for expansion  

## Documentation

- 📖 [**Game Documentation**](GAME_DOCUMENTATION.md) - Comprehensive game systems guide
- 🔧 [**Modding Guide**](MODDING_GUIDE.md) - Complete modding and extension guide
- 🛠️ [**Troubleshooting**](TROUBLESHOOTING.md) - Common issues and solutions

## Common Issues

Having trouble building the project? Check these common solutions:

- **"Build tools for Visual Studio 2022 (v143) cannot be found"**
  - Make sure Visual Studio 2022 is installed with C++ workloads
  - Regenerate project files (right-click `.uproject` → Generate Visual Studio project files)
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#visual-studio-2022-build-tools-not-found)

- **"Platform 'Win64' referenced in the project file cannot be found"**
  - Delete old project files and regenerate them
  - Make sure you open `VoxelSurvival.sln`, not files in Intermediate folder
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#win64-platform-cannot-be-found)

- **Project won't open in Unreal Engine**
  - Verify Unreal Engine 5.6.1 is installed
  - Right-click `.uproject` → Switch Unreal Engine version
  - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#missing-unreal-engine-association)

For more detailed solutions, see [**TROUBLESHOOTING.md**](TROUBLESHOOTING.md).

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
