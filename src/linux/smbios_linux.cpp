#include "../smbios_common.h"

#ifdef __linux__

#include <fstream>
#include <string>

namespace smbios {

// DMI information is typically available in /sys/class/dmi/id/
const std::string DMI_PATH = "/sys/class/dmi/id/";

/**
 * Read DMI file content
 */
std::string ReadDMI(const std::string& filename) {
    std::string path = DMI_PATH + filename;
    return ReadFile(path);
}

BiosInfo GetBiosInfo() {
    BiosInfo info;
    
    info.vendor = ReadDMI("bios_vendor");
    info.version = ReadDMI("bios_version");
    info.releaseDate = ReadDMI("bios_date");
    info.biosCharacteristics = ReadDMI("modalias");
    
    return info;
}

SystemInfo GetSystemInfo() {
    SystemInfo info;
    
    info.manufacturer = ReadDMI("sys_vendor");
    info.productName = ReadDMI("product_name");
    info.serialNumber = ReadDMI("product_serial");
    info.uuid = ReadDMI("product_uuid");
    info.skuNumber = ReadDMI("product_sku");
    info.family = ReadDMI("product_family");
    info.wakeUpType = ReadDMI("chassis_type");
    
    // Clean up common placeholder values
    if (info.serialNumber == "To Be Filled By O.E.M." || 
        info.serialNumber == "System Serial Number" ||
        info.serialNumber == "0") {
        info.serialNumber = "";
    }
    
    if (info.uuid == "To Be Filled By O.E.M." || 
        info.uuid == "00000000-0000-0000-0000-000000000000") {
        info.uuid = "";
    }
    
    return info;
}

BoardInfo GetBoardInfo() {
    BoardInfo info;
    
    info.manufacturer = ReadDMI("board_vendor");
    info.product = ReadDMI("board_name");
    info.version = ReadDMI("board_version");
    info.serialNumber = ReadDMI("board_serial");
    info.assetTag = ReadDMI("board_asset_tag");
    info.locationInChassis = ReadDMI("chassis_vendor");
    
    // Clean up common placeholder values
    if (info.serialNumber == "To Be Filled By O.E.M." || 
        info.serialNumber == "Board Serial Number" ||
        info.serialNumber == "0") {
        info.serialNumber = "";
    }
    
    if (info.assetTag == "To Be Filled By O.E.M." || 
        info.assetTag == "Asset Tag" ||
        info.assetTag == "0") {
        info.assetTag = "";
    }
    
    return info;
}

ProcessorInfo GetProcessorInfo() {
    ProcessorInfo info;
    
    // Try to read from /proc/cpuinfo
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (cpuinfo.is_open()) {
        std::string line;
        while (std::getline(cpuinfo, line)) {
            if (line.find("vendor_id") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.manufacturer = TrimString(line.substr(pos + 1));
                }
            } else if (line.find("model name") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.version = TrimString(line.substr(pos + 1));
                }
            } else if (line.find("cpu family") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.processorFamily = TrimString(line.substr(pos + 1));
                }
            } else if (line.find("cpu MHz") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.currentSpeed = TrimString(line.substr(pos + 1));
                }
            } else if (line.find("cpu cores") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.coreCount = TrimString(line.substr(pos + 1));
                }
            } else if (line.find("siblings") != std::string::npos && info.threadCount.empty()) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.threadCount = TrimString(line.substr(pos + 1));
                }
            }
        }
        cpuinfo.close();
    }
    
    info.socketDesignation = "CPU Socket";
    info.processorType = "Central Processor";
    info.maxSpeed = info.currentSpeed; // Approximation
    
    // Try to get cache sizes
    std::string l2Cache = ReadFile("/sys/devices/system/cpu/cpu0/cache/index2/size");
    if (!l2Cache.empty()) {
        info.l2CacheSize = l2Cache;
    }
    
    std::string l3Cache = ReadFile("/sys/devices/system/cpu/cpu0/cache/index3/size");
    if (!l3Cache.empty()) {
        info.l3CacheSize = l3Cache;
    }
    
    return info;
}

MemoryInfo GetMemoryInfo() {
    MemoryInfo info;
    
    // Read from /proc/meminfo
    std::ifstream meminfo("/proc/meminfo");
    if (meminfo.is_open()) {
        std::string line;
        while (std::getline(meminfo, line)) {
            if (line.find("MemTotal") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    std::string value = TrimString(line.substr(pos + 1));
                    // Convert from KB to bytes
                    try {
                        size_t kb = std::stoull(value);
                        info.totalPhysicalMemory = std::to_string(kb * 1024);
                    } catch (...) {
                        info.totalPhysicalMemory = value;
                    }
                }
            } else if (line.find("MemAvailable") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    std::string value = TrimString(line.substr(pos + 1));
                    try {
                        size_t kb = std::stoull(value);
                        info.availablePhysicalMemory = std::to_string(kb * 1024);
                    } catch (...) {
                        info.availablePhysicalMemory = value;
                    }
                }
            } else if (line.find("SwapTotal") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.totalVirtualMemory = TrimString(line.substr(pos + 1));
                }
            } else if (line.find("SwapFree") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    info.availableVirtualMemory = TrimString(line.substr(pos + 1));
                }
            }
        }
        meminfo.close();
    }
    
    info.memoryDevices = "N/A";
    info.maxCapacity = info.totalPhysicalMemory;
    
    return info;
}

ChassisInfo GetChassisInfo() {
    ChassisInfo info;
    
    info.manufacturer = ReadDMI("chassis_vendor");
    info.type = ReadDMI("chassis_type");
    info.version = ReadDMI("chassis_version");
    info.serialNumber = ReadDMI("chassis_serial");
    info.assetTag = ReadDMI("chassis_asset_tag");
    info.bootUpState = "Normal";
    info.powerSupplyState = "Safe";
    info.thermalState = "Safe";
    
    return info;
}

} // namespace smbios

#endif // __linux__
