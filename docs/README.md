# Documentation Assets

This directory is for storing images, diagrams, and other visual assets used in documentation.

## Directory Structure

```
docs/
├── images/
│   ├── setup/           # Setup and installation screenshots
│   ├── gameplay/        # Gameplay screenshots
│   ├── architecture/    # Architecture diagrams
│   ├── ui/              # UI screenshots
│   └── troubleshooting/ # Error message screenshots
├── diagrams/            # Editable diagram source files
└── README.md
```

## Adding Screenshots

To add screenshots to the documentation:

### 1. Take the Screenshot

**For Windows:**
- Use Windows Snipping Tool (Win + Shift + S)
- Use Windows Game Bar (Win + G) for in-game screenshots
- Or use third-party tools like ShareX or Greenshot

**For Mac:**
- Cmd + Shift + 4 for selection capture
- Cmd + Shift + 3 for full screen

### 2. Name the File Descriptively

Use descriptive names that explain what the image shows:

**Good names:**
- `generate-project-files-context-menu.png`
- `visual-studio-build-configuration.png`
- `unreal-editor-first-open.png`
- `error-modules-missing-dialog.png`

**Bad names:**
- `screenshot1.png`
- `image.png`
- `pic.png`

### 3. Save to Appropriate Directory

```
docs/images/setup/          - Project setup screenshots
docs/images/troubleshooting/ - Error messages and solutions
docs/images/gameplay/       - In-game features
docs/images/architecture/   - System diagrams
docs/images/ui/             - User interface screenshots
```

### 4. Reference in Documentation

Use relative paths in markdown:

```markdown
## Step 2: Generate Project Files

Right-click on `VoxelSurvival.uproject`:

![Generate Project Files](docs/images/setup/generate-project-files-context-menu.png)

The context menu will appear with the option to generate project files.
```

### 5. Add Alt Text

Always include descriptive alt text for accessibility:

```markdown
![Visual Studio build configuration showing Development Editor and Win64](docs/images/setup/vs-build-config.png)
```

## Image Guidelines

### File Format
- **Screenshots:** PNG (lossless, good for UI)
- **Photos:** JPG (smaller file size)
- **Diagrams:** SVG (scalable) or PNG
- **Icons:** PNG with transparency

### File Size
- Keep images under 1MB when possible
- Compress large screenshots using tools like:
  - TinyPNG (https://tinypng.com/)
  - ImageOptim (Mac)
  - PNGGauntlet (Windows)

### Resolution
- Screenshots: 1920x1080 or smaller
- Crop to show only relevant parts
- Use highlights/arrows to draw attention (see tools below)

### Image Editing Tools

**Free Tools for Annotations:**
- **ShareX** (Windows) - Built-in annotation
- **Greenshot** (Windows/Mac) - Built-in annotation
- **Skitch** (Mac) - Evernote annotation tool
- **GIMP** - Full image editor
- **Paint.NET** - Simple Windows editor

**What to Annotate:**
- Add red arrows to point to important UI elements
- Add numbered circles for step-by-step instructions
- Add text boxes to explain what to click
- Highlight important areas with rectangles or circles

## Creating Diagrams

### Tools for Diagrams

**Free Online:**
- **draw.io** (https://app.diagrams.net/) - Best for flowcharts
- **Excalidraw** (https://excalidraw.com/) - Hand-drawn style
- **ASCII Art** - Text-based diagrams (already used in docs)

**Installed Software:**
- **yEd** - Graph editor (free)
- **PlantUML** - Code-based diagrams
- **Mermaid** - Markdown-based diagrams

### Diagram Types Needed

1. **Architecture Diagrams**
   - Class hierarchies
   - System interactions
   - Module dependencies

2. **Flowcharts**
   - Build process
   - Setup steps
   - Decision trees for troubleshooting

3. **Sequence Diagrams**
   - Multiplayer communication
   - System initialization
   - Event flows

## Examples Needed

### High Priority Screenshots

Setup Guide needs these screenshots:

1. **Visual Studio Installation**
   - [ ] VS Installer with required workloads checked
   - [ ] C++ workload details
   - [ ] Game development workload details

2. **Project Setup**
   - [ ] Context menu showing "Generate Visual Studio project files"
   - [ ] Generation progress/success message
   - [ ] File explorer showing VoxelSurvival.sln after generation

3. **Visual Studio Build**
   - [ ] Build configuration dropdown (Development Editor)
   - [ ] Platform dropdown (Win64)
   - [ ] Build menu with "Build Solution" highlighted
   - [ ] Successful build output window
   - [ ] Failed build with errors (for troubleshooting)

4. **Common Errors**
   - [ ] "Modules missing or built with different engine version" dialog
   - [ ] "Could not be compiled" error
   - [ ] "Build tools not found" error
   - [ ] "Platform Win64 not found" error

5. **Unreal Engine**
   - [ ] Project opening with shader compilation progress
   - [ ] Editor main window after successful open
   - [ ] Plugin manager showing ExampleMod

6. **Troubleshooting**
   - [ ] Directory structure before/after build
   - [ ] Where to find log files
   - [ ] Visual Studio Error List window

### Medium Priority

7. **Gameplay Features**
   - [ ] Voxel world in editor viewport
   - [ ] Character with wireframe
   - [ ] Territory system visualization
   - [ ] Building placement
   - [ ] Ship construction

8. **Modding**
   - [ ] Plugin creation dialog
   - [ ] ExampleMod in content browser
   - [ ] Creating Blueprint from C++ class

## Contributing Screenshots

If you're contributing screenshots:

1. Follow the naming conventions above
2. Place in appropriate `docs/images/` subdirectory
3. Update this README with what you added
4. Reference the images in the relevant documentation files
5. Ensure screenshots don't contain:
   - Personal information
   - Copyrighted content
   - Inappropriate content

## Placeholders

Until actual screenshots are added, the documentation uses:
- ✅ ASCII art diagrams
- ✅ Text-based visual representations
- ✅ Detailed descriptions in lieu of images
- ✅ Unicode box-drawing characters

These provide good guidance but actual screenshots would be even better!

## Future Enhancements

Planned documentation improvements:

- [ ] Add actual screenshots for all setup steps
- [ ] Create animated GIFs for complex workflows
- [ ] Add video tutorials (link to YouTube)
- [ ] Create interactive diagrams
- [ ] Add dark mode versions of screenshots
- [ ] Create printable PDF guides

## Tools for Creating Animated GIFs

For showing workflows:
- **ScreenToGif** (Windows) - Free, easy to use
- **LICEcap** (Windows/Mac) - Simple, lightweight
- **Gifox** (Mac) - Professional quality

Keep GIFs:
- Under 5MB
- 10-30 seconds max
- At 10-15 fps (smooth but small)
- Cropped to relevant area only

## Questions?

If you're unsure about:
- Where to place an image
- What to screenshot
- How to annotate

Open an issue and ask! The maintainers will help guide you.

---

**Last Updated:** October 2025
