#pragma once

#include "astra/core/payloads.hpp"
#include "astra/util/enum_class_helpers.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class MouseButtonEventType {
    Down = SDL_EVENT_MOUSE_BUTTON_DOWN,
    Up = SDL_EVENT_MOUSE_BUTTON_UP,
};

enum class MouseButtonFlags {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2,
};

struct MouseButtonEvent {
    ASTRA_TAG_MEMBER(sdl3::MouseButtonEvent);
    MouseButtonEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_MouseID which;
    MouseButtonFlags button;
    bool down;
    std::uint8_t clicks;
    float x;
    float y;
};

enum class MouseDeviceEventType {
    Added = SDL_EVENT_MOUSE_ADDED,
    Removed = SDL_EVENT_MOUSE_REMOVED,
};

struct MouseDeviceEvent {
    ASTRA_TAG_MEMBER(sdl3::MouseDeviceEvent);
    MouseDeviceEventType type;
    std::uint64_t timestamp;
    SDL_MouseID which;
};

struct MouseMotionEvent {
    ASTRA_TAG_MEMBER(sdl3::MouseMotionEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_MouseID which;
    MouseButtonFlags state;
    float x;
    float y;
    float xrel;
    float yrel;
};

enum class MouseWheelDirection {
    Normal = SDL_MOUSEWHEEL_NORMAL,
    Flipped = SDL_MOUSEWHEEL_FLIPPED,
};

struct MouseWheelEvent {
    ASTRA_TAG_MEMBER(sdl3::MouseWheelEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_MouseID which;
    float x;
    float y;
    MouseWheelDirection direction;
    float mouse_x;
    float mouse_y;
    std::int32_t integer_x;
    std::int32_t integer_y;
};
} // namespace sdl3

ENUM_CLASS_ENABLE_BITOPS(sdl3::MouseButtonFlags);

template<>
struct fmt::formatter<sdl3::MouseButtonEventType> : formatter<std::string_view> {
    auto format(const sdl3::MouseButtonEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::MouseButtonEventType::Down:
            return formatter<std::string_view>::format("Down", ctx);
        case sdl3::MouseButtonEventType::Up:
            return formatter<std::string_view>::format("Up", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::MouseButtonFlags> : formatter<std::string_view> {
    auto format(const sdl3::MouseButtonFlags &e, format_context &ctx) const {
        switch (e) {
        case sdl3::MouseButtonFlags::Left:
            return formatter<std::string_view>::format("Left", ctx);
        case sdl3::MouseButtonFlags::Middle:
            return formatter<std::string_view>::format("Middle", ctx);
        case sdl3::MouseButtonFlags::Right:
            return formatter<std::string_view>::format("Right", ctx);
        case sdl3::MouseButtonFlags::X1:
            return formatter<std::string_view>::format("X1", ctx);
        case sdl3::MouseButtonFlags::X2:
            return formatter<std::string_view>::format("X2", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::MouseButtonEvent> : formatter<std::string_view> {
    auto format(const sdl3::MouseButtonEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "MouseButtonEvent(type={}, timestamp={}, window_id={}, which={}, button={}, down={}, clicks={}, x={}, "
                "y={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.which,
                e.button,
                e.down,
                e.clicks,
                e.x,
                e.y);
    }
};


template<>
struct fmt::formatter<sdl3::MouseDeviceEventType> : formatter<std::string_view> {
    auto format(const sdl3::MouseDeviceEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::MouseDeviceEventType::Added:
            return formatter<std::string_view>::format("Added", ctx);
        case sdl3::MouseDeviceEventType::Removed:
            return formatter<std::string_view>::format("Removed", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::MouseDeviceEvent> : formatter<std::string_view> {
    auto format(const sdl3::MouseDeviceEvent &e, format_context &ctx) const {
        return format_to(ctx.out(), "MouseDeviceEvent(type={}, timestamp={}, which={})", e.type, e.timestamp, e.which);
    }
};

template<>
struct fmt::formatter<sdl3::MouseMotionEvent> : formatter<std::string_view> {
    auto format(const sdl3::MouseMotionEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "MouseMotionEvent(timestamp={}, window_id={}, which={}, state={}, x={}, y={}, xrel={}, yrel={})",
                e.timestamp,
                e.window_id,
                e.which,
                e.state,
                e.x,
                e.y,
                e.xrel,
                e.yrel);
    }
};

template<>
struct fmt::formatter<sdl3::MouseWheelDirection> : formatter<std::string_view> {
    auto format(const sdl3::MouseWheelDirection &e, format_context &ctx) const {
        switch (e) {
        case sdl3::MouseWheelDirection::Normal:
            return formatter<std::string_view>::format("Normal", ctx);
        case sdl3::MouseWheelDirection::Flipped:
            return formatter<std::string_view>::format("Flipped", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::MouseWheelEvent> : formatter<std::string_view> {
    auto format(const sdl3::MouseWheelEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "MouseWheelEvent(timestamp={}, window_id={}, which={}, x={}, y={}, direction={}, mouse_x={}, "
                "mouse_y={}, integer_x={}, integer_y={})",
                e.timestamp,
                e.window_id,
                e.which,
                e.x,
                e.y,
                e.direction,
                e.mouse_x,
                e.mouse_y,
                e.integer_x,
                e.integer_y);
    }
};
