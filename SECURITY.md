# Security Policy

## Reporting Security Issues

If you discover a security vulnerability in this project, please report it by opening an issue on GitHub. We take all security concerns seriously.

## Known Security Advisories

### Magick.NET-Q16-HDRI-AnyCPU Vulnerability (GHSA-qp29-wxp5-wh82)

**Status:** Not Applicable to This Repository

**Summary:**
This repository has received warnings about Magick.NET-Q16-HDRI-AnyCPU version 14.7.0 having a high severity vulnerability (CVE-2025-55154) related to integer overflows in MNG magnification.

**Analysis:**
After thorough investigation of the repository:
- This is an Unreal Engine 5.6.1 C++ project
- No NuGet packages are used in this project
- No .NET dependencies (csproj, packages.config, etc.) exist in the repository
- No references to Magick.NET or ImageMagick libraries were found in the codebase
- The only C# files are Unreal Build Tool configuration files (.Build.cs) which do not reference image processing libraries

**Possible Causes of the Warning:**
1. **False Positive**: GitHub's dependency scanning may have incorrectly associated this project with the vulnerability
2. **System-Level Dependency**: Unreal Engine itself or Visual Studio Build Tools may use ImageMagick as a transitive dependency for asset processing
3. **Cached Scanning Data**: Old scanning data that hasn't been refreshed

**Resolution:**
Since this package is not directly used by this project:
1. The vulnerability does not affect the game code or runtime
2. No action is required in this repository
3. If the warning persists, it should be reported to GitHub Support or dismissed as a false positive

**For Users Experiencing the Warning:**
If you're seeing Magick.NET vulnerability warnings when building this project:
1. Check if the warning is from GitHub Dependabot/Security tab - if so, it's likely a false positive
2. Verify your local development environment - the package may be installed globally on your system
3. Update your Visual Studio and Unreal Engine installations to their latest versions
4. If building through CI/CD, ensure no extra NuGet packages are being restored

### Visual Studio 2022 Build Tools (v143)

**Issue:** Missing v143 build tools warning

**Status:** Documented

This is a common setup issue, not a security vulnerability. See:
- [TROUBLESHOOTING.md - Visual Studio 2022 Build Tools Not Found](TROUBLESHOOTING.md#visual-studio-2022-build-tools-not-found)
- [SETUP_GUIDE.md](SETUP_GUIDE.md) for complete installation instructions

**Solution:**
1. Install Visual Studio 2022 with required workloads:
   - Desktop development with C++
   - Game development with C++
   - .NET desktop development (for Unreal Build Tool)
   - Windows 10/11 SDK

2. Generate Visual Studio project files:
   - Right-click `VoxelSurvival.uproject`
   - Select "Generate Visual Studio project files"

## Supported Versions

| Unreal Engine Version | Supported          |
| --------------------- | ------------------ |
| 5.6.x                 | :white_check_mark: |
| < 5.6                 | :x:                |

## Dependencies

This project uses only Unreal Engine native modules:
- Core
- CoreUObject
- Engine
- InputCore
- OnlineSubsystem
- OnlineSubsystemUtils
- Networking
- Sockets
- UMG
- Slate
- SlateCore
- ProceduralMeshComponent

No external NuGet packages, npm packages, or third-party libraries are included.

## Security Best Practices

When contributing to or modifying this project:
1. Do not commit secrets, API keys, or credentials to the repository
2. Keep Unreal Engine and Visual Studio updated to the latest stable versions
3. Review changes to .Build.cs files carefully to avoid introducing unsafe dependencies
4. Test multiplayer code for potential exploits (command injection, buffer overflows, etc.)
5. Validate user input in Blueprint and C++ code

## Contact

For security concerns, please open an issue on GitHub: https://github.com/shifty81/Unreal-Engine-Game-Dev/issues

---

**Last Updated:** October 2025
