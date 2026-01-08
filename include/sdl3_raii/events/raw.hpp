#pragma once

#include "astra/core/hermes.hpp"

#include <SDL3/SDL_events.h>

namespace sdl3 {
struct RawEvent {
    HERMES_TAG_MEMBER(sdl3::RawEvent);
    const SDL_Event &e;
};
} // namespace sdl3
