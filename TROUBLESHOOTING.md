# Troubleshooting Guide

This guide covers common issues and their solutions when building and running Voxel Survival.

## Table of Contents
- [Build Issues](#build-issues)
- [Visual Studio Issues](#visual-studio-issues)
- [Unreal Engine Issues](#unreal-engine-issues)
- [Performance Issues](#performance-issues)

---

## Build Issues

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
cd /path/to/Unreal-Engine-Game-Dev
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

**Last Updated:** November 2025
