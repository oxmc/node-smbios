#include "../smbios_common.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <string>
#include <vector>

#pragma comment(lib, "wbemuuid.lib")

namespace smbios {

/**
 * Helper class for WMI queries
 */
class WMIQuery {
private:
    IWbemLocator* pLoc;
    IWbemServices* pSvc;
    bool initialized;

public:
    WMIQuery() : pLoc(nullptr), pSvc(nullptr), initialized(false) {
        // Initialize COM
        HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres) && hres != RPC_E_CHANGED_MODE) {
            return;
        }

        // Set general COM security levels
        hres = CoInitializeSecurity(
            NULL, -1, NULL, NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL, EOAC_NONE, NULL
        );

        if (FAILED(hres) && hres != RPC_E_TOO_LATE) {
            CoUninitialize();
            return;
        }

        // Obtain the initial locator to WMI
        hres = CoCreateInstance(
            CLSID_WbemLocator, 0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc
        );

        if (FAILED(hres)) {
            CoUninitialize();
            return;
        }

        // Connect to WMI
        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0,
            NULL, 0, 0, &pSvc
        );

        if (FAILED(hres)) {
            pLoc->Release();
            CoUninitialize();
            return;
        }

        // Set security levels on the proxy
        hres = CoSetProxyBlanket(
            pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
            RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL, EOAC_NONE
        );

        if (FAILED(hres)) {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return;
        }

        initialized = true;
    }

    ~WMIQuery() {
        if (pSvc) pSvc->Release();
        if (pLoc) pLoc->Release();
        CoUninitialize();
    }

    std::string QueryProperty(const wchar_t* wmiClass, const wchar_t* property) {
        if (!initialized) return "";

        std::wstring query = L"SELECT ";
        query += property;
        query += L" FROM ";
        query += wmiClass;

        IEnumWbemClassObject* pEnumerator = NULL;
        HRESULT hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t(query.c_str()),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL, &pEnumerator
        );

        if (FAILED(hres)) {
            return "";
        }

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        std::string result;

        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            VariantInit(&vtProp);
            hr = pclsObj->Get(property, 0, &vtProp, 0, 0);
            
            if (SUCCEEDED(hr)) {
                if (vtProp.vt == VT_BSTR) {
                    _bstr_t bstr(vtProp.bstrVal);
                    result = (char*)bstr;
                } else if (vtProp.vt == VT_I4 || vtProp.vt == VT_UI4) {
                    result = std::to_string(vtProp.lVal);
                } else if (vtProp.vt == VT_I2 || vtProp.vt == VT_UI2) {
                    result = std::to_string(vtProp.iVal);
                } else if (vtProp.vt == VT_UI1) {
                    result = std::to_string(vtProp.bVal);
                }
            }
            
            VariantClear(&vtProp);
            pclsObj->Release();
            break; // Get only first result
        }

        pEnumerator->Release();
        return TrimString(result);
    }
};

BiosInfo GetBiosInfo() {
    BiosInfo info;
    WMIQuery wmi;
    
    info.vendor = wmi.QueryProperty(L"Win32_BIOS", L"Manufacturer");
    info.version = wmi.QueryProperty(L"Win32_BIOS", L"SMBIOSBIOSVersion");
    info.releaseDate = wmi.QueryProperty(L"Win32_BIOS", L"ReleaseDate");
    info.biosCharacteristics = wmi.QueryProperty(L"Win32_BIOS", L"BIOSVersion");
    
    return info;
}

SystemInfo GetSystemInfo() {
    SystemInfo info;
    WMIQuery wmi;
    
    info.manufacturer = wmi.QueryProperty(L"Win32_ComputerSystem", L"Manufacturer");
    info.productName = wmi.QueryProperty(L"Win32_ComputerSystem", L"Model");
    info.uuid = wmi.QueryProperty(L"Win32_ComputerSystemProduct", L"UUID");
    info.wakeUpType = wmi.QueryProperty(L"Win32_ComputerSystem", L"WakeUpType");
    
    // Try to get serial number from ComputerSystemProduct
    std::string serial = wmi.QueryProperty(L"Win32_ComputerSystemProduct", L"IdentifyingNumber");
    if (serial.empty() || serial == "To Be Filled By O.E.M.") {
        serial = wmi.QueryProperty(L"Win32_BIOS", L"SerialNumber");
    }
    info.serialNumber = serial;
    
    info.skuNumber = wmi.QueryProperty(L"Win32_ComputerSystemProduct", L"SKUNumber");
    info.family = wmi.QueryProperty(L"Win32_ComputerSystemProduct", L"Version");
    
    return info;
}

