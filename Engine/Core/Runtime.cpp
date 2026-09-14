#include "Engine/Core/Runtime.hpp"

#include "Engine/Core/Log.hpp"

namespace urbanduty::core {

Runtime::Runtime(RuntimeConfig config) : config_(config) {
    if (config_.target_tick_hz == 0) {
        config_.target_tick_hz = 60;
    }
    if (config_.max_frame_delta_seconds <= 0.0) {
        config_.max_frame_delta_seconds = 0.25;
    }
    log(LogLevel::Info, "Runtime created");
}

int Runtime::run_one_frame(double delta_seconds) {
    if (!running_) {
        return 1;
    }
    if (delta_seconds < 0.0) {
        log(LogLevel::Error, "Negative frame delta rejected");
        return 1;
    }

    const double clamped_delta =
        delta_seconds > config_.max_frame_delta_seconds
            ? config_.max_frame_delta_seconds
            : delta_seconds;
    const double fixed_step = 1.0 / static_cast<double>(config_.target_tick_hz);

    accumulator_seconds_ += clamped_delta;
    simulation_steps_last_frame_ = 0;

    constexpr std::uint32_t max_steps_per_frame = 8;
    while (accumulator_seconds_ >= fixed_step &&
           simulation_steps_last_frame_ < max_steps_per_frame) {
        accumulator_seconds_ -= fixed_step;
        ++simulation_steps_last_frame_;
    }

    // Prevent a stalled application from attempting an unbounded catch-up.
    if (simulation_steps_last_frame_ == max_steps_per_frame &&
        accumulator_seconds_ >= fixed_step) {
        accumulator_seconds_ = 0.0;
    }

    return 0;
}

bool Runtime::running() const noexcept { return running_; }

void Runtime::request_exit() noexcept { running_ = false; }

std::uint32_t Runtime::simulation_steps_last_frame() const noexcept {
    return simulation_steps_last_frame_;
}

} // namespace urbanduty::core
