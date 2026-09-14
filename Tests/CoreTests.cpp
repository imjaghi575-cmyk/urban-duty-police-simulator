#include "Engine/Core/Runtime.hpp"

#include <cassert>

int main() {
    using namespace urbanduty::core;

    Runtime runtime(RuntimeConfig{});
    assert(runtime.running());
    assert(runtime.run_one_frame(1.0 / 60.0) == 0);
    assert(runtime.run_one_frame(-1.0) != 0);

    runtime.request_exit();
    assert(!runtime.running());
    assert(runtime.run_one_frame(1.0 / 60.0) != 0);
    return 0;
}
