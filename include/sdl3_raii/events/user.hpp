#pragma once

#include "astra/core/hermes.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
struct UserEvent {
    HERMES_TAG_MEMBER(sdl3::UserEvent);
    std::uint32_t type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    std::int32_t code;
    void *data1;
    void *data2;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::UserEvent> : formatter<std::string_view> {
    auto format(const sdl3::UserEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "UserEvent(type={}, timestamp={}, window_id={}, code={}, data1={}, data2={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.code,
                e.data1,
                e.data2);
    }
};
