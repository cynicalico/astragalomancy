#pragma once

#include "astra/core/payloads.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class DisplayEventType {
    Orientation = SDL_EVENT_DISPLAY_ORIENTATION,
    Added = SDL_EVENT_DISPLAY_ADDED,
    Removed = SDL_EVENT_DISPLAY_REMOVED,
    Moved = SDL_EVENT_DISPLAY_MOVED,
    DesktopModeChanged = SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,
    CurrentModeChanged = SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,
    ContentScaleChanged = SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,
};

struct DisplayEvent {
    ASTRA_TAG_MEMBER(sdl3::DisplayEvent);
    DisplayEventType type;
    std::uint64_t timestamp;
    SDL_DisplayID display_id;
    std::int32_t data1;
    std::int32_t data2;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::DisplayEventType> : formatter<std::string_view> {
    auto format(const sdl3::DisplayEventType e, format_context &ctx) const {
        switch (e) {
        case sdl3::DisplayEventType::Orientation: return formatter<std::string_view>::format("Orientation", ctx);
        case sdl3::DisplayEventType::Added: return formatter<std::string_view>::format("Added", ctx);
        case sdl3::DisplayEventType::Removed: return formatter<std::string_view>::format("Removed", ctx);
        case sdl3::DisplayEventType::Moved: return formatter<std::string_view>::format("Moved", ctx);
        case sdl3::DisplayEventType::DesktopModeChanged:
            return formatter<std::string_view>::format("DesktopModeChanged", ctx);
        case sdl3::DisplayEventType::CurrentModeChanged:
            return formatter<std::string_view>::format("CurrentModeChanged", ctx);
        case sdl3::DisplayEventType::ContentScaleChanged:
            return formatter<std::string_view>::format("ContentScaleChanged", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::DisplayEvent> : formatter<std::string_view> {
    auto format(const sdl3::DisplayEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "DisplayEvent(type={}, timestamp={}, display_id={}, data1={}, data2={})",
                e.type,
                e.timestamp,
                e.display_id,
                e.data1,
                e.data2);
    }
};
