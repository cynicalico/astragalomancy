#pragma once

#include "astra/core/messenger.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <string_view>

namespace sdl3 {
struct GamepadAxisEvent {
    ASTRA_TAG_MEMBER(sdl3::GamepadAxisEvent);
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::uint8_t axis;
    std::int16_t value;
};

enum class GamepadButtonEventType {
    Down = SDL_EVENT_GAMEPAD_BUTTON_DOWN,
    Up = SDL_EVENT_GAMEPAD_BUTTON_UP,
};

struct GamepadButtonEvent {
    ASTRA_TAG_MEMBER(sdl3::GamepadButtonEvent);
    GamepadButtonEventType type;
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::uint8_t button;
    bool down;
};

enum class GamepadDeviceEventType {
    Added = SDL_EVENT_GAMEPAD_ADDED,
    Removed = SDL_EVENT_GAMEPAD_REMOVED,
    Remapped = SDL_EVENT_GAMEPAD_REMAPPED,
    UpdateComplete = SDL_EVENT_GAMEPAD_UPDATE_COMPLETE,
    SteamHandleUpdated = SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED,
};

struct GamepadDeviceEvent {
    ASTRA_TAG_MEMBER(sdl3::GamepadDeviceEvent);
    GamepadDeviceEventType type;
    std::uint64_t timestamp;
    SDL_JoystickID which;
};

enum class GamepadSensorType {
    Invalid = SDL_SENSOR_INVALID,
    Unknown = SDL_SENSOR_UNKNOWN,
    Accelerometer = SDL_SENSOR_ACCEL,
    Gyroscope = SDL_SENSOR_GYRO,
    AccelerometerL = SDL_SENSOR_ACCEL_L,
    GyroscopeL = SDL_SENSOR_GYRO_L,
    AccelerometerR = SDL_SENSOR_ACCEL_R,
    GyroscopeR = SDL_SENSOR_GYRO_R,
};

struct GamepadSensorEvent {
    ASTRA_TAG_MEMBER(sdl3::GamepadSensorEvent);
    std::uint64_t timestamp;
    SDL_JoystickID which;
    GamepadSensorType type;
    std::array<float, 3> data;
    std::uint64_t sensor_timestamp;
};

enum class GamepadTouchpadEventType {
    Down = SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN,
    Motion = SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION,
    Up = SDL_EVENT_GAMEPAD_TOUCHPAD_UP,
};

struct GamepadTouchpadEvent {
    ASTRA_TAG_MEMBER(sdl3::GamepadTouchpadEvent);
    GamepadTouchpadEventType type;
    std::uint64_t timestamp;
    SDL_JoystickID which;
    std::int32_t touchpad;
    std::int32_t finger;
    float x;
    float y;
    float pressure;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::GamepadAxisEvent> : formatter<std::string_view> {
    auto format(const sdl3::GamepadAxisEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "GamepadAxisEvent(timestamp={}, which={}, axis={}, value={})",
                e.timestamp,
                e.which,
                e.axis,
                e.value);
    }
};

template<>
struct fmt::formatter<sdl3::GamepadButtonEventType> : formatter<std::string_view> {
    auto format(const sdl3::GamepadButtonEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::GamepadButtonEventType::Down: return format_to(ctx.out(), "Down");
        case sdl3::GamepadButtonEventType::Up: return format_to(ctx.out(), "Up");
        default: return format_to(ctx.out(), "Unknown");
        }
    }
};

template<>
struct fmt::formatter<sdl3::GamepadButtonEvent> : formatter<std::string_view> {
    auto format(const sdl3::GamepadButtonEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "GamepadButtonEvent(type={}, timestamp={}, which={}, button={}, down={})",
                e.type,
                e.timestamp,
                e.which,
                e.button,
                e.down);
    }
};

template<>
struct fmt::formatter<sdl3::GamepadDeviceEventType> : formatter<std::string_view> {
    auto format(const sdl3::GamepadDeviceEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::GamepadDeviceEventType::Added: return formatter<std::string_view>::format("Added", ctx);
        case sdl3::GamepadDeviceEventType::Removed: return formatter<std::string_view>::format("Removed", ctx);
        case sdl3::GamepadDeviceEventType::Remapped: return formatter<std::string_view>::format("Remapped", ctx);
        case sdl3::GamepadDeviceEventType::UpdateComplete:
            return formatter<std::string_view>::format("UpdateComplete", ctx);
        case sdl3::GamepadDeviceEventType::SteamHandleUpdated:
            return formatter<std::string_view>::format("SteamHandleUpdated", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::GamepadDeviceEvent> : formatter<std::string_view> {
    auto format(const sdl3::GamepadDeviceEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(), "GamepadDeviceEvent(type={}, timestamp={}, which={})", e.type, e.timestamp, e.which);
    }
};

template<>
struct fmt::formatter<sdl3::GamepadSensorType> : formatter<std::string_view> {
    auto format(const sdl3::GamepadSensorType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::GamepadSensorType::Invalid: return formatter<std::string_view>::format("Invalid", ctx);
        case sdl3::GamepadSensorType::Unknown: return formatter<std::string_view>::format("Unknown", ctx);
        case sdl3::GamepadSensorType::Accelerometer: return formatter<std::string_view>::format("Accelerometer", ctx);
        case sdl3::GamepadSensorType::Gyroscope: return formatter<std::string_view>::format("Gyroscope", ctx);
        case sdl3::GamepadSensorType::AccelerometerL: return formatter<std::string_view>::format("AccelerometerL", ctx);
        case sdl3::GamepadSensorType::GyroscopeL: return formatter<std::string_view>::format("GyroscopeL", ctx);
        case sdl3::GamepadSensorType::AccelerometerR: return formatter<std::string_view>::format("AccelerometerR", ctx);
        case sdl3::GamepadSensorType::GyroscopeR: return formatter<std::string_view>::format("GyroscopeR", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::GamepadSensorEvent> : formatter<std::string_view> {
    auto format(const sdl3::GamepadSensorEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "GamepadSensorEvent(timestamp={}, which={}, type={}, data=[{}, {}, {}], sensor_timestamp={})",
                e.timestamp,
                e.which,
                e.type,
                e.data[0],
                e.data[1],
                e.data[2],
                e.sensor_timestamp);
    }
};

template<>
struct fmt::formatter<sdl3::GamepadTouchpadEventType> : formatter<std::string_view> {
    auto format(const sdl3::GamepadTouchpadEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::GamepadTouchpadEventType::Down: return formatter<std::string_view>::format("Down", ctx);
        case sdl3::GamepadTouchpadEventType::Motion: return formatter<std::string_view>::format("Motion", ctx);
        case sdl3::GamepadTouchpadEventType::Up: return formatter<std::string_view>::format("Up", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::GamepadTouchpadEvent> : formatter<std::string_view> {
    auto format(const sdl3::GamepadTouchpadEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "GamepadTouchpadEvent(type={}, timestamp={}, which={}, touchpad={}, finger={}, x={}, y={}, "
                "pressure={})",
                e.type,
                e.timestamp,
                e.which,
                e.touchpad,
                e.finger,
                e.x,
                e.y,
                e.pressure);
    }
};
