# Build Instructions for Visual Studio 2022

This document provides step-by-step instructions for building the OpenSSL Certificate Manager application with Visual Studio 2022.

## Prerequisites Checklist

Before you begin, ensure you have:

- [ ] **Windows 10/11** operating system
- [ ] **Visual Studio 2022** installed (any edition: Community, Professional, or Enterprise)
- [ ] **Desktop development with C++** workload installed in Visual Studio
- [ ] **OpenSSL** installed on your system

## Step 1: Install Visual Studio 2022

If you don't have Visual Studio 2022 installed:

1. Download from: https://visualstudio.microsoft.com/downloads/
2. Run the installer
3. Select the **"Desktop development with C++"** workload
4. Ensure the following components are selected:
   - MSVC v143 - VS 2022 C++ x64/x86 build tools
   - Windows 10 SDK (or Windows 11 SDK)
   - C++ CMake tools for Windows (optional but recommended)
5. Complete the installation

## Step 2: Install OpenSSL

### Method A: Using Pre-built Binaries (Recommended for beginners)

1. **Download OpenSSL:**
   - Visit: https://slproweb.com/products/Win32OpenSSL.html
   - Download **Win64 OpenSSL v3.x.x** (not the "Light" version)
   - Example: `Win64OpenSSL-3_1_4.exe`

2. **Install OpenSSL:**
   - Run the downloaded installer
   - When prompted for installation location, use default: `C:\Program Files\OpenSSL-Win64`
   - When asked "Copy OpenSSL DLLs to:", select **"The OpenSSL binaries (/bin) directory"**
   - Complete the installation

3. **Set Environment Variable:**
   - Open Command Prompt as Administrator
   - Run:
     ```cmd
     setx OPENSSL_ROOT_DIR "C:\Program Files\OpenSSL-Win64" /M
     ```
   - Alternative: Set via System Properties:
     - Right-click "This PC" → Properties → Advanced system settings
     - Click "Environment Variables"
     - Under "System variables", click "New"
     - Variable name: `OPENSSL_ROOT_DIR`
     - Variable value: `C:\Program Files\OpenSSL-Win64`
     - Click OK

4. **Add to PATH (Required for running the application):**
   ```cmd
   setx PATH "%PATH%;C:\Program Files\OpenSSL-Win64\bin" /M
   ```

5. **Verify Installation:**
   - Open a new Command Prompt
   - Run: `openssl version`
   - You should see: `OpenSSL 3.x.x ...`

### Method B: Using vcpkg (Recommended for advanced users)

1. **Install vcpkg:**
   ```cmd
   cd C:\
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   bootstrap-vcpkg.bat
   ```

2. **Install OpenSSL:**
   ```cmd
   vcpkg install openssl:x64-windows
   ```

3. **Integrate with Visual Studio:**
   ```cmd
   vcpkg integrate install
   ```

4. **Set Environment Variable:**
   ```cmd
   setx OPENSSL_ROOT_DIR "C:\vcpkg\installed\x64-windows"
   ```

**Note:** After setting environment variables, you MUST restart Visual Studio for changes to take effect.

## Step 3: Clone the Repository

1. **Using Git:**
   ```cmd
   cd C:\Projects
   git clone https://github.com/benkemt/OpenSSL-Certificate-Managament.git
   cd OpenSSL-Certificate-Managament
   ```

2. **Or download ZIP:**
   - Visit: https://github.com/benkemt/OpenSSL-Certificate-Managament
   - Click "Code" → "Download ZIP"
   - Extract to your desired location

## Step 4: Open the Project in Visual Studio 2022

1. **Launch Visual Studio 2022**

2. **Open the solution:**
   - Method 1: Double-click `CertManager.sln` in File Explorer
   - Method 2: In Visual Studio:
     - File → Open → Project/Solution
     - Navigate to the repository folder
     - Select `CertManager.sln`
     - Click "Open"

3. **Wait for Visual Studio to load:**
   - Visual Studio will parse the project files
   - Check the Output window for any errors

## Step 5: Configure Build Settings

1. **Select Configuration:**
   - Top toolbar: Select **"Release"** or **"Debug"**
   - Debug: Includes debugging symbols, slower
   - Release: Optimized, faster execution

2. **Select Platform:**
   - Top toolbar: Select **"x64"**
   - Must be x64 to match OpenSSL installation

3. **Verify Project Properties (Optional):**
   - Right-click the CertManager project in Solution Explorer
   - Select "Properties"
   - Check:
     - Configuration Properties → General → Platform Toolset: `v143`
     - C/C++ → General → Additional Include Directories: Should show `$(OPENSSL_ROOT_DIR)\include`
     - Linker → General → Additional Library Directories: Should show `$(OPENSSL_ROOT_DIR)\lib`

## Step 6: Build the Project

### Method A: Using Visual Studio UI

