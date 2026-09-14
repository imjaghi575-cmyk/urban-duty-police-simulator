#include "Engine/Core/Log.hpp"

#include <iostream>

namespace urbanduty::core {

void log(LogLevel level, std::string_view message) {
    const char* prefix = "INFO";
    if (level == LogLevel::Warning) prefix = "WARN";
    if (level == LogLevel::Error) prefix = "ERROR";
    std::clog << "[UrbanDuty][" << prefix << "] " << message << '\n';
}

} // namespace urbanduty::core
