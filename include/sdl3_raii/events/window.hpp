#pragma once

#include "astra/core/payloads.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class WindowEventType {
    Shown = SDL_EVENT_WINDOW_SHOWN,
    Hidden = SDL_EVENT_WINDOW_HIDDEN,
    Exposed = SDL_EVENT_WINDOW_EXPOSED,
    Moved = SDL_EVENT_WINDOW_MOVED,
    Resized = SDL_EVENT_WINDOW_RESIZED,
    PixelSizeChanged = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
    MetalViewResized = SDL_EVENT_WINDOW_METAL_VIEW_RESIZED,
    Minimized = SDL_EVENT_WINDOW_MINIMIZED,
    Maximized = SDL_EVENT_WINDOW_MAXIMIZED,
    Restored = SDL_EVENT_WINDOW_RESTORED,
    MouseEnter = SDL_EVENT_WINDOW_MOUSE_ENTER,
    MouseLeave = SDL_EVENT_WINDOW_MOUSE_LEAVE,
    FocusGained = SDL_EVENT_WINDOW_FOCUS_GAINED,
    FocusLost = SDL_EVENT_WINDOW_FOCUS_LOST,
    CloseRequested = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
    HitTest = SDL_EVENT_WINDOW_HIT_TEST,
    IccProfileChanged = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
    DisplayChanged = SDL_EVENT_WINDOW_DISPLAY_CHANGED,
    DisplayScaleChanged = SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
    SafeAreaChanged = SDL_EVENT_WINDOW_SAFE_AREA_CHANGED,
    OccludedChanged = SDL_EVENT_WINDOW_OCCLUDED,
    EnterFullscreen = SDL_EVENT_WINDOW_ENTER_FULLSCREEN,
    LeaveFullscreen = SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,
    Destroyed = SDL_EVENT_WINDOW_DESTROYED,
};

struct WindowEvent {
    ASTRA_TAG_MEMBER(sdl3::WindowEvent);
    WindowEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    std::int32_t data1;
    std::int32_t data2;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::WindowEventType> : formatter<std::string_view> {
    auto format(const sdl3::WindowEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::WindowEventType::Shown:
            return formatter<std::string_view>::format("Shown", ctx);
        case sdl3::WindowEventType::Hidden:
            return formatter<std::string_view>::format("Hidden", ctx);
        case sdl3::WindowEventType::Exposed:
            return formatter<std::string_view>::format("Exposed", ctx);
        case sdl3::WindowEventType::Moved:
            return formatter<std::string_view>::format("Moved", ctx);
        case sdl3::WindowEventType::Resized:
            return formatter<std::string_view>::format("Resized", ctx);
        case sdl3::WindowEventType::PixelSizeChanged:
            return formatter<std::string_view>::format("PixelSizeChanged", ctx);
        case sdl3::WindowEventType::MetalViewResized:
            return formatter<std::string_view>::format("MetalViewResized", ctx);
        case sdl3::WindowEventType::Minimized:
            return formatter<std::string_view>::format("Minimized", ctx);
        case sdl3::WindowEventType::Maximized:
            return formatter<std::string_view>::format("Maximized", ctx);
        case sdl3::WindowEventType::Restored:
            return formatter<std::string_view>::format("Restored", ctx);
        case sdl3::WindowEventType::MouseEnter:
            return formatter<std::string_view>::format("MouseEnter", ctx);
        case sdl3::WindowEventType::MouseLeave:
            return formatter<std::string_view>::format("MouseLeave", ctx);
        case sdl3::WindowEventType::FocusGained:
            return formatter<std::string_view>::format("FocusGained", ctx);
        case sdl3::WindowEventType::FocusLost:
            return formatter<std::string_view>::format("FocusLost", ctx);
        case sdl3::WindowEventType::CloseRequested:
            return formatter<std::string_view>::format("CloseRequested", ctx);
        case sdl3::WindowEventType::HitTest:
            return formatter<std::string_view>::format("HitTest", ctx);
        case sdl3::WindowEventType::IccProfileChanged:
            return formatter<std::string_view>::format("IccProfileChanged", ctx);
        case sdl3::WindowEventType::DisplayChanged:
            return formatter<std::string_view>::format("DisplayChanged", ctx);
        case sdl3::WindowEventType::DisplayScaleChanged:
            return formatter<std::string_view>::format("DisplayScaleChanged", ctx);
        case sdl3::WindowEventType::SafeAreaChanged:
            return formatter<std::string_view>::format("SafeAreaChanged", ctx);
        case sdl3::WindowEventType::OccludedChanged:
            return formatter<std::string_view>::format("OccludedChanged", ctx);
        case sdl3::WindowEventType::EnterFullscreen:
            return formatter<std::string_view>::format("EnterFullscreen", ctx);
        case sdl3::WindowEventType::LeaveFullscreen:
            return formatter<std::string_view>::format("LeaveFullscreen", ctx);
        case sdl3::WindowEventType::Destroyed:
            return formatter<std::string_view>::format("Destroyed", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::WindowEvent> : formatter<std::string_view> {
    auto format(const sdl3::WindowEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "WindowEvent(type={}, timestamp={}, window_id={}, data1={}, data2={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.data1,
                e.data2);
    }
};
