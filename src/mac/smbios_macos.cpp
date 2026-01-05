#include "../smbios_common.h"

#ifdef __APPLE__

#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <string>

namespace smbios {

/**
 * Helper function to get string property from IORegistry
 */
std::string GetIORegistryString(const char* service, const char* property) {
    std::string result;
    
    io_service_t platformExpert = IOServiceGetMatchingService(
        kIOMasterPortDefault,
        IOServiceMatching(service)
    );
    
    if (platformExpert) {
        CFTypeRef cfProperty = IORegistryEntryCreateCFProperty(
            platformExpert,
            CFStringCreateWithCString(kCFAllocatorDefault, property, kCFStringEncodingUTF8),
            kCFAllocatorDefault,
            0
        );
        
        if (cfProperty) {
            if (CFGetTypeID(cfProperty) == CFStringGetTypeID()) {
                CFStringRef cfString = (CFStringRef)cfProperty;
                CFIndex length = CFStringGetLength(cfString);
                CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
                char* buffer = new char[maxSize];
                
                if (CFStringGetCString(cfString, buffer, maxSize, kCFStringEncodingUTF8)) {
                    result = buffer;
                }
                
                delete[] buffer;
            } else if (CFGetTypeID(cfProperty) == CFDataGetTypeID()) {
                CFDataRef cfData = (CFDataRef)cfProperty;
                CFIndex length = CFDataGetLength(cfData);
                const UInt8* bytes = CFDataGetBytePtr(cfData);
                result = std::string((const char*)bytes, length);
                
                // Remove null terminators
                size_t pos = result.find('\0');
                if (pos != std::string::npos) {
                    result = result.substr(0, pos);
                }
            }
            
            CFRelease(cfProperty);
        }
        
        IOObjectRelease(platformExpert);
    }
    
    return TrimString(result);
}

/**
 * Helper function to get data property as hex string (for UUID)
 */
std::string GetIORegistryUUID(const char* service, const char* property) {
    std::string result;
    
    io_service_t platformExpert = IOServiceGetMatchingService(
        kIOMasterPortDefault,
        IOServiceMatching(service)
    );
    
    if (platformExpert) {
        CFTypeRef cfProperty = IORegistryEntryCreateCFProperty(
            platformExpert,
            CFStringCreateWithCString(kCFAllocatorDefault, property, kCFStringEncodingUTF8),
            kCFAllocatorDefault,
            0
        );
        
        if (cfProperty) {
            if (CFGetTypeID(cfProperty) == CFStringGetTypeID()) {
                CFStringRef cfString = (CFStringRef)cfProperty;
                CFIndex length = CFStringGetLength(cfString);
                CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
                char* buffer = new char[maxSize];
                
                if (CFStringGetCString(cfString, buffer, maxSize, kCFStringEncodingUTF8)) {
                    result = buffer;
                }
                
                delete[] buffer;
            }
            
            CFRelease(cfProperty);
        }
        
        IOObjectRelease(platformExpert);
    }
    
    return TrimString(result);
}

BiosInfo GetBiosInfo() {
    BiosInfo info;
    
    // On macOS, BIOS info is limited
    info.vendor = "Apple Inc.";
    info.version = GetIORegistryString("IOPlatformExpertDevice", "version");
    info.releaseDate = GetIORegistryString("IOPlatformExpertDevice", "release-date");
    
    // Try getting boot ROM version
    std::string bootRomVersion = GetIORegistryString("IOPlatformExpertDevice", "target-type");
    if (!bootRomVersion.empty()) {
        info.version = bootRomVersion;
    }
    
    // Get ROM version
    std::string romVersion = GetIORegistryString("IOPlatformExpertDevice", "version-major");
    if (!romVersion.empty()) {
        info.biosCharacteristics = "ROM Version: " + romVersion;
    }
    
    return info;
}

SystemInfo GetSystemInfo() {
    SystemInfo info;
    
    info.manufacturer = "Apple Inc.";
    info.productName = GetIORegistryString("IOPlatformExpertDevice", "model");
    info.serialNumber = GetIORegistryString("IOPlatformExpertDevice", "IOPlatformSerialNumber");
    info.uuid = GetIORegistryUUID("IOPlatformExpertDevice", "IOPlatformUUID");
    
    // Get additional info
    std::string modelName = GetIORegistryString("IOPlatformExpertDevice", "model-name");
    if (!modelName.empty()) {
        info.family = modelName;
    }
    
    info.skuNumber = GetIORegistryString("IOPlatformExpertDevice", "model-number");
    info.wakeUpType = GetIORegistryString("IOPlatformExpertDevice", "clock-frequency");
    
    return info;
}

BoardInfo GetBoardInfo() {
    BoardInfo info;
    
    info.manufacturer = "Apple Inc.";
    info.product = GetIORegistryString("IOPlatformExpertDevice", "board-id");
    info.serialNumber = GetIORegistryString("IOPlatformExpertDevice", "IOPlatformSerialNumber");
    
    // Try to get more board info
    std::string modelNumber = GetIORegistryString("IOPlatformExpertDevice", "model-number");
    if (!modelNumber.empty()) {
        info.version = modelNumber;
    }
    
    info.assetTag = GetIORegistryString("IOPlatformExpertDevice", "target-type");
    info.locationInChassis = GetIORegistryString("IOPlatformExpertDevice", "product-name");
    
    return info;
}

ProcessorInfo GetProcessorInfo() {
    ProcessorInfo info;
    
    // Get CPU info from sysctl or IORegistry
    info.manufacturer = GetIORegistryString("IODeviceTree:/cpus/cpu@0", "vendor");
    if (info.manufacturer.empty()) {
        info.manufacturer = "Apple";
    }
    
    info.version = GetIORegistryString("IODeviceTree:/cpus/cpu@0", "name");
    info.socketDesignation = GetIORegistryString("IOPlatformExpertDevice", "target-type");
    info.processorType = GetIORegistryString("IODeviceTree:/cpus/cpu@0", "device_type");
    info.processorFamily = GetIORegistryString("IOPlatformExpertDevice", "compatible");
    
    // Note: More detailed CPU info would require sysctlbyname calls
    info.maxSpeed = "N/A (use sysctl for detailed info)";
    info.currentSpeed = "N/A";
    info.coreCount = "N/A";
    info.threadCount = "N/A";
    info.l2CacheSize = "N/A";
    info.l3CacheSize = "N/A";
    
    return info;
}

MemoryInfo GetMemoryInfo() {
    MemoryInfo info;
    
    // Note: Full implementation would use sysctlbyname
    info.totalPhysicalMemory = "N/A (use sysctl hw.memsize)";
    info.availablePhysicalMemory = "N/A";
    info.totalVirtualMemory = "N/A";
    info.availableVirtualMemory = "N/A";
    info.memoryDevices = "N/A";
    info.maxCapacity = "N/A";
    
    return info;
}

ChassisInfo GetChassisInfo() {
    ChassisInfo info;
    
    info.manufacturer = "Apple Inc.";
    info.type = GetIORegistryString("IOPlatformExpertDevice", "product-name");
    info.version = GetIORegistryString("IOPlatformExpertDevice", "version");
    info.serialNumber = GetIORegistryString("IOPlatformExpertDevice", "IOPlatformSerialNumber");
    info.assetTag = GetIORegistryString("IOPlatformExpertDevice", "model");
    info.bootUpState = "Normal";
    info.powerSupplyState = "Safe";
    info.thermalState = "Safe";
    
    return info;
}

} // namespace smbios

#endif // __APPLE__