1. **Clean the solution (optional but recommended):**
   - Menu: Build → Clean Solution

2. **Build the solution:**
   - Menu: Build → Build Solution
   - Or press: `Ctrl+Shift+B`

3. **Watch the Output window:**
   - View → Output (if not visible)
   - You should see compilation progress
   - Final line should be: `========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========`

### Method B: Using Developer Command Prompt

1. **Open Developer Command Prompt:**
   - Start Menu → Visual Studio 2022 → Developer Command Prompt for VS 2022

2. **Navigate to project:**
   ```cmd
   cd C:\Projects\OpenSSL-Certificate-Managament
   ```

3. **Build:**
   ```cmd
   msbuild CertManager.sln /p:Configuration=Release /p:Platform=x64
   ```

## Step 7: Run the Application

### From Visual Studio:

1. **Without debugging (recommended for first run):**
   - Press `Ctrl+F5`
   - Or: Debug → Start Without Debugging

2. **With debugging:**
   - Press `F5`
   - Or: Debug → Start Debugging

### From Command Line:

1. **Navigate to output directory:**
   ```cmd
   cd x64\Release
   ```

2. **Run the executable:**
   ```cmd
   CertManager.exe
   ```

## Troubleshooting

### Error: "Cannot open include file: 'openssl/x509.h'"

**Cause:** Visual Studio cannot find OpenSSL headers

**Solutions:**
1. Verify OpenSSL is installed
2. Check `OPENSSL_ROOT_DIR` environment variable:
   ```cmd
   echo %OPENSSL_ROOT_DIR%
   ```
3. Restart Visual Studio after setting environment variables
4. Manually set include path in project properties:
   - Right-click project → Properties
   - C/C++ → General → Additional Include Directories
   - Add: `C:\Program Files\OpenSSL-Win64\include`

### Error: "Cannot open file 'libssl.lib'"

**Cause:** Visual Studio cannot find OpenSSL libraries

**Solutions:**
1. Check library directory exists: `C:\Program Files\OpenSSL-Win64\lib`
2. Manually set library path in project properties:
   - Right-click project → Properties
   - Linker → General → Additional Library Directories
   - Add: `C:\Program Files\OpenSSL-Win64\lib`

### Error: "The code execution cannot proceed because libssl-3-x64.dll was not found"

**Cause:** OpenSSL DLLs are not in PATH or application directory

**Solutions:**
1. **Add to system PATH (Recommended):**
   ```cmd
   setx PATH "%PATH%;C:\Program Files\OpenSSL-Win64\bin" /M
   ```
   Then restart Command Prompt/Visual Studio

2. **Copy DLLs to executable directory:**
   - Copy from: `C:\Program Files\OpenSSL-Win64\bin\`
   - Files: `libssl-3-x64.dll`, `libcrypto-3-x64.dll`
   - To: `x64\Release\` (or `x64\Debug\`)

### Error: "MSB8036: The Windows SDK version X was not found"

**Cause:** Missing Windows SDK

**Solutions:**
1. Open Visual Studio Installer
2. Modify your Visual Studio 2022 installation
3. Under "Individual components", search for "Windows SDK"
4. Install the latest Windows 10 or 11 SDK
5. Restart Visual Studio

### Build Succeeds but Application Crashes

**Solutions:**
1. Ensure you're building for x64 (not x86)
2. Verify OpenSSL DLLs match architecture (64-bit)
3. Run from Visual Studio with debugger to see crash details
4. Check Output window for any error messages

## Verification

After successful build and run, you should see:

```
OpenSSL Certificate Management System
Version 1.0
======================================

========================================
  OpenSSL Certificate Manager
========================================
1. Initialize PKI Directory Structure
2. Create Self-Signed Certificate
3. List Certificates
4. Display Certificate Info
5. Move Certificate
6. Exit
========================================
Enter your choice:
```

If you see this menu, congratulations! The application is working correctly.

## Next Steps

- Read [USAGE.md](USAGE.md) for detailed usage examples
- Try creating a test certificate (option 2)
- Explore the certificate management features
- Review the source code to understand the implementation

## Getting Help

If you encounter issues not covered here:

1. Check the main [README.md](README.md) for general information
2. Review OpenSSL documentation: https://www.openssl.org/docs/
3. Check Visual Studio documentation: https://docs.microsoft.com/visualstudio/
4. Open an issue on GitHub with:
   - Your Visual Studio version
   - Your OpenSSL version (run `openssl version`)
   - The complete error message
   - Steps to reproduce the problem

## Additional Resources

- **OpenSSL Documentation:** https://www.openssl.org/docs/
- **Visual Studio 2022 Docs:** https://docs.microsoft.com/visualstudio/
- **C++ Language Reference:** https://docs.microsoft.com/cpp/
- **MSBuild Reference:** https://docs.microsoft.com/visualstudio/msbuild/

---

**Happy Building!** 🎉
