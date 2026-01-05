#include <napi.h>
#include "smbios_common.h"

namespace smbios {

/**
 * Convert BiosInfo struct to JavaScript object
 */
Napi::Object BiosInfoToObject(Napi::Env env, const BiosInfo& info) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("vendor", Napi::String::New(env, info.vendor));
    obj.Set("version", Napi::String::New(env, info.version));
    obj.Set("releaseDate", Napi::String::New(env, info.releaseDate));
    obj.Set("biosCharacteristics", Napi::String::New(env, info.biosCharacteristics));
    return obj;
}

/**
 * Convert SystemInfo struct to JavaScript object
 */
Napi::Object SystemInfoToObject(Napi::Env env, const SystemInfo& info) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("manufacturer", Napi::String::New(env, info.manufacturer));
    obj.Set("productName", Napi::String::New(env, info.productName));
    obj.Set("serialNumber", Napi::String::New(env, info.serialNumber));
    obj.Set("uuid", Napi::String::New(env, info.uuid));
    obj.Set("skuNumber", Napi::String::New(env, info.skuNumber));
    obj.Set("family", Napi::String::New(env, info.family));
    obj.Set("wakeUpType", Napi::String::New(env, info.wakeUpType));
    return obj;
}

/**
 * Convert BoardInfo struct to JavaScript object
 */
Napi::Object BoardInfoToObject(Napi::Env env, const BoardInfo& info) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("manufacturer", Napi::String::New(env, info.manufacturer));
    obj.Set("product", Napi::String::New(env, info.product));
    obj.Set("version", Napi::String::New(env, info.version));
    obj.Set("serialNumber", Napi::String::New(env, info.serialNumber));
    obj.Set("assetTag", Napi::String::New(env, info.assetTag));
    obj.Set("locationInChassis", Napi::String::New(env, info.locationInChassis));
    return obj;
}

/**
 * Convert ProcessorInfo struct to JavaScript object
 */
Napi::Object ProcessorInfoToObject(Napi::Env env, const ProcessorInfo& info) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("manufacturer", Napi::String::New(env, info.manufacturer));
    obj.Set("version", Napi::String::New(env, info.version));
    obj.Set("socketDesignation", Napi::String::New(env, info.socketDesignation));
    obj.Set("processorType", Napi::String::New(env, info.processorType));
    obj.Set("processorFamily", Napi::String::New(env, info.processorFamily));
    obj.Set("maxSpeed", Napi::String::New(env, info.maxSpeed));
    obj.Set("currentSpeed", Napi::String::New(env, info.currentSpeed));
    obj.Set("coreCount", Napi::String::New(env, info.coreCount));
    obj.Set("threadCount", Napi::String::New(env, info.threadCount));
    obj.Set("l2CacheSize", Napi::String::New(env, info.l2CacheSize));
    obj.Set("l3CacheSize", Napi::String::New(env, info.l3CacheSize));
    return obj;
}

/**
 * Convert MemoryInfo struct to JavaScript object
 */
Napi::Object MemoryInfoToObject(Napi::Env env, const MemoryInfo& info) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("totalPhysicalMemory", Napi::String::New(env, info.totalPhysicalMemory));
    obj.Set("availablePhysicalMemory", Napi::String::New(env, info.availablePhysicalMemory));
    obj.Set("totalVirtualMemory", Napi::String::New(env, info.totalVirtualMemory));
    obj.Set("availableVirtualMemory", Napi::String::New(env, info.availableVirtualMemory));
    obj.Set("memoryDevices", Napi::String::New(env, info.memoryDevices));
    obj.Set("maxCapacity", Napi::String::New(env, info.maxCapacity));
    return obj;
}

/**
 * Convert ChassisInfo struct to JavaScript object
 */
