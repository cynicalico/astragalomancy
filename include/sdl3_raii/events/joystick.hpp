#pragma once

#include "astra/core/hermes.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
struct JoystickAxisEvent {
    HERMES_TAG_MEMBER(sdl3::JoystickAxisEvent);
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::uint8_t axis;
    std::int16_t value;
};

struct JoystickBallEvent {
    HERMES_TAG_MEMBER(sdl3::JoystickBallEvent);
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::uint8_t ball;
    std::int16_t xrel;
    std::int16_t yrel;
};

enum class JoystickBatteryState {
    Error = SDL_POWERSTATE_ERROR,
    Unknown = SDL_POWERSTATE_UNKNOWN,
    OnBattery = SDL_POWERSTATE_ON_BATTERY,
    NoBattery = SDL_POWERSTATE_NO_BATTERY,
    Charging = SDL_POWERSTATE_CHARGING,
    Charged = SDL_POWERSTATE_CHARGED,
};

struct JoystickBatteryEvent {
    HERMES_TAG_MEMBER(sdl3::JoystickBatteryEvent);
    std::uint64_t timestamp;
    SDL_JoystickID which;
    JoystickBatteryState state;
    int percent;
};

enum class JoystickButtonEventType {
    Down = SDL_EVENT_JOYSTICK_BUTTON_DOWN,
    Up = SDL_EVENT_JOYSTICK_BUTTON_UP,
};

struct JoystickButtonEvent {
    HERMES_TAG_MEMBER(sdl3::JoystickButtonEvent);
    JoystickButtonEventType type;
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::uint8_t button;
    bool down;
};

enum class JoystickDeviceEventType {
    Added = SDL_EVENT_JOYSTICK_ADDED,
    Removed = SDL_EVENT_JOYSTICK_REMOVED,
    UpdateComplete = SDL_EVENT_JOYSTICK_UPDATE_COMPLETE,
};

struct JoystickDeviceEvent {
    HERMES_TAG_MEMBER(sdl3::JoystickDeviceEvent);
    JoystickDeviceEventType type;
    std::uint64_t timestamp;
    SDL_JoystickID which;
};

enum class JoystickHatValue {
    Centered = SDL_HAT_CENTERED,
    Up = SDL_HAT_UP,
    Right = SDL_HAT_RIGHT,
    Down = SDL_HAT_DOWN,
    Left = SDL_HAT_LEFT,
    RightUp = SDL_HAT_RIGHTUP,
    RightDown = SDL_HAT_RIGHTDOWN,
    LeftUp = SDL_HAT_LEFTUP,
    LeftDown = SDL_HAT_LEFTDOWN,
};

struct JoystickHatEvent {
    HERMES_TAG_MEMBER(sdl3::JoystickHatEvent);
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::uint8_t hat;
    JoystickHatValue value;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::JoystickAxisEvent> : formatter<std::string_view> {
    auto format(const sdl3::JoystickAxisEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "JoystickAxisEvent(timestamp={}, which={}, axis={}, value={})",
                e.timestamp,
                e.which,
                e.axis,
                e.value);
    }
};

template<>
struct fmt::formatter<sdl3::JoystickBallEvent> : formatter<std::string_view> {
    auto format(const sdl3::JoystickBallEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "JoystickBallEvent(timestamp={}, which={}, ball={}, xrel={}, yrel={})",
                e.timestamp,
                e.which,
                e.ball,
                e.xrel,
                e.yrel);
    }
};

template<>
struct fmt::formatter<sdl3::JoystickBatteryState> : formatter<std::string_view> {
    auto format(const sdl3::JoystickBatteryState &e, format_context &ctx) const {
        switch (e) {
        case sdl3::JoystickBatteryState::Error: return formatter<std::string_view>::format("Error", ctx);
        case sdl3::JoystickBatteryState::Unknown: return formatter<std::string_view>::format("Unknown", ctx);
        case sdl3::JoystickBatteryState::OnBattery: return formatter<std::string_view>::format("OnBattery", ctx);
        case sdl3::JoystickBatteryState::NoBattery: return formatter<std::string_view>::format("NoBattery", ctx);
        case sdl3::JoystickBatteryState::Charging: return formatter<std::string_view>::format("Charging", ctx);
        case sdl3::JoystickBatteryState::Charged: return formatter<std::string_view>::format("Charged", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::JoystickBatteryEvent> : formatter<std::string_view> {
    auto format(const sdl3::JoystickBatteryEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "JoystickBatteryEvent(timestamp={}, which={}, state={}, percent={})",
                e.timestamp,
                e.which,
                e.state,
                e.percent);
    }
};

template<>
struct fmt::formatter<sdl3::JoystickButtonEventType> : formatter<std::string_view> {
    auto format(const sdl3::JoystickButtonEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::JoystickButtonEventType::Down: return formatter<std::string_view>::format("Down", ctx);
        case sdl3::JoystickButtonEventType::Up: return formatter<std::string_view>::format("Up", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::JoystickButtonEvent> : formatter<std::string_view> {
    auto format(const sdl3::JoystickButtonEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "JoystickButtonEvent(type={}, timestamp={}, which={}, button={}, down={})",
                e.type,
                e.timestamp,
                e.which,
                e.button,
                e.down);
    }
};

template<>
struct fmt::formatter<sdl3::JoystickDeviceEventType> : formatter<std::string_view> {
    auto format(const sdl3::JoystickDeviceEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::JoystickDeviceEventType::Added: return formatter<std::string_view>::format("Added", ctx);
        case sdl3::JoystickDeviceEventType::Removed: return formatter<std::string_view>::format("Removed", ctx);
        case sdl3::JoystickDeviceEventType::UpdateComplete:
            return formatter<std::string_view>::format("UpdateComplete", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::JoystickDeviceEvent> : formatter<std::string_view> {
    auto format(const sdl3::JoystickDeviceEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(), "JoystickDeviceEvent(type={}, timestamp={}, which={})", e.type, e.timestamp, e.which);
    }
};

template<>
struct fmt::formatter<sdl3::JoystickHatValue> : formatter<std::string_view> {
    auto format(const sdl3::JoystickHatValue &e, format_context &ctx) const {
        switch (e) {
        case sdl3::JoystickHatValue::Centered: return formatter<std::string_view>::format("Centered", ctx);
        case sdl3::JoystickHatValue::Up: return formatter<std::string_view>::format("Up", ctx);
        case sdl3::JoystickHatValue::Right: return formatter<std::string_view>::format("Right", ctx);
        case sdl3::JoystickHatValue::Down: return formatter<std::string_view>::format("Down", ctx);
        case sdl3::JoystickHatValue::Left: return formatter<std::string_view>::format("Left", ctx);
        case sdl3::JoystickHatValue::RightUp: return formatter<std::string_view>::format("RightUp", ctx);
        case sdl3::JoystickHatValue::RightDown: return formatter<std::string_view>::format("RightDown", ctx);
        case sdl3::JoystickHatValue::LeftUp: return formatter<std::string_view>::format("LeftUp", ctx);
        case sdl3::JoystickHatValue::LeftDown: return formatter<std::string_view>::format("LeftDown", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::JoystickHatEvent> : formatter<std::string_view> {
    auto format(const sdl3::JoystickHatEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "JoystickHatEvent(timestamp={}, which={}, hat={}, value={})",
                e.timestamp,
                e.which,
                e.hat,
                e.value);
    }
};