BoardInfo GetBoardInfo() {
    BoardInfo info;
    WMIQuery wmi;
    
    info.manufacturer = wmi.QueryProperty(L"Win32_BaseBoard", L"Manufacturer");
    info.product = wmi.QueryProperty(L"Win32_BaseBoard", L"Product");
    info.version = wmi.QueryProperty(L"Win32_BaseBoard", L"Version");
    info.serialNumber = wmi.QueryProperty(L"Win32_BaseBoard", L"SerialNumber");
    info.assetTag = wmi.QueryProperty(L"Win32_BaseBoard", L"Tag");
    info.locationInChassis = wmi.QueryProperty(L"Win32_BaseBoard", L"Model");
    
    return info;
}

ProcessorInfo GetProcessorInfo() {
    ProcessorInfo info;
    WMIQuery wmi;
    
    info.manufacturer = wmi.QueryProperty(L"Win32_Processor", L"Manufacturer");
    info.version = wmi.QueryProperty(L"Win32_Processor", L"Name");
    info.socketDesignation = wmi.QueryProperty(L"Win32_Processor", L"SocketDesignation");
    info.processorType = wmi.QueryProperty(L"Win32_Processor", L"Architecture");
    info.processorFamily = wmi.QueryProperty(L"Win32_Processor", L"Family");
    info.maxSpeed = wmi.QueryProperty(L"Win32_Processor", L"MaxClockSpeed");
    info.currentSpeed = wmi.QueryProperty(L"Win32_Processor", L"CurrentClockSpeed");
    info.coreCount = wmi.QueryProperty(L"Win32_Processor", L"NumberOfCores");
    info.threadCount = wmi.QueryProperty(L"Win32_Processor", L"NumberOfLogicalProcessors");
    info.l2CacheSize = wmi.QueryProperty(L"Win32_Processor", L"L2CacheSize");
    info.l3CacheSize = wmi.QueryProperty(L"Win32_Processor", L"L3CacheSize");
    
    return info;
}

MemoryInfo GetMemoryInfo() {
    MemoryInfo info;
    WMIQuery wmi;
    
    info.totalPhysicalMemory = wmi.QueryProperty(L"Win32_ComputerSystem", L"TotalPhysicalMemory");
    info.maxCapacity = wmi.QueryProperty(L"Win32_PhysicalMemoryArray", L"MaxCapacity");
    info.memoryDevices = wmi.QueryProperty(L"Win32_PhysicalMemoryArray", L"MemoryDevices");
    
    // Get available memory from OS
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        info.availablePhysicalMemory = std::to_string(memStatus.ullAvailPhys);
        info.totalVirtualMemory = std::to_string(memStatus.ullTotalVirtual);
        info.availableVirtualMemory = std::to_string(memStatus.ullAvailVirtual);
    }
    
    return info;
}

ChassisInfo GetChassisInfo() {
    ChassisInfo info;
    WMIQuery wmi;
    
    info.manufacturer = wmi.QueryProperty(L"Win32_SystemEnclosure", L"Manufacturer");
    info.type = wmi.QueryProperty(L"Win32_SystemEnclosure", L"ChassisTypes");
    info.version = wmi.QueryProperty(L"Win32_SystemEnclosure", L"Version");
    info.serialNumber = wmi.QueryProperty(L"Win32_SystemEnclosure", L"SerialNumber");
    info.assetTag = wmi.QueryProperty(L"Win32_SystemEnclosure", L"SMBIOSAssetTag");
    info.bootUpState = wmi.QueryProperty(L"Win32_ComputerSystem", L"BootupState");
    info.powerSupplyState = wmi.QueryProperty(L"Win32_ComputerSystem", L"PowerState");
    info.thermalState = wmi.QueryProperty(L"Win32_ComputerSystem", L"ThermalState");
    
    return info;
}

} // namespace smbios

#endif // _WIN32
