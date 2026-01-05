# node-smbios

[![npm version](https://badge.fury.io/js/node-smbios.svg)](https://www.npmjs.com/package/node-smbios)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Cross-platform Node.js native addon for reading SMBIOS (System Management BIOS) / DMI (Desktop Management Interface) hardware information.

## Features

- 🖥️ **Cross-platform** - Works on Windows, macOS, and Linux
- ⚡ **Fast** - Native C++ implementation for optimal performance
- 📦 **Easy to use** - Simple JavaScript API
- 🔒 **No external dependencies** - Uses platform-native APIs
- 🎯 **Comprehensive** - Reads BIOS, System, Board, Processor, Memory, and Chassis information
- 🔧 **Stable** - Uses N-API for compatibility across Node.js versions

## Installation

```bash
npm install node-smbios
```

### How Installation Works

The package uses **prebuilt native binaries** for most users:

1. **First**: Attempts to download a prebuilt binary from [GitHub Releases](https://github.com/oxmc/node-smbios/releases)
   - Binaries are available for common platforms: Windows, macOS, Linux
   - Supports Node.js versions: 16, 18, 20, 22
   - Covers architectures: x64, arm64

2. **Fallback**: If no prebuilt binary is available, it compiles from source
   - Requires build tools (see below)
   - Takes longer but works on any platform

**Benefits:**
- ✅ Most users get instant installation (no compilation needed)
- ✅ No build tools required for common platforms
- ✅ Automatic fallback ensures compatibility

### Build Tools (Only if Compiling from Source)

**Node.js:**
- Node.js >= 16.0.0
- npm (comes with Node.js)

**Build Tools:**

The package requires native compilation tools on each platform:

#### Windows

You need Visual Studio Build Tools (or full Visual Studio) with C++ support:

**Option 1: Install Visual Studio 2022 or later**
- Download from [Visual Studio Downloads](https://visualstudio.microsoft.com/downloads/)
- During installation, select "Desktop development with C++" workload
- Includes MSVC compiler, Windows SDK, and MSBuild

**Option 2: Install Build Tools only**
```bash
npm install --global windows-build-tools
```

**Required components:**
- MSVC C++ compiler
- Windows SDK (10 or 11)
- MSBuild

**Dependencies/Libraries used:**
- Windows Management Instrumentation (WMI) - Built into Windows
- `wbemuuid.lib` - Links automatically via `#pragma comment`

#### macOS

Install Xcode Command Line Tools:

```bash
xcode-select --install
```

**Required components:**
- Clang/LLVM C++ compiler
- macOS SDK

**System frameworks used:**
- `IOKit.framework` - For hardware information access
- `CoreFoundation.framework` - For data type conversions

**Minimum macOS version:** 10.13 (High Sierra)

#### Linux

Install build essentials and development tools:

**Debian/Ubuntu:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential python3
```

**Red Hat/Fedora/CentOS:**
```bash
sudo yum install gcc gcc-c++ make python3
# or on newer versions:
sudo dnf install gcc gcc-c++ make python3
```

**Arch Linux:**
```bash
sudo pacman -S base-devel python
```

**Required components:**
- GCC/G++ compiler (version 7.0+)
- GNU Make
- Python 3 (for node-gyp)

**System dependencies:**
- No external libraries required (reads from `/sys/class/dmi/id/` and `/proc/`)
- May require read permissions for DMI files (usually root or `sudo`)

### Verifying Installation

After installation, verify it works:

```bash
node -e "const smbios = require('node-smbios'); console.log(smbios.getSystemInfo());"
```

### Forcing Source Build

If you want to compile from source instead of using prebuilt binaries:

```bash
npm install node-smbios --build-from-source
```

## Usage

```javascript
const smbios = require('node-smbios');

// Get BIOS information
const biosInfo = smbios.getBiosInfo();
console.log('BIOS Vendor:', biosInfo.vendor);
console.log('BIOS Version:', biosInfo.version);
console.log('Release Date:', biosInfo.releaseDate);

// Get System information
const systemInfo = smbios.getSystemInfo();
console.log('Manufacturer:', systemInfo.manufacturer);
console.log('Product:', systemInfo.productName);
console.log('Serial:', systemInfo.serialNumber);
console.log('UUID:', systemInfo.uuid);

// Get Board information
const boardInfo = smbios.getBoardInfo();
console.log('Board Manufacturer:', boardInfo.manufacturer);
console.log('Board Product:', boardInfo.product);

// Get Processor information
const procInfo = smbios.getProcessorInfo();
console.log('CPU:', procInfo.version);
console.log('Cores:', procInfo.coreCount);
console.log('Threads:', procInfo.threadCount);

// Get Memory information
const memInfo = smbios.getMemoryInfo();
console.log('Total Memory:', memInfo.totalPhysicalMemory);
console.log('Available Memory:', memInfo.availablePhysicalMemory);

// Get Chassis information
const chassisInfo = smbios.getChassisInfo();
console.log('Chassis Type:', chassisInfo.type);
console.log('Manufacturer:', chassisInfo.manufacturer);

// Get ALL information at once
const allInfo = smbios.getAllInfo();
console.log(JSON.stringify(allInfo, null, 2));
```

## API Reference

### `getBiosInfo()`

Returns BIOS information:
- `vendor` - BIOS manufacturer
- `version` - BIOS version
- `releaseDate` - BIOS release date
- `biosCharacteristics` - Additional BIOS characteristics

### `getSystemInfo()`

Returns system information:
- `manufacturer` - System manufacturer
- `productName` - System product/model name
- `serialNumber` - System serial number
- `uuid` - System UUID
- `skuNumber` - SKU number
- `family` - Product family
- `wakeUpType` - Wake-up type

### `getBoardInfo()`

Returns motherboard/baseboard information:
- `manufacturer` - Board manufacturer
- `product` - Board product name
- `version` - Board version
- `serialNumber` - Board serial number
- `assetTag` - Asset tag
- `locationInChassis` - Location in chassis

### `getProcessorInfo()`

Returns processor information:
- `manufacturer` - CPU manufacturer
- `version` - CPU model name
- `socketDesignation` - CPU socket
- `processorType` - Processor type
- `processorFamily` - Processor family
- `maxSpeed` - Maximum speed (MHz)
- `currentSpeed` - Current speed (MHz)
- `coreCount` - Number of cores
- `threadCount` - Number of threads
- `l2CacheSize` - L2 cache size (KB)
- `l3CacheSize` - L3 cache size (KB)

### `getMemoryInfo()`

Returns memory information:
- `totalPhysicalMemory` - Total physical memory (bytes)
- `availablePhysicalMemory` - Available physical memory (bytes)
- `totalVirtualMemory` - Total virtual memory (bytes)
- `availableVirtualMemory` - Available virtual memory (bytes)
- `memoryDevices` - Number of memory devices
- `maxCapacity` - Maximum memory capacity

### `getChassisInfo()`

Returns chassis information:
- `manufacturer` - Chassis manufacturer
- `type` - Chassis type
- `version` - Chassis version
- `serialNumber` - Chassis serial number
- `assetTag` - Asset tag
- `bootUpState` - Boot-up state
- `powerSupplyState` - Power supply state
- `thermalState` - Thermal state

### `getAllInfo()`

Returns all information in a single object with keys: `bios`, `system`, `board`, `processor`, `memory`, `chassis`.

## Platform-Specific Notes

### Windows
- Uses **WMI** (Windows Management Instrumentation)
- Supports Windows 7 and later
- May require administrator privileges for some detailed information

### macOS
- Uses **IOKit** framework
- Accesses IORegistry for system information
- Works on macOS 10.13 and later
- Some processor/memory details may require additional system calls

### Linux
- Reads from `/sys/class/dmi/id/` for DMI information
- Reads from `/proc/cpuinfo` and `/proc/meminfo` for processor and memory details
- May require root privileges to access some DMI files
- Works on most modern Linux distributions

## Building from Source

If you want to build the addon from source or contribute to development:

### 1. Clone the Repository

```bash
git clone https://github.com/oxmc/node-smbios.git
cd node-smbios
```

### 2. Install Dependencies

```bash
npm install
```

This installs:
- `node-addon-api` - N-API C++ wrapper classes
- `node-gyp` - Build tool for native addons

### 3. Build the Native Addon

```bash
npm run build
```

This runs `node-gyp rebuild` which:
1. Generates platform-specific build files (Visual Studio solution on Windows, Makefile on Unix)
2. Compiles the C++ source files:
   - `src/binding.cpp` - N-API bindings
   - `src/smbios_common.cpp` - Common utilities
   - Platform-specific file: `src/windows/smbios_windows.cpp`, `src/mac/smbios_macos.cpp`, or `src/linux/smbios_linux.cpp`
3. Links with platform-specific libraries
4. Outputs `build/Release/smbios.node` (the compiled addon)

### 4. Run Tests

```bash
npm test
```

This runs `example.js` which demonstrates all API functions.

### Build Configuration

The build process is configured in `binding.gyp`:

**Common settings:**
- C++11 or later required
- N-API based (no V8 API dependencies)
- Exception handling enabled

**Platform-specific compilation:**
- **Windows**: Compiles with MSVC, links against `wbemuuid.lib` for WMI
- **macOS**: Compiles with Clang, links against IOKit and CoreFoundation frameworks
- **Linux**: Compiles with GCC, no external libraries needed

### Development Commands

```bash
# Clean build artifacts
npm run clean

# Rebuild (clean + build)
npm run build

# Install and build (as end user would)
npm install

# Run example/test
npm test
```

### Project Structure

```
node-smbios/
├── src/
│   ├── binding.cpp              # N-API entry point
│   ├── smbios_common.h          # Common data structures
│   ├── smbios_common.cpp        # Utility functions
│   ├── windows/
│   │   └── smbios_windows.cpp   # Windows WMI implementation
│   ├── mac/
│   │   └── smbios_macos.cpp     # macOS IOKit implementation
│   └── linux/
│       └── smbios_linux.cpp     # Linux /sys/dmi implementation
├── binding.gyp                  # node-gyp configuration
├── package.json                 # npm package configuration
├── index.js                     # JavaScript entry point
└── example.js                   # Usage examples
```

### Debugging Build Issues

**Enable verbose output:**
```bash
npm run build -- --verbose
```

**Check node-gyp version:**
```bash
npx node-gyp --version
```

**Manually rebuild:**
```bash
npx node-gyp clean
npx node-gyp configure
npx node-gyp build
```

**Common issues:**
- **Python not found**: node-gyp requires Python 3.x
- **Compiler not found**: Install platform build tools (see Prerequisites)
- **Permission errors**: On Linux, some DMI files require root access

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Releasing

This project uses automated releases via GitHub Actions. To create a new release:

1. Update the version in `package.json`:
   ```bash
   npm version patch  # or minor, or major
   ```

2. Push the tag to GitHub:
   ```bash
   git push && git push --tags
   ```

3. GitHub Actions will automatically:
   - Build native binaries for all supported platforms (Windows, macOS, Linux)
   - Upload binaries to GitHub Releases
   - Publish the package to npm

See [.github/RELEASE.md](.github/RELEASE.md) for more details on the release process.

## Troubleshooting

### Build Errors

**Windows:**
```bash
npm install --global windows-build-tools
```

**macOS:**
```bash
xcode-select --install
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt-get install build-essential
```

### Permission Issues

Some SMBIOS information may require elevated privileges:

**Windows:** Run as Administrator

**Linux:** Run with sudo or add your user to appropriate groups

**macOS:** Usually works without special privileges

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Author

Your Name <your.email@example.com>

## Acknowledgments

- Uses [node-addon-api](https://github.com/nodejs/node-addon-api) for N-API bindings
- Inspired by system information tools across different platforms

## Related Projects

- [systeminformation](https://www.npmjs.com/package/systeminformation) - Comprehensive system and OS information
- [node-dmidecode](https://www.npmjs.com/package/node-dmidecode) - Linux dmidecode wrapper
- [wmi-client](https://www.npmjs.com/package/wmi-client) - Windows WMI client

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for release history.
