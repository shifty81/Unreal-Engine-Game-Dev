# Visual Setup Guide - Voxel Survival

This comprehensive guide walks you through setting up Voxel Survival with detailed step-by-step instructions.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Step-by-Step Setup](#step-by-step-setup)
3. [Common Issues](#common-issues)
4. [Visual Directory Structure](#visual-directory-structure)

---

## Prerequisites

Before you begin, ensure you have:

```
✓ Unreal Engine 5.6.1 (or compatible 5.6.x)
✓ Visual Studio 2022 with C++ workloads
✓ 16GB+ RAM (32GB recommended)
✓ 50GB+ free disk space
```

### Visual Studio 2022 Required Components

When installing Visual Studio 2022, you **MUST** select these workloads:

```
┌─────────────────────────────────────────────────┐
│ Visual Studio Installer                        │
├─────────────────────────────────────────────────┤
│ ☑ Desktop development with C++                 │
│   ├─ MSVC v143 - VS 2022 C++ x64/x86 build     │
│   ├─ Windows 10/11 SDK (latest)                │
│   └─ C++ profiling tools                       │
│                                                 │
│ ☑ Game development with C++                    │
│   ├─ C++ profiling tools                       │
│   ├─ Windows 10/11 SDK                          │
│   └─ Unreal Engine installer                   │
│                                                 │
│ ☑ .NET desktop development                     │
│   └─ (Required for Unreal Build Tool)          │
└─────────────────────────────────────────────────┘
```

---

## Step-by-Step Setup

### Step 1: Clone the Repository

```bash
git clone https://github.com/shifty81/Unreal-Engine-Game-Dev.git
cd Unreal-Engine-Game-Dev
```

**What you should see:**
```
Unreal-Engine-Game-Dev/
├── Config/
├── Content/
├── Plugins/
│   └── ExampleMod/
├── Source/
│   └── VoxelSurvival/
├── VoxelSurvival.uproject  ← The main project file
├── README.md
└── [other files...]
```

---

### Step 2: Generate Visual Studio Project Files

**This is the MOST IMPORTANT STEP!** Most build errors occur because users skip this step.

#### Option A: Using Windows Explorer (Recommended)

```
┌──────────────────────────────────────────────────────┐
│ 1. Navigate to the project folder                    │
│                                                       │
│ 2. Find: VoxelSurvival.uproject                      │
│                                                       │
│ 3. Right-click on it                                 │
│                                                       │
│ 4. You should see a context menu:                    │
│    ┌──────────────────────────────────┐             │
│    │ Open                             │             │
│    │ Switch Unreal Engine version...  │             │
│    │ ► Generate Visual Studio project files ← Click │
│    │ Run                              │             │
│    │ Properties                       │             │
│    └──────────────────────────────────┘             │
│                                                       │
│ 5. Click "Generate Visual Studio project files"      │
│                                                       │
│ 6. Wait for the process to complete (30-60 seconds)  │
└──────────────────────────────────────────────────────┘
```

**Expected Output:**
```
Generating project files...
Running: C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat
Writing project files...
Formatting project files...
Project file generation completed successfully!
```

**What gets created:**
```
Unreal-Engine-Game-Dev/
├── VoxelSurvival.sln              ← NEW: Solution file
├── VoxelSurvival.vcxproj          ← NEW: Project file
├── VoxelSurvival.vcxproj.filters  ← NEW: Filters
├── Intermediate/                   ← NEW: Build artifacts
│   └── ProjectFiles/
└── [existing files...]
```

#### Option B: Using Command Line

If right-click doesn't show the option, use command line:

```batch
cd C:\path\to\Unreal-Engine-Game-Dev

REM Replace with your UE installation path
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" ^
  -projectfiles ^
  -project="%CD%\VoxelSurvival.uproject" ^
  -game ^
  -engine
```

---

### Step 3: Open and Build in Visual Studio

Now that project files are generated, let's build the project.

```
┌────────────────────────────────────────────────────┐
│ 1. Open VoxelSurvival.sln in Visual Studio 2022   │
│    (Double-click the .sln file)                    │
│                                                     │
│ 2. Set the configuration (top toolbar):            │
│    ┌────────────────────────────────┐             │
│    │ Configuration: [Development Editor ▼]        │
│    │ Platform:      [Win64 ▼]                     │
│    └────────────────────────────────┘             │
│                                                     │
│    ⚠ Make sure it says "Development Editor"       │
│       NOT "Development" or "Shipping"              │
│                                                     │
│ 3. Build the solution:                             │
│    Menu: Build → Build Solution                    │
│    Keyboard: Ctrl + Shift + B                      │
│                                                     │
│ 4. Wait for compilation (5-15 minutes)             │
└────────────────────────────────────────────────────┘
```

**Visual Studio Build Process:**
```
┌───────────────────────────────────────────────┐
│ Build started...                              │
│ 1>------ Build started: Project: UE5 ------   │
│ 2>------ Build started: VoxelSurvival ------  │
│    Compiling VoxelWorld.cpp...                │
│    Compiling VoxelChunk.cpp...                │
│    Compiling BuildingSystem.cpp...            │
│    Compiling [more files...]                  │
│ 3>------ Build started: ExampleMod -------    │
│    Compiling ExampleMod.cpp...                │
│    Compiling ExampleBuilding.cpp...           │
│                                               │
│ ========== Build: 3 succeeded, 0 failed =====│
└───────────────────────────────────────────────┘
```

**Expected Output:**
- ✅ Build succeeded
- ✅ 0 errors
- ⚠ Warnings are okay (common in Unreal projects)

**If build fails**, see the [Troubleshooting](#common-issues) section below.

---

### Step 4: Open in Unreal Engine

**ONLY AFTER** successful build in Step 3:

```
┌────────────────────────────────────────────────────┐
│ 1. Double-click VoxelSurvival.uproject            │
│                                                     │
│ 2. Unreal Engine 5.6.1 will start                  │
│                                                     │
│ 3. First launch takes 5-10 minutes:                │
│    ┌──────────────────────────────────┐           │
│    │ Compiling shaders...             │           │
│    │ Building meshes...               │           │
│    │ Loading assets...                │           │
│    └──────────────────────────────────┘           │
│                                                     │
│ 4. Editor opens successfully!                      │
└────────────────────────────────────────────────────┘
```

---

## Common Issues

### Issue 1: "Missing or Built with Different Engine Version" ❌

```
┌──────────────────────────────────────────────────────┐
│ ERROR: The following modules are missing or built    │
│        with a different engine version:              │
│                                                       │
│        • VoxelSurvival                               │
│        • ExampleMod                                  │
│                                                       │
│        Would you like to rebuild them now?           │
│        [Yes]  [No]                                   │
└──────────────────────────────────────────────────────┘
```

**Why this happens:**
- You skipped Step 2 (generating project files)
- You skipped Step 3 (building in Visual Studio)
- You tried to open .uproject directly without building first

**Solution:**

```
┌────────────────────────────────────────────────────┐
│ DO NOT click "Yes" on that dialog!                 │
│                                                     │
│ Instead, click "No" and follow these steps:        │
│                                                     │
│ 1. Close the error dialog                          │
│ 2. Go back to Step 2 above                         │
│ 3. Generate project files (right-click .uproject)  │
│ 4. Open VoxelSurvival.sln in Visual Studio         │
│ 5. Build the solution (Ctrl+Shift+B)               │
│ 6. Wait for successful build                       │
│ 7. NOW double-click VoxelSurvival.uproject         │
└────────────────────────────────────────────────────┘
```

**The correct order is:**
```
Clone Repo → Generate Project Files → Build in VS → Open in UE
    ↓              ↓                      ↓              ↓
  (Step 1)      (Step 2)              (Step 3)      (Step 4)
```

---

### Issue 2: "Could Not Be Compiled" After Clicking "Yes" ❌

If you already clicked "Yes" on the rebuild dialog:

```
┌──────────────────────────────────────────────────────┐
│ ERROR: VoxelSurvival could not be compiled.          │
│        Try rebuilding from source manually.           │
└──────────────────────────────────────────────────────┘
```

**Why this happens:**
The Unreal Editor's internal build system doesn't have the proper project files to work with.

**Solution - Clean and Rebuild:**

```batch
# 1. Close Unreal Editor if open

# 2. Delete generated files
cd C:\path\to\Unreal-Engine-Game-Dev
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved
rmdir /s /q .vs
del *.sln
del *.vcxproj*

# 3. Regenerate project files
# Right-click VoxelSurvival.uproject → Generate Visual Studio project files

# 4. Open VoxelSurvival.sln in Visual Studio

# 5. Build → Rebuild Solution

# 6. Wait for successful build

# 7. Now open VoxelSurvival.uproject
```

---

### Issue 3: "Build Tools for Visual Studio 2022 Cannot Be Found" ❌

```
The build tools for Visual Studio 2022 (v143) cannot be found.
```

**Solution:**
1. Verify Visual Studio 2022 is installed with C++ workloads (see Prerequisites)
   - ⚠️ **CRITICAL:** Must include **"MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)"**
   - Open Visual Studio Installer → Modify → Desktop development with C++ → Check right panel for v143
2. Regenerate project files (Step 2)
3. Restart Visual Studio

For detailed step-by-step instructions, see [TROUBLESHOOTING.md - Visual Studio 2022 Build Tools Not Found](TROUBLESHOOTING.md#visual-studio-2022-build-tools-not-found).

---

### Issue 4: Visual Studio Shows "Platform 'Win64' Not Found" ❌

**Solution:**
1. This often appears with the v143 error - ensure v143 build tools are installed first (see Issue 3)
2. Close Visual Studio
3. Delete: `.vs` folder, `*.sln`, `*.vcxproj*` files
4. Regenerate project files
5. Open the NEW `VoxelSurvival.sln` file in the project root (NOT from Intermediate folder)

For detailed instructions, see [TROUBLESHOOTING.md - Win64 Platform Cannot Be Found](TROUBLESHOOTING.md#win64-platform-cannot-be-found).

---

## Visual Directory Structure

### Before Building
```
Unreal-Engine-Game-Dev/
├── 📁 Config/
│   └── [Config files]
├── 📁 Content/
│   └── [Game assets]
├── 📁 Plugins/
│   └── 📁 ExampleMod/
│       ├── 📄 ExampleMod.uplugin
│       ├── 📁 Source/
│       └── 📄 README.md
├── 📁 Source/
│   ├── 📄 VoxelSurvivalTarget.cs
│   ├── 📄 VoxelSurvivalEditorTarget.cs
│   └── 📁 VoxelSurvival/
│       ├── 📄 VoxelSurvival.Build.cs
│       ├── 📄 VoxelSurvival.h/.cpp
│       └── [Other source files]
├── 📄 VoxelSurvival.uproject
├── 📄 README.md
└── 📄 TROUBLESHOOTING.md
```

### After Generating Project Files
```
Unreal-Engine-Game-Dev/
├── 📄 VoxelSurvival.sln ← NEW! Open this in VS
├── 📄 VoxelSurvival.vcxproj ← NEW!
├── 📄 VoxelSurvival.vcxproj.filters ← NEW!
├── 📁 Intermediate/ ← NEW!
│   └── ProjectFiles/
├── [Previous files remain...]
```

### After Building in Visual Studio
```
Unreal-Engine-Game-Dev/
├── 📁 Binaries/ ← NEW! Contains compiled DLLs
│   └── Win64/
│       ├── VoxelSurvivalEditor.exe
│       ├── VoxelSurvival.dll
│       └── ExampleMod.dll
├── 📁 Intermediate/ ← UPDATED with build artifacts
│   ├── Build/
│   └── ProjectFiles/
├── 📁 Saved/ ← NEW! Contains logs and cache
│   ├── Logs/
│   └── Config/
├── [Previous files remain...]
```

---

## Build Process Flow Chart

```
┌─────────────────┐
│  Clone Repo     │
└────────┬────────┘
         │
         ▼
┌─────────────────────────────┐
│  Generate Project Files     │
│  (Right-click .uproject)    │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐     ┌──────────────┐
│  Open in Visual Studio      │────▶│  Build Error?│
│  (Open .sln file)           │     └──────┬───────┘
└────────┬────────────────────┘            │
         │                                 │ Yes
         ▼                                 ▼
┌─────────────────────────────┐     ┌──────────────────────┐
│  Set Configuration:         │     │  See Troubleshooting │
│  Development Editor / Win64 │     └──────────────────────┘
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│  Build Solution             │
│  (Ctrl+Shift+B)             │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│  Wait for Build Success     │
│  (5-15 minutes)             │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│  Double-click .uproject     │
│  Opens in Unreal Engine     │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│  Wait for Shader Compile    │
│  (First launch: 5-10 min)   │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│  ✅ Ready to Use!           │
└─────────────────────────────┘
```

---

## Quick Reference

### When to Regenerate Project Files
- ✅ After cloning repository
- ✅ After adding new C++ classes
- ✅ After modifying .Build.cs files
- ✅ After updating to new Unreal Engine version
- ✅ When switching between different branches
- ✅ When getting "Platform not found" errors

### When to Clean and Rebuild
- ✅ After regenerating project files
- ✅ When getting compilation errors
- ✅ After switching engine versions
- ✅ When binary files are corrupted
- ✅ Before packaging for distribution

### What NOT to Do
- ❌ Don't open .uproject before building in Visual Studio
- ❌ Don't click "Yes" when asked to rebuild modules
- ❌ Don't skip project file generation
- ❌ Don't open files from Intermediate/ folder
- ❌ Don't commit Binaries/ or Intermediate/ to git

---

## Additional Resources

- **Main Documentation**: [GAME_DOCUMENTATION.md](GAME_DOCUMENTATION.md)
- **Troubleshooting**: [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
- **Modding Guide**: [MODDING_GUIDE.md](MODDING_GUIDE.md)
- **Quick Reference**: [QUICK_REFERENCE.md](QUICK_REFERENCE.md)

---

## Still Having Issues?

If you're still experiencing problems:

1. **Check the logs:**
   - Build logs: `Saved/Logs/UnrealBuildTool.log`
   - Runtime logs: `Saved/Logs/VoxelSurvival.log`

2. **Try a clean rebuild** (see Issue 2 above)

3. **Verify your setup:**
   ```batch
   # Check Visual Studio version
   "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" /?
   
   # Check Unreal Engine version
   "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" -version
   ```

4. **Report the issue:**
   - GitHub Issues: https://github.com/shifty81/Unreal-Engine-Game-Dev/issues
   - Include error messages and log files

---

**Last Updated:** October 2025
**For Unreal Engine:** 5.6.1
**For Visual Studio:** 2022
