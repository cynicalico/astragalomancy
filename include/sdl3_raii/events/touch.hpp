#pragma once

#include "astra/core/payloads.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class TouchFingerEventType {
    Down = SDL_EVENT_FINGER_DOWN,
    Up = SDL_EVENT_FINGER_UP,
    Motion = SDL_EVENT_FINGER_MOTION,
    Canceled = SDL_EVENT_FINGER_CANCELED,
};

struct TouchFingerEvent {
    ASTRA_TAG_MEMBER(sdl3::TouchFingerEvent);
    TouchFingerEventType type;
    std::uint64_t timestamp;
    SDL_TouchID touch_id;
    SDL_FingerID finger_id;
    float x;
    float y;
    float dx;
    float dy;
    float pressure;
    SDL_WindowID window_id;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::TouchFingerEventType> : formatter<std::string_view> {
    auto format(const sdl3::TouchFingerEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::TouchFingerEventType::Down:
            return formatter<std::string_view>::format("Down", ctx);
        case sdl3::TouchFingerEventType::Up:
            return formatter<std::string_view>::format("Up", ctx);
        case sdl3::TouchFingerEventType::Motion:
            return formatter<std::string_view>::format("Motion", ctx);
        case sdl3::TouchFingerEventType::Canceled:
            return formatter<std::string_view>::format("Canceled", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::TouchFingerEvent> : formatter<std::string_view> {
    auto format(const sdl3::TouchFingerEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "TouchFingerEvent(type={}, timestamp={}, touch_id={}, finger_id={}, x={}, y={}, dx={}, dy={}, "
                "pressure={}, window_id={})",
                e.type,
                e.timestamp,
                e.touch_id,
                e.finger_id,
                e.x,
                e.y,
                e.dx,
                e.dy,
                e.pressure,
                e.window_id);
    }
};
