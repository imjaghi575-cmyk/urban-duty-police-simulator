#include "Engine/Core/Runtime.hpp"

#include <cassert>

int main() {
    using namespace urbanduty::core;

    Runtime runtime(RuntimeConfig{});
    assert(runtime.running());

    // A single 60 Hz frame advances exactly one fixed simulation step.
    assert(runtime.run_one_frame(1.0 / 60.0) == 0);
    assert(runtime.simulation_steps_last_frame() == 1);

    // Half a frame is accumulated without advancing simulation.
    assert(runtime.run_one_frame(1.0 / 120.0) == 0);
    assert(runtime.simulation_steps_last_frame() == 0);

    // The second half-frame completes the next fixed step.
    assert(runtime.run_one_frame(1.0 / 120.0) == 0);
    assert(runtime.simulation_steps_last_frame() == 1);

    // Pathological frame deltas are clamped and cannot cause unbounded catch-up.
    assert(runtime.run_one_frame(10.0) == 0);
    assert(runtime.simulation_steps_last_frame() == 8);

    // Negative deltas are invalid.
    assert(runtime.run_one_frame(-1.0) != 0);

    runtime.request_exit();
    assert(!runtime.running());
    assert(runtime.run_one_frame(1.0 / 60.0) != 0);
    return 0;
}
