#pragma once

#include <cstdint>

namespace urbanduty::core {

struct RuntimeConfig {
    const char* application_name = "Urban Duty: Police Simulator";
    std::uint32_t target_tick_hz = 60;
};

class Runtime {
public:
    explicit Runtime(RuntimeConfig config);
    int run_one_frame(double delta_seconds);
    bool running() const noexcept;
    void request_exit() noexcept;

private:
    RuntimeConfig config_;
    bool running_{true};
};

} // namespace urbanduty::core
