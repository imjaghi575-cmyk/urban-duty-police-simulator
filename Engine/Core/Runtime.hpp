#pragma once

#include <cstdint>

namespace urbanduty::core {

struct RuntimeConfig {
    const char* application_name = "Urban Duty: Police Simulator";
    std::uint32_t target_tick_hz = 60;
    double max_frame_delta_seconds = 0.25;
};

class Runtime {
public:
    explicit Runtime(RuntimeConfig config);
    int run_one_frame(double delta_seconds);
    bool running() const noexcept;
    void request_exit() noexcept;
    std::uint32_t simulation_steps_last_frame() const noexcept;

private:
    RuntimeConfig config_;
    bool running_{true};
    double accumulator_seconds_{0.0};
    std::uint32_t simulation_steps_last_frame_{0};
};

} // namespace urbanduty::core
