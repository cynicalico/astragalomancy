#pragma once

#include "astra/core/payloads.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class RenderEventType {
    TargetsReset = SDL_EVENT_RENDER_TARGETS_RESET,
    DeviceReset = SDL_EVENT_RENDER_DEVICE_RESET,
    DeviceLost = SDL_EVENT_RENDER_DEVICE_LOST,
};

struct RenderEvent {
    ASTRA_TAG_MEMBER(sdl3::RenderEvent);
    RenderEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::RenderEventType> : formatter<std::string_view> {
    auto format(const sdl3::RenderEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::RenderEventType::TargetsReset:
            return formatter<std::string_view>::format("TargetsReset", ctx);
        case sdl3::RenderEventType::DeviceReset:
            return formatter<std::string_view>::format("DeviceReset", ctx);
        case sdl3::RenderEventType::DeviceLost:
            return formatter<std::string_view>::format("DeviceLost", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::RenderEvent> : formatter<std::string_view> {
    auto format(const sdl3::RenderEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(), "RenderEvent(type={}, timestamp={}, window_id={})", e.type, e.timestamp, e.window_id);
    }
};
