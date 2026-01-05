#include "smbios_common.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <sys/stat.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace smbios {

/**
 * Trim whitespace from both ends of a string
 */
std::string TrimString(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();
    
    // Trim from start
    while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }
    
    // Trim from end
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        end--;
    }
    
    return str.substr(start, end - start);
}

/**
 * Check if a file exists
 */
bool FileExists(const std::string& path) {
#ifdef _WIN32
    struct _stat buffer;
    return (_stat(path.c_str(), &buffer) == 0);
#else
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
#endif
}

/**
 * Read entire file contents
 */
std::string ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Remove trailing newlines
    while (!content.empty() && (content.back() == '\n' || content.back() == '\r')) {
        content.pop_back();
    }
    
    return TrimString(content);
}

} // namespace smbios
