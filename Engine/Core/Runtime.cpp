#include "Engine/Core/Runtime.hpp"

#include "Engine/Core/Log.hpp"

namespace urbanduty::core {

Runtime::Runtime(RuntimeConfig config) : config_(config) {
    log(LogLevel::Info, "Runtime created");
}

int Runtime::run_one_frame(double delta_seconds) {
    if (delta_seconds < 0.0) {
        log(LogLevel::Error, "Negative frame delta rejected");
        return 1;
    }
    return running_ ? 0 : 1;
}

bool Runtime::running() const noexcept { return running_; }

void Runtime::request_exit() noexcept { running_ = false; }

} // namespace urbanduty::core
