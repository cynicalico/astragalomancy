#pragma once

#include "astra/core/payloads.hpp"
#include "astra/util/enum_class_helpers.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class PenInputFlags {
    None = 0,
    Down = SDL_PEN_INPUT_DOWN,
    Button1 = SDL_PEN_INPUT_BUTTON_1,
    Button2 = SDL_PEN_INPUT_BUTTON_2,
    Button3 = SDL_PEN_INPUT_BUTTON_3,
    Button4 = SDL_PEN_INPUT_BUTTON_4,
    Button5 = SDL_PEN_INPUT_BUTTON_5,
    EraserTip = SDL_PEN_INPUT_ERASER_TIP,
};

enum class PenAxis {
    Pressure = SDL_PEN_AXIS_PRESSURE,
    XTilt = SDL_PEN_AXIS_XTILT,
    YTilt = SDL_PEN_AXIS_YTILT,
    Distance = SDL_PEN_AXIS_DISTANCE,
    Rotation = SDL_PEN_AXIS_ROTATION,
    Slider = SDL_PEN_AXIS_SLIDER,
    TangentialPressure = SDL_PEN_AXIS_TANGENTIAL_PRESSURE,
};

struct PenAxisEvent {
    ASTRA_TAG_MEMBER(sdl3::PenAxisEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_PenID which;
    PenInputFlags pen_state;
    float x;
    float y;
    PenAxis axis;
    float value;
};

enum class PenButtonEventType {
    Down = SDL_EVENT_PEN_BUTTON_DOWN,
    Up = SDL_EVENT_PEN_BUTTON_UP,
};

struct PenButtonEvent {
    ASTRA_TAG_MEMBER(sdl3::PenButtonEvent);
    PenButtonEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_PenID which;
    PenInputFlags pen_state;
    float x;
    float y;
    std::uint8_t button;
    bool down;
};

struct PenMotionEvent {
    ASTRA_TAG_MEMBER(sdl3::PenMotionEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_PenID which;
    PenInputFlags pen_state;
    float x;
    float y;
};

enum class PenProximityEventType {
    In = SDL_EVENT_PEN_PROXIMITY_IN,
    Out = SDL_EVENT_PEN_PROXIMITY_OUT,
};

struct PenProximityEvent {
    ASTRA_TAG_MEMBER(sdl3::PenProximityEvent);
    PenProximityEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_PenID which;
};

enum class PenTouchEventType {
    Down = SDL_EVENT_PEN_DOWN,
    Up = SDL_EVENT_PEN_UP,
};

struct PenTouchEvent {
    ASTRA_TAG_MEMBER(sdl3::PenTouchEvent);
    PenTouchEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    SDL_PenID which;
    PenInputFlags pen_state;
    float x;
    float y;
    bool eraser;
    bool down;
};
} // namespace sdl3

ENUM_CLASS_ENABLE_BITOPS(sdl3::PenInputFlags);

template<>
struct fmt::formatter<sdl3::PenInputFlags> : formatter<std::string_view> {
    auto format(const sdl3::PenInputFlags &e, format_context &ctx) const {
        switch (e) {
        case sdl3::PenInputFlags::None:
            return formatter<std::string_view>::format("None", ctx);
        case sdl3::PenInputFlags::Down:
            return formatter<std::string_view>::format("Down", ctx);
        case sdl3::PenInputFlags::Button1:
            return formatter<std::string_view>::format("Button1", ctx);
        case sdl3::PenInputFlags::Button2:
            return formatter<std::string_view>::format("Button2", ctx);
        case sdl3::PenInputFlags::Button3:
            return formatter<std::string_view>::format("Button3", ctx);
        case sdl3::PenInputFlags::Button4:
            return formatter<std::string_view>::format("Button4", ctx);
        case sdl3::PenInputFlags::Button5:
            return formatter<std::string_view>::format("Button5", ctx);
        case sdl3::PenInputFlags::EraserTip:
            return formatter<std::string_view>::format("EraserTip", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::PenAxis> : formatter<std::string_view> {
    auto format(const sdl3::PenAxis &e, format_context &ctx) const {
        switch (e) {
        case sdl3::PenAxis::Pressure:
            return formatter<std::string_view>::format("Pressure", ctx);
        case sdl3::PenAxis::XTilt:
            return formatter<std::string_view>::format("XTilt", ctx);
        case sdl3::PenAxis::YTilt:
            return formatter<std::string_view>::format("YTilt", ctx);
        case sdl3::PenAxis::Distance:
            return formatter<std::string_view>::format("Distance", ctx);
        case sdl3::PenAxis::Rotation:
            return formatter<std::string_view>::format("Rotation", ctx);
        case sdl3::PenAxis::Slider:
            return formatter<std::string_view>::format("Slider", ctx);
        case sdl3::PenAxis::TangentialPressure:
            return formatter<std::string_view>::format("TangentialPressure", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::PenAxisEvent> : formatter<std::string_view> {
    auto format(const sdl3::PenAxisEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "PenAxisEvent(timestamp={}, window_id={}, which={}, pen_state={}, x={}, y={}, axis={}, value={})",
                e.timestamp,
                e.window_id,
                e.which,
                e.pen_state,
                e.x,
                e.y,
                e.axis,
                e.value);
    }
};

template<>
struct fmt::formatter<sdl3::PenButtonEventType> : formatter<std::string_view> {
    auto format(const sdl3::PenButtonEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::PenButtonEventType::Down:
            return formatter<std::string_view>::format("Down", ctx);
        case sdl3::PenButtonEventType::Up:
            return formatter<std::string_view>::format("Up", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::PenButtonEvent> : formatter<std::string_view> {
    auto format(const sdl3::PenButtonEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "PenButtonEvent(type={}, timestamp={}, window_id={}, which={}, pen_state={}, x={}, y={}, button={}, "
                "down={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.which,
                e.pen_state,
                e.x,
                e.y,
                e.button,
                e.down);
    }
};

template<>
struct fmt::formatter<sdl3::PenMotionEvent> : formatter<std::string_view> {
    auto format(const sdl3::PenMotionEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "PenMotionEvent(timestamp={}, window_id={}, which={}, pen_state={}, x={}, y={})",
                e.timestamp,
                e.window_id,
                e.which,
                e.pen_state,
                e.x,
                e.y);
    }
};

template<>
struct fmt::formatter<sdl3::PenProximityEventType> : formatter<std::string_view> {
    auto format(const sdl3::PenProximityEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::PenProximityEventType::In:
            return formatter<std::string_view>::format("In", ctx);
        case sdl3::PenProximityEventType::Out:
            return formatter<std::string_view>::format("Out", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::PenProximityEvent> : formatter<std::string_view> {
    auto format(const sdl3::PenProximityEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "PenProximityEvent(type={}, timestamp={}, window_id={}, which={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.which);
    }
};

template<>
struct fmt::formatter<sdl3::PenTouchEventType> : formatter<std::string_view> {
    auto format(const sdl3::PenTouchEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::PenTouchEventType::Down:
            return formatter<std::string_view>::format("Down", ctx);
        case sdl3::PenTouchEventType::Up:
            return formatter<std::string_view>::format("Up", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::PenTouchEvent> : formatter<std::string_view> {
    auto format(const sdl3::PenTouchEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "PenTouchEvent(type={}, timestamp={}, window_id={}, which={}, pen_state={}, x={}, y={}, eraser={}, "
                "down={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.which,
                e.pen_state,
                e.x,
                e.y,
                e.eraser,
                e.down);
    }
};