Napi::Object ChassisInfoToObject(Napi::Env env, const ChassisInfo& info) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("manufacturer", Napi::String::New(env, info.manufacturer));
    obj.Set("type", Napi::String::New(env, info.type));
    obj.Set("version", Napi::String::New(env, info.version));
    obj.Set("serialNumber", Napi::String::New(env, info.serialNumber));
    obj.Set("assetTag", Napi::String::New(env, info.assetTag));
    obj.Set("bootUpState", Napi::String::New(env, info.bootUpState));
    obj.Set("powerSupplyState", Napi::String::New(env, info.powerSupplyState));
    obj.Set("thermalState", Napi::String::New(env, info.thermalState));
    return obj;
}

/**
 * Node.js binding: getBiosInfo()
 */
Napi::Value GetBiosInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        BiosInfo biosInfo = GetBiosInfo();
        return BiosInfoToObject(env, biosInfo);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Node.js binding: getSystemInfo()
 */
Napi::Value GetSystemInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        SystemInfo systemInfo = GetSystemInfo();
        return SystemInfoToObject(env, systemInfo);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Node.js binding: getBoardInfo()
 */
Napi::Value GetBoardInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        BoardInfo boardInfo = GetBoardInfo();
        return BoardInfoToObject(env, boardInfo);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Node.js binding: getProcessorInfo()
 */
Napi::Value GetProcessorInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        ProcessorInfo procInfo = GetProcessorInfo();
        return ProcessorInfoToObject(env, procInfo);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Node.js binding: getMemoryInfo()
 */
Napi::Value GetMemoryInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        MemoryInfo memInfo = GetMemoryInfo();
        return MemoryInfoToObject(env, memInfo);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Node.js binding: getChassisInfo()
 */
Napi::Value GetChassisInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        ChassisInfo chassisInfo = GetChassisInfo();
        return ChassisInfoToObject(env, chassisInfo);
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Node.js binding: getAllInfo()
 * Returns all information in a single call
 */
Napi::Value GetAllInfoWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        Napi::Object result = Napi::Object::New(env);
        
        BiosInfo biosInfo = GetBiosInfo();
        SystemInfo systemInfo = GetSystemInfo();
        BoardInfo boardInfo = GetBoardInfo();
        ProcessorInfo procInfo = GetProcessorInfo();
        MemoryInfo memInfo = GetMemoryInfo();
        ChassisInfo chassisInfo = GetChassisInfo();
        
        result.Set("bios", BiosInfoToObject(env, biosInfo));
        result.Set("system", SystemInfoToObject(env, systemInfo));
        result.Set("board", BoardInfoToObject(env, boardInfo));
        result.Set("processor", ProcessorInfoToObject(env, procInfo));
        result.Set("memory", MemoryInfoToObject(env, memInfo));
        result.Set("chassis", ChassisInfoToObject(env, chassisInfo));
        
        return result;
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

/**
 * Initialize the Node.js addon
 */
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(
        Napi::String::New(env, "getBiosInfo"),
        Napi::Function::New(env, GetBiosInfoWrapped)
    );
    
    exports.Set(
        Napi::String::New(env, "getSystemInfo"),
        Napi::Function::New(env, GetSystemInfoWrapped)
    );
    
    exports.Set(
        Napi::String::New(env, "getBoardInfo"),
        Napi::Function::New(env, GetBoardInfoWrapped)
    );
    
    exports.Set(
        Napi::String::New(env, "getProcessorInfo"),
        Napi::Function::New(env, GetProcessorInfoWrapped)
    );
    
    exports.Set(
        Napi::String::New(env, "getMemoryInfo"),
        Napi::Function::New(env, GetMemoryInfoWrapped)
    );
    
    exports.Set(
        Napi::String::New(env, "getChassisInfo"),
        Napi::Function::New(env, GetChassisInfoWrapped)
    );
    
    exports.Set(
        Napi::String::New(env, "getAllInfo"),
        Napi::Function::New(env, GetAllInfoWrapped)
    );
    
    return exports;
}

} // namespace smbios

// Register the addon (must be outside namespace)
Napi::Object InitModule(Napi::Env env, Napi::Object exports) {
    return smbios::Init(env, exports);
}

NODE_API_MODULE(addon, InitModule)
