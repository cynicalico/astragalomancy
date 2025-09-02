#include "sdl3_raii/init.hpp"

#include "astra/core/log.hpp"

#include <SDL3/SDL.h>
#include <fmt/format.h>

bool sdl3::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        ASTRA_LOG_CRITICAL("Failed to initialize SDL3: {}", SDL_GetError());
        return false;
    }

    const auto version = SDL_GetVersion();
    ASTRA_LOG_DEBUG(
            "SDL version: {}.{}.{}",
            SDL_VERSIONNUM_MAJOR(version),
            SDL_VERSIONNUM_MINOR(version),
            SDL_VERSIONNUM_MICRO(version));

    return true;
}

void sdl3::exit() {
    SDL_Quit();
}
