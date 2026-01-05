/**
 * Example usage of node-smbios
 * 
 * This demonstrates how to retrieve SMBIOS information from the system.
 * Run with: node example.js
 */

const smbios = require('./index');

console.log('=== node-smbios Example ===\n');

try {
    // Get BIOS information
    console.log('--- BIOS Information ---');
    const biosInfo = smbios.getBiosInfo();
    console.log('Vendor:', biosInfo.vendor || 'N/A');
    console.log('Version:', biosInfo.version || 'N/A');
    console.log('Release Date:', biosInfo.releaseDate || 'N/A');
    console.log('BIOS Characteristics:', biosInfo.biosCharacteristics || 'N/A');
    console.log();

    // Get System information
    console.log('--- System Information ---');
    const systemInfo = smbios.getSystemInfo();
    console.log('Manufacturer:', systemInfo.manufacturer || 'N/A');
    console.log('Product Name:', systemInfo.productName || 'N/A');
    console.log('Serial Number:', systemInfo.serialNumber || 'N/A');
    console.log('UUID:', systemInfo.uuid || 'N/A');
    console.log('SKU Number:', systemInfo.skuNumber || 'N/A');
    console.log('Family:', systemInfo.family || 'N/A');
    console.log('Wake Up Type:', systemInfo.wakeUpType || 'N/A');
    console.log();

    // Get Board information
    console.log('--- Board Information ---');
    const boardInfo = smbios.getBoardInfo();
    console.log('Manufacturer:', boardInfo.manufacturer || 'N/A');
    console.log('Product:', boardInfo.product || 'N/A');
    console.log('Version:', boardInfo.version || 'N/A');
    console.log('Serial Number:', boardInfo.serialNumber || 'N/A');
    console.log('Asset Tag:', boardInfo.assetTag || 'N/A');
    console.log('Location in Chassis:', boardInfo.locationInChassis || 'N/A');
    console.log();

    // Get Processor information
    console.log('--- Processor Information ---');
    const procInfo = smbios.getProcessorInfo();
    console.log('Manufacturer:', procInfo.manufacturer || 'N/A');
    console.log('Name:', procInfo.version || 'N/A');
    console.log('Socket:', procInfo.socketDesignation || 'N/A');
    console.log('Type:', procInfo.processorType || 'N/A');
    console.log('Family:', procInfo.processorFamily || 'N/A');
    console.log('Max Speed:', procInfo.maxSpeed || 'N/A');
    console.log('Current Speed:', procInfo.currentSpeed || 'N/A');
    console.log('Core Count:', procInfo.coreCount || 'N/A');
    console.log('Thread Count:', procInfo.threadCount || 'N/A');
    console.log('L2 Cache:', procInfo.l2CacheSize || 'N/A');
    console.log('L3 Cache:', procInfo.l3CacheSize || 'N/A');
    console.log();

    // Get Memory information
    console.log('--- Memory Information ---');
    const memInfo = smbios.getMemoryInfo();
    console.log('Total Physical Memory:', memInfo.totalPhysicalMemory || 'N/A');
    console.log('Available Physical Memory:', memInfo.availablePhysicalMemory || 'N/A');
    console.log('Total Virtual Memory:', memInfo.totalVirtualMemory || 'N/A');
    console.log('Available Virtual Memory:', memInfo.availableVirtualMemory || 'N/A');
    console.log('Memory Devices:', memInfo.memoryDevices || 'N/A');
    console.log('Max Capacity:', memInfo.maxCapacity || 'N/A');
    console.log();

    // Get Chassis information
    console.log('--- Chassis Information ---');
    const chassisInfo = smbios.getChassisInfo();
    console.log('Manufacturer:', chassisInfo.manufacturer || 'N/A');
    console.log('Type:', chassisInfo.type || 'N/A');
    console.log('Version:', chassisInfo.version || 'N/A');
    console.log('Serial Number:', chassisInfo.serialNumber || 'N/A');
    console.log('Asset Tag:', chassisInfo.assetTag || 'N/A');
    console.log('Boot Up State:', chassisInfo.bootUpState || 'N/A');
    console.log('Power Supply State:', chassisInfo.powerSupplyState || 'N/A');
    console.log('Thermal State:', chassisInfo.thermalState || 'N/A');
    console.log();

    // Get all information at once
    console.log('--- All Information (Single Call) ---');
    const allInfo = smbios.getAllInfo();
    console.log(JSON.stringify(allInfo, null, 2));

} catch (error) {
    console.error('Error:', error.message);
    console.error('\nNote: Some information may require elevated privileges (administrator/root).');
    process.exit(1);
}
