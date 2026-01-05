/**
 * node-smbios - Cross-platform SMBIOS information reader
 * 
 * This module provides a simple interface to read SMBIOS information
 * from the system using native C++ code.
 */

try {
    // Try to load prebuilt binary via node-pre-gyp
    const binding_path = require('@mapbox/node-pre-gyp').find(
        require('path').resolve(__dirname, './package.json')
    );
    module.exports = require(binding_path);
} catch (err) {
    try {
        // Fall back to Release build
        module.exports = require('./build/Release/smbios.node');
    } catch (err2) {
        try {
            // Fall back to Debug build
            module.exports = require('./build/Debug/smbios.node');
        } catch (err3) {
            // If no build is available, throw a helpful error
            throw new Error(
                'Native addon not found. Please run "npm install" or "npm run build" to compile the native addon.\n' +
                'Original error: ' + err.message
            );
        }
    }
}

// Add version information
module.exports.version = require('./package.json').version;
