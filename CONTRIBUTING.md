# Contributing to Voxel Survival

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Bugs

1. Check if the bug is already reported in Issues
2. If not, create a new issue with:
   - Clear title and description
   - Steps to reproduce
   - Expected vs actual behavior
   - Screenshots/logs if applicable
   - Your environment (UE version, OS, etc.)

### Suggesting Features

1. Check existing feature requests
2. Create a new issue describing:
   - Use case and motivation
   - Proposed implementation (if applicable)
   - Examples from other games/engines

### Code Contributions

1. **Fork the Repository**
   ```bash
   git clone https://github.com/shifty81/Unreal-Engine-Game-Dev.git
   cd Unreal-Engine-Game-Dev
   ```

2. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make Changes**
   - Follow the coding standards below
   - Test your changes thoroughly
   - Update documentation if needed

4. **Commit Changes**
   ```bash
   git add .
   git commit -m "Add: Brief description of changes"
   ```

5. **Push and Create PR**
   ```bash
   git push origin feature/your-feature-name
   # Create Pull Request on GitHub
   ```

## Coding Standards

### C++ Style

```cpp
// Class naming: PascalCase with prefix
class VOXELSURVIVAL_API AVoxelChunk : public AActor
{
    GENERATED_BODY()

public:
    // Public functions: PascalCase
    void GenerateMesh();

protected:
    // Protected members: PascalCase
    UProceduralMeshComponent* MeshComponent;

private:
    // Private functions: PascalCase
    void InternalFunction();
    
    // Private members: PascalCase
    int32 InternalValue;
};
```

### Naming Conventions

- **Classes**: PascalCase with A/U/F prefix
  - `AActor` for actors
  - `UObject` for objects
  - `FStruct` for structs
  
- **Variables**: PascalCase
  - `HealthPoints`, `MaxSpeed`
  
- **Functions**: PascalCase
  - `GetHealth()`, `SetVoxel()`
  
- **Constants**: UPPER_CASE
  - `MAX_CHUNK_SIZE`, `DEFAULT_SPEED`

### Comments

```cpp
/**
 * Multi-line documentation comment
 * for classes and public functions
 */
UCLASS()
class AVoxelChunk : public AActor
{
    /** Single line comment for properties */
    UPROPERTY()
    float VoxelSize;
};
```

### Blueprints

- **Naming**: BP_ prefix
  - `BP_WireframeCharacter`
  - `BP_VoxelWorld`
  
- **Variables**: Descriptive names
- **Functions**: Clear, single-purpose
- **Comments**: Document complex logic

## Project Structure

```
Source/VoxelSurvival/
├── Core/              # Core game systems
├── Voxel/            # Voxel-related classes
├── Gameplay/         # Gameplay mechanics
├── RTS/              # RTS elements
└── Multiplayer/      # Network code
```

## Testing

### Before Submitting PR

1. **Compile Successfully**
   - Development Editor configuration
   - Shipping configuration (if applicable)
   - If you encounter build errors, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

2. **Test in Editor**
   - PIE (Play In Editor)
   - Standalone game

3. **Test Multiplayer**
   - Listen server mode
   - Multiple clients

4. **Check for Warnings**
   - No compiler warnings
   - No blueprint warnings

### Writing Tests

(Future: Add test framework documentation)

## Documentation

### Code Documentation

- Document all public APIs
- Explain complex algorithms
- Include usage examples

### User Documentation

- Update README.md if needed
- Update GAME_DOCUMENTATION.md for features
- Update MODDING_GUIDE.md for extensibility
- Update TROUBLESHOOTING.md for common issues

### Visual Documentation 🎨

**We encourage adding screenshots and diagrams to documentation!**

See [docs/README.md](docs/README.md) for guidelines on adding visual assets.

**High Priority Screenshots Needed:**
- Setup process steps
- Error messages and solutions
- UI elements and workflows
- Gameplay features

**How to Add Screenshots:**
1. Take clear, annotated screenshots
2. Use descriptive filenames (e.g., `error-modules-missing.png`)
3. Place in appropriate `docs/images/` subdirectory
4. Reference in markdown: `![Description](docs/images/category/filename.png)`
5. Update docs/README.md checklist

**Creating Diagrams:**
- Use draw.io, Excalidraw, or PlantUML
- Save source files in `docs/diagrams/`
- Export PNG to `docs/images/architecture/`
- ASCII art diagrams are also great (see SETUP_GUIDE.md)

Even simple screenshots help new users tremendously!

## Pull Request Guidelines

### PR Title Format

```
[Type] Brief description

Types:
- Add: New feature
- Fix: Bug fix
- Refactor: Code refactoring
- Docs: Documentation update
- Test: Test additions/changes
```

### PR Description

Include:
- What changes were made
- Why the changes are needed
- How to test the changes
- Related issues (if any)

### Example PR

```markdown
## Description
Adds mining system for voxel blocks

## Changes
- New `AMiningTool` class
- Integration with `AVoxelChunk`
- Inventory system for mined resources

## Testing
1. Equip mining tool
2. Click on voxel block
3. Verify block is removed
4. Check resource added to inventory

## Related Issues
Closes #123
```

## Review Process

1. Automated checks run on PR
2. Maintainers review code
3. Address feedback if needed
4. Approved PRs are merged

## Community Guidelines

- Be respectful and constructive
- Help others when possible
- Follow the Code of Conduct
- Share knowledge and learnings

## Areas for Contribution

### High Priority
- Resource gathering system
- Crafting mechanics
- Combat system
- AI enemies
- UI improvements

### Medium Priority
- Additional building types
- More ship components
- Enhanced world generation
- Visual effects
- Sound design

### Always Welcome
- Bug fixes
- Performance improvements
- Documentation improvements
- Test coverage
- Code cleanup

## Getting Help

- Review [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for common build and setup issues
- Open an issue for questions
- Join community Discord (if available)
- Check existing documentation
- Review example code

## License

By contributing, you agree that your contributions will be licensed under the project's license.

## Recognition

Contributors will be recognized in:
- CONTRIBUTORS.md file
- Release notes
- Project credits

Thank you for contributing! 🎮
