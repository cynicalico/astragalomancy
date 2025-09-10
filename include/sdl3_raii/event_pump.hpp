#pragma once

#include "astra/core/messenger.hpp"

#include <SDL3/SDL.h>

namespace sdl3 {
class EventPump {
public:
    explicit EventPump(astra::Messenger *messenger);

    void pump();

private:
    astra::Messenger *messenger_;

    void publish_event_(const SDL_Event &e);
};
} // namespace sdl3
