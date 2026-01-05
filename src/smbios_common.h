#ifndef SMBIOS_COMMON_H
#define SMBIOS_COMMON_H

#include <string>
#include <vector>

namespace smbios {

/**
 * BIOS Information structure
 */
struct BiosInfo {
    std::string vendor;
    std::string version;
    std::string releaseDate;
    std::string biosCharacteristics;
};

/**
 * System Information structure
 */
struct SystemInfo {
    std::string manufacturer;
    std::string productName;
    std::string serialNumber;
    std::string uuid;
    std::string skuNumber;
    std::string family;
    std::string wakeUpType;
};

/**
 * Board/Baseboard Information structure
 */
struct BoardInfo {
    std::string manufacturer;
    std::string product;
    std::string version;
    std::string serialNumber;
    std::string assetTag;
    std::string locationInChassis;
};

/**
 * Processor Information structure
 */
struct ProcessorInfo {
    std::string manufacturer;
    std::string version;
    std::string socketDesignation;
    std::string processorType;
    std::string processorFamily;
    std::string maxSpeed;
    std::string currentSpeed;
    std::string coreCount;
    std::string threadCount;
    std::string l2CacheSize;
    std::string l3CacheSize;
};

/**
 * Memory Information structure
 */
struct MemoryInfo {
    std::string totalPhysicalMemory;
    std::string availablePhysicalMemory;
    std::string totalVirtualMemory;
    std::string availableVirtualMemory;
    std::string memoryDevices;
    std::string maxCapacity;
};

/**
 * Chassis Information structure
 */
struct ChassisInfo {
    std::string manufacturer;
    std::string type;
    std::string version;
    std::string serialNumber;
    std::string assetTag;
    std::string bootUpState;
    std::string powerSupplyState;
    std::string thermalState;
};

/**
 * Platform-specific implementations
 * These functions must be implemented for each platform
 */
BiosInfo GetBiosInfo();
SystemInfo GetSystemInfo();
BoardInfo GetBoardInfo();
ProcessorInfo GetProcessorInfo();
MemoryInfo GetMemoryInfo();
ChassisInfo GetChassisInfo();

/**
 * Utility functions
 */
std::string TrimString(const std::string& str);
bool FileExists(const std::string& path);
std::string ReadFile(const std::string& path);

} // namespace smbios

#endif // SMBIOS_COMMON_H
