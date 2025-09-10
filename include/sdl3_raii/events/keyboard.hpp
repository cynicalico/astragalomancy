#pragma once

#include "astra/core/payloads.hpp"
#include "sdl3_raii/keycode.hpp"
#include "sdl3_raii/scancode.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class KeyboardDeviceEventType {
    Added = SDL_EVENT_KEYBOARD_ADDED,
    Removed = SDL_EVENT_KEYBOARD_REMOVED,
};

struct KeyboardDeviceEvent {
    ASTRA_TAG_MEMBER(sdl3::KeyboardDeviceEvent);
    KeyboardDeviceEventType type;
    std::uint64_t timestamp;
    SDL_KeyboardID which;
};

enum class KeyboardEventType {
    Down = SDL_EVENT_KEY_DOWN,
    Up = SDL_EVENT_KEY_UP,
};

struct KeyboardEvent {
    ASTRA_TAG_MEMBER(sdl3::KeyboardEvent);
    KeyboardEventType type;
    std::uint64_t timestamp;
    SDL_KeyboardID which;
    SDL_Scancode scancode;
    SDL_Keycode key;
    SDL_Keymod mod;
    std::uint16_t raw;
    bool down;
    bool repeat;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::KeyboardDeviceEventType> : formatter<std::string_view> {
    auto format(const sdl3::KeyboardDeviceEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::KeyboardDeviceEventType::Added:
            return formatter<std::string_view>::format("Added", ctx);
        case sdl3::KeyboardDeviceEventType::Removed:
            return formatter<std::string_view>::format("Removed", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::KeyboardDeviceEvent> : formatter<std::string_view> {
    auto format(const sdl3::KeyboardDeviceEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(), "KeyboardDeviceEvent(type={}, timestamp={}, which={})", e.type, e.timestamp, e.which);
    }
};

template<>
struct fmt::formatter<sdl3::KeyboardEventType> : formatter<std::string_view> {
    auto format(const sdl3::KeyboardEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::KeyboardEventType::Down:
            return formatter<std::string_view>::format("Down", ctx);
        case sdl3::KeyboardEventType::Up:
            return formatter<std::string_view>::format("Up", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::KeyboardEvent> : formatter<std::string_view> {
    auto format(const sdl3::KeyboardEvent &e, format_context &ctx) const {
        const auto key_str = sdl3::keycode_to_string(e.key);
        return format_to(
                ctx.out(),
                "KeyboardEvent(type={}, timestamp={}, which={}, scancode={}/{}, key={}/{}, mod={}, raw={}, down={}, "
                "repeat={})",
                e.type,
                e.timestamp,
                e.which,
                e.scancode,
                static_cast<std::uint32_t>(e.scancode),
                key_str != nullptr ? key_str : "???",
                e.key,
                e.mod,
                e.raw,
                e.down,
                e.repeat);
    }
};
