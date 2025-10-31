# Troubleshooting Guide

This guide covers common issues and their solutions when building and running Voxel Survival.

## Table of Contents
- [Build Issues](#build-issues)
- [Visual Studio Issues](#visual-studio-issues)
- [Unreal Engine Issues](#unreal-engine-issues)
- [Performance Issues](#performance-issues)
- [Security Warnings](#security-warnings)

---

## Build Issues

### Missing or Built with Different Engine Version (MOST COMMON) ⚠️

**Error Message:**
```
The following modules are missing or built with a different engine version:

  VoxelSurvival
  ExampleMod

Would you like to rebuild them now?
```

Followed by (if you click "Yes"):
```
VoxelSurvival could not be compiled. Try rebuilding from source manually.
```

**Cause:**
This is the **most common error** and happens when:
1. You opened `VoxelSurvival.uproject` **before** building the project in Visual Studio
2. You haven't generated Visual Studio project files
3. The compiled module binaries don't exist or are out of date
4. You're trying to use the editor's "rebuild" feature without proper project files

**⚠️ DO NOT click "Yes" on the rebuild dialog!** The editor cannot rebuild properly without project files being generated first.

**Solution:**

**Step 1: Close the Error Dialog**
- Click **"No"** on the rebuild prompt
- Close Unreal Engine completely

**Step 2: Clean Previous Build Attempts**
```batch
cd C:\path\to\Unreal-Engine-Game-Dev

# Delete build artifacts
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved
rmdir /s /q .vs

# Delete old project files (if any)
del *.sln
del *.vcxproj*
```

**Step 3: Generate Visual Studio Project Files**
1. Navigate to your project folder in Windows Explorer
2. Right-click on `VoxelSurvival.uproject`
3. Select **"Generate Visual Studio project files"**
4. Wait for completion (30-60 seconds)
5. Verify that `VoxelSurvival.sln` now exists in the folder

**Step 4: Build in Visual Studio**
1. Open `VoxelSurvival.sln` in Visual Studio 2022
2. Set configuration to **"Development Editor"** (top toolbar)
3. Set platform to **"Win64"**
4. Build → Build Solution (or press Ctrl+Shift+B)
5. Wait for build to complete (5-15 minutes)
6. Verify: "Build succeeded" with 0 errors

**Step 5: Now Open in Unreal Engine**
- Double-click `VoxelSurvival.uproject`
- Editor should open successfully

**Visual Guide:**
See [SETUP_GUIDE.md](SETUP_GUIDE.md) for detailed visual step-by-step instructions.

**Remember:** The correct order is:
```
Clone → Generate Project Files → Build in Visual Studio → Open in Unreal Engine
```

---

### Visual Studio 2022 Build Tools Not Found

**Error Message:**
```
The build tools for Visual Studio 2022 (v143) cannot be found. Install Visual Studio 2022 (v143) to build using the Visual Studio 2022 (v143) build tools.
```

**Cause:**
This error occurs when Visual Studio project files haven't been generated for your local environment, or when Visual Studio 2022 with the required components is not properly installed.

**Solution:**

#### Step 1: Verify Visual Studio 2022 Installation

1. Ensure Visual Studio 2022 is installed with the following components:
   - **Desktop development with C++** workload
   - **Game development with C++** workload
   - **.NET desktop development** (for Unreal Build Tool)
   - **Windows 10/11 SDK** (latest version)

To modify your installation:
- Open **Visual Studio Installer**
- Click **Modify** on Visual Studio 2022
- Select the required workloads
- Click **Modify** to install

#### Step 2: Generate Visual Studio Project Files

**Method 1: Using Windows Explorer (Recommended)**
1. Navigate to your project folder
2. Right-click on `VoxelSurvival.uproject`
3. Select **"Generate Visual Studio project files"**
4. Wait for the process to complete
5. Open the newly generated `VoxelSurvival.sln`

**Method 2: Using Command Line**
```batch
cd C:\path\to\Unreal-Engine-Game-Dev
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%CD%\VoxelSurvival.uproject" -game -engine
```

**Method 3: Using Unreal Engine Editor**
1. Open Unreal Engine 5.6.1
2. Open `VoxelSurvival.uproject`
3. Go to **File → Refresh Visual Studio Project**

---

### Win64 Platform Cannot Be Found

**Error Message:**
```
Platform 'Win64' referenced in the project file 'UE5' cannot be found.
```

**Cause:**
The Visual Studio project files are missing, corrupted, or outdated.

**Solution:**

1. **Delete existing project files** (if any):
   ```batch
   del /f /s /q *.sln
   del /f /s /q *.vcxproj
   del /f /s /q *.vcxproj.filters
   rmdir /s /q Intermediate
   rmdir /s /q Binaries
   ```

2. **Regenerate project files** using one of the methods above

3. **Verify the solution file exists**:
   - Look for `VoxelSurvival.sln` in the project root
   - If missing, regeneration failed - check the output logs

4. **Open the correct solution file**:
   - Open `VoxelSurvival.sln` (not any files in Intermediate folder)
   - Set build configuration to **"Development Editor"**
   - Select platform **"Win64"**

---

### Missing Unreal Engine Association

**Error Message:**
```
The project file is associated with Unreal Engine 5.6, which does not appear to be installed.
```

**Cause:**
Unreal Engine 5.6.1 is not installed, or the engine association is incorrect.

**Solution:**

1. **Install Unreal Engine 5.6.1**:
   - Open Epic Games Launcher
   - Go to **Unreal Engine → Library**
   - Install version **5.6.1** (or newer 5.6.x)

2. **Update engine association**:
   - Right-click `VoxelSurvival.uproject`
   - Select **"Switch Unreal Engine version..."**
   - Choose your installed UE 5.6.x version

3. **Manual association** (if right-click option is unavailable):
   - Edit `VoxelSurvival.uproject`
   - Update line: `"EngineAssociation": "5.6"` to match your version

---

### Build Fails with Compilation Errors

**Symptoms:**
- Hundreds of compiler errors
- Missing header files
- Unresolved external symbols

**Solutions:**

1. **Clean the project**:
   ```batch
   rmdir /s /q Intermediate
   rmdir /s /q Binaries
   rmdir /s /q Saved
   ```

2. **Regenerate project files** (see above)

3. **Rebuild solution**:
   - In Visual Studio: **Build → Rebuild Solution**
   - Or use command line:
     ```batch
     "C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" VoxelSurvivalEditor Win64 Development "%CD%\VoxelSurvival.uproject" -WaitMutex
     ```

4. **Check for missing dependencies**:
   - Ensure all plugins referenced in `.uproject` are available
   - Verify `ProceduralMeshComponent` plugin is enabled

---

## Visual Studio Issues

### Visual Studio Crashes When Opening Solution

**Solutions:**
1. Clear Visual Studio cache:
   ```batch
   rmdir /s /q .vs
   del *.suo
   del *.user
   ```

2. Reset Visual Studio settings:
   - **Tools → Import and Export Settings → Reset all settings**

3. Disable Visual Studio extensions that may conflict

---

### IntelliSense Not Working

**Solutions:**
1. Delete IntelliSense database:
   ```batch
   rmdir /s /q .vs
   del *.VC.db
   ```

2. Regenerate project files

3. **Tools → Options → Text Editor → C/C++ → Advanced**:
   - Set **"Disable IntelliSense"** to **False**
   - Enable **"Rescan Solution"**

---

## Unreal Engine Issues

### Editor Fails to Open Project

**Solutions:**

1. **Check engine version compatibility**:
   - Ensure UE 5.6.1 or compatible version is installed

2. **Verify project integrity**:
   - Check that `VoxelSurvival.uproject` is valid JSON
   - Ensure all referenced plugins exist

3. **Open with specific engine version**:
   ```batch
   "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "%CD%\VoxelSurvival.uproject"
   ```

4. **Check logs**:
   - View `Saved/Logs/VoxelSurvival.log` for error details

---

### Shader Compilation Errors

**Solutions:**

1. Delete shader cache:
   ```batch
   rmdir /s /q Saved\ShaderDebugInfo
   rmdir /s /q DerivedDataCache
   ```

2. Restart the editor to rebuild shaders

3. Ensure graphics drivers are up to date

---

### Missing Content or Assets

**Cause:**
Some assets might not be committed to the repository.

**Solution:**

1. Check `.gitignore` to ensure content is not excluded
2. If using Git LFS, ensure it's properly set up:
   ```bash
   git lfs install
   git lfs pull
   ```

---

## Performance Issues

### Slow Editor Performance

**Solutions:**

1. **Reduce voxel chunk render distance**:
   - Open project settings
   - Lower chunk generation distance

2. **Disable unnecessary features**:
   - Real-time rendering
   - Lumen/Nanite (if not needed)

3. **Increase system resources**:
   - Close other applications
   - Ensure 16GB+ RAM available

---

### Multiplayer Lag or Connection Issues

**Solutions:**

1. **Check firewall settings**:
   - Allow UE5 Editor through Windows Firewall
   - Forward required ports (default: 7777)

2. **Test locally first**:
   - Use Play As Listen Server in editor
   - Test with localhost before network play

3. **Check network replication settings**:
   - Verify NetUpdateFrequency on replicated actors
   - Optimize replication conditions

---

## Security Warnings

### Magick.NET-Q16-HDRI-AnyCPU Vulnerability Warning

**Warning Message:**
```
Package 'Magick.NET-Q16-HDRI-AnyCPU' 14.7.0 has a known high severity vulnerability
https://github.com/advisories/GHSA-qp29-wxp5-wh82
```

**Status:** This is a **false positive** for this repository.

**Explanation:**
This Unreal Engine project does not use Magick.NET or any NuGet packages. This warning may appear due to:

1. **GitHub Dependabot False Positive**: GitHub's automated scanning incorrectly associates this repository with the vulnerability
2. **System-Level Dependency**: Unreal Engine or Visual Studio Build Tools may use ImageMagick internally for asset processing, but this doesn't affect your game code
3. **Cached Scanning Results**: Outdated security scan data

**What This Means:**
- Your game code is **NOT affected** by this vulnerability
- The project does not directly or indirectly reference Magick.NET
- This warning can be safely ignored for this project

**To Verify:**
```bash
# Search for any Magick.NET references (should return nothing)
grep -r "Magick.NET" . --include="*.cs" --include="*.csproj" --include="*.config"

# Check for NuGet packages (should return nothing)
find . -name "packages.config" -o -name "*.csproj"
```

**Action Items:**
1. ✅ **No code changes required** - this package is not used in the project
2. 📋 If the warning appears on GitHub, report it as a false positive
3. 📚 See [SECURITY.md](SECURITY.md) for complete analysis

**Related Issue:**
If you're seeing this warning alongside "missing v143" errors, these are separate issues. The v143 issue is a build configuration problem (see below), not a security concern.

---

### False Dependency Warnings in General

If you receive other security warnings for packages not used in this project:

1. **Verify the package exists**: Search the codebase for references
2. **Check .gitignore**: Ensure build artifacts aren't committed
3. **Review GitHub Security tab**: Dismiss false positives
4. **Update scanning tools**: Old tools may have incorrect data

This is a pure C++ Unreal Engine project with no external package dependencies beyond Unreal Engine's native modules.

---

## Still Having Issues?

If your problem isn't covered here:

1. **Check the logs**:
   - `Saved/Logs/VoxelSurvival.log` - Runtime logs
   - `Saved/Logs/UnrealBuildTool.log` - Build logs

2. **Search existing issues**:
   - Visit: https://github.com/shifty81/Unreal-Engine-Game-Dev/issues

3. **Create a new issue**:
   - Include error messages
   - Attach relevant log files
   - Describe steps to reproduce
   - Mention your environment (OS, UE version, VS version)

4. **Community resources**:
   - [Unreal Engine Documentation](https://docs.unrealengine.com/)
   - [Unreal Engine Forums](https://forums.unrealengine.com/)
   - [Unreal Slackers Discord](https://unrealslackers.org/)

---

## Quick Reference Commands

### Regenerate Project Files
```batch
# Right-click VoxelSurvival.uproject → Generate Visual Studio project files
```

### Clean Build
```batch
rmdir /s /q Intermediate Binaries Saved .vs
# Then regenerate project files
```

### Build from Command Line
```batch
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" VoxelSurvivalEditor Win64 Development "%CD%\VoxelSurvival.uproject" -WaitMutex
```

### Open in Editor
```batch
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "%CD%\VoxelSurvival.uproject"
```

---

**Last Updated:** October 2025
