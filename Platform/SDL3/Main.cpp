#include "Engine/Core/Log.hpp"
#include "Engine/Core/Runtime.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdint>

int main(int, char**) {
    using namespace urbanduty::core;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        log(LogLevel::Error, SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Urban Duty: Police Simulator",
        1280,
        720,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

    if (!window) {
        log(LogLevel::Error, SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Runtime runtime(RuntimeConfig{});
    bool running = true;
    std::uint64_t previous = SDL_GetTicks();

    while (running && runtime.running()) {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
                runtime.request_exit();
            }
        }

        const std::uint64_t now = SDL_GetTicks();
        const double delta = static_cast<double>(now - previous) / 1000.0;
        previous = now;
        runtime.run_one_frame(delta);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
