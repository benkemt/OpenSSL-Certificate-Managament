# OpenSSL Certificate Management

A C++ console application for managing SSL/TLS certificates using OpenSSL on Windows. Built with Visual Studio 2022.

## Features

- Create self-signed X.509 certificates
- Manage certificates in an organized PKI directory structure
- Display certificate information
- Move certificates between different trust categories
- List certificates by category

## PKI Directory Structure

The application maintains certificates in the following organized structure:

```
pki/
├── own/
│   ├── certs/     (Your own certificates)
│   └── private/   (Your private keys)
├── trusted/       (Trusted certificates)
├── rejected/      (Rejected certificates)
├── issuers/       (Issuer/CA certificates)
└── crl/           (Certificate Revocation Lists)
```

## Prerequisites

### Required Software
- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - Desktop development with C++ workload
  - Windows 10 SDK
- **OpenSSL** (version 1.1.1 or 3.x)

### Installing OpenSSL on Windows

#### Option 1: Using Pre-built Binaries (Recommended)
1. Download OpenSSL for Windows from [Shining Light Productions](https://slproweb.com/products/Win32OpenSSL.html)
2. Install the Win64 OpenSSL v3.x version
3. During installation, note the installation directory (default: `C:\Program Files\OpenSSL-Win64`)

#### Option 2: Using vcpkg
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install openssl:x64-windows
vcpkg integrate install
```

## Building the Project

### Setting up Environment Variables

Set the `OPENSSL_ROOT_DIR` environment variable to point to your OpenSSL installation:

```cmd
setx OPENSSL_ROOT_DIR "C:\Program Files\OpenSSL-Win64"
```

*Note: Restart Visual Studio after setting environment variables.*

### Build Instructions

1. **Clone the repository:**
   ```cmd
   git clone https://github.com/benkemt/OpenSSL-Certificate-Managament.git
   cd OpenSSL-Certificate-Managament
   ```

2. **Open the solution:**
   - Double-click `CertManager.sln` or
   - Open Visual Studio 2022 → File → Open → Project/Solution → Select `CertManager.sln`

3. **Configure the project:**
   - Select the build configuration: `Debug` or `Release`
   - Select the platform: `x64`

4. **Build:**
   - Press `Ctrl+Shift+B` or
   - Menu: Build → Build Solution

5. **Run:**
   - Press `F5` (Debug mode) or `Ctrl+F5` (Run without debugging)

### Alternative: Command Line Build with MSBuild

```cmd
# Open Visual Studio Developer Command Prompt for VS 2022
cd OpenSSL-Certificate-Managament
msbuild CertManager.sln /p:Configuration=Release /p:Platform=x64
```

### Alternative: CMake Build (Cross-Platform)

For those who prefer CMake or want to build on other platforms:

```cmd
# Create build directory
mkdir build
cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# Run
.\bin\Release\CertManager.exe  # Windows
# or
./bin/CertManager              # Linux/macOS
```

**Note:** CMake build requires OpenSSL to be findable by CMake's `find_package()`. On Windows, ensure `OPENSSL_ROOT_DIR` is set.

## Usage

### Running the Application

After building, run the executable:
```cmd
.\x64\Release\CertManager.exe
```

### Menu Options

```
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
```

### Example: Creating a Certificate

1. Select option `1` to initialize the PKI directory structure
2. Select option `2` to create a self-signed certificate
3. Enter the required information:
   - **Common Name:** example.com
   - **Organization:** My Company
   - **Country Code:** US
   - **Validity (days):** 365

The application will:
- Generate a 2048-bit RSA key pair
- Create a self-signed X.509 certificate
- Save the certificate to `pki/own/certs/`
- Save the private key to `pki/own/private/`

### Example: Managing Certificates

**List certificates:**
- Select option `3`
- Choose the directory category to list

**View certificate details:**
- Select option `4`
- Choose the directory and certificate file
- View subject, issuer, serial number, and validity dates

**Move certificate to trusted:**
- Select option `5`
- Choose source directory and certificate
- Choose destination directory (e.g., trusted, rejected, issuers)

## Project Structure

```
OpenSSL-Certificate-Managament/
├── CertManager.sln                    # Visual Studio solution file
├── CMakeLists.txt                     # CMake build configuration
├── CertManager/
│   ├── CertManager.vcxproj           # Visual Studio project file
│   ├── CertManager.vcxproj.filters   # VS project file organization
│   ├── main.cpp                       # Application entry point with menu
│   ├── CertificateManager.h          # Certificate manager header
│   └── CertificateManager.cpp        # Certificate manager implementation
├── pki/                               # PKI directory (created at runtime)
├── README.md                          # This file
├── BUILD_INSTRUCTIONS.md              # Detailed build guide
└── USAGE.md                           # Usage examples and documentation
```

## Troubleshooting

### OpenSSL Not Found
**Error:** `Cannot open include file: 'openssl/x509.h'`

**Solution:** 
- Verify OpenSSL is installed
- Ensure `OPENSSL_ROOT_DIR` environment variable is set correctly
- Restart Visual Studio after setting environment variables

### Missing DLLs at Runtime
**Error:** `The code execution cannot proceed because libssl-3-x64.dll was not found`

**Solution:**
- Copy OpenSSL DLLs to your executable directory, or
- Add OpenSSL bin directory to system PATH:
  ```cmd
  setx PATH "%PATH%;C:\Program Files\OpenSSL-Win64\bin"
  ```

### Build Errors with vcpkg
If using vcpkg, ensure you've run:
```cmd
vcpkg integrate install
```

## Security Considerations

- **Private Keys:** Keep private keys in `pki/own/private/` secure and never share them
- **Production Use:** This is a demonstration application. For production, implement:
  - Password-protected private keys
  - Certificate validation
  - Proper error handling for all operations
  - Secure key storage mechanisms

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- Built with [OpenSSL](https://www.openssl.org/)
- Designed for Visual Studio 2022
