#pragma once

#include "astra/core/hermes.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace sdl3 {
enum class DropEventType {
    File = SDL_EVENT_DROP_FILE,
    Text = SDL_EVENT_DROP_TEXT,
    Begin = SDL_EVENT_DROP_BEGIN,
    Complete = SDL_EVENT_DROP_COMPLETE,
    Position = SDL_EVENT_DROP_POSITION,
};

struct DropEvent {
    HERMES_TAG_MEMBER(sdl3::DropEvent);
    DropEventType type;
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    float x;
    float y;
    std::optional<std::string> source;
    std::optional<std::string> data;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::DropEventType> : formatter<std::string_view> {
    auto format(const sdl3::DropEventType &e, format_context &ctx) const {
        switch (e) {
        case sdl3::DropEventType::File: return formatter<std::string_view>::format("File", ctx);
        case sdl3::DropEventType::Text: return formatter<std::string_view>::format("Text", ctx);
        case sdl3::DropEventType::Begin: return formatter<std::string_view>::format("Begin", ctx);
        case sdl3::DropEventType::Complete: return formatter<std::string_view>::format("Complete", ctx);
        case sdl3::DropEventType::Position: return formatter<std::string_view>::format("Position", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::DropEvent> : formatter<std::string_view> {
    auto format(const sdl3::DropEvent &e, format_context &ctx) const {
        const auto source = e.source ? *e.source : "none";
        const auto data = e.data ? *e.data : "none";
        return format_to(
                ctx.out(),
                "DropEvent(type={}, timestamp={}, window_id={}, x={}, y={}, source={}, data={})",
                e.type,
                e.timestamp,
                e.window_id,
                e.x,
                e.y,
                source.c_str(), // FIXME: Why can't this be passed without .c_str()?
                data.c_str());
    }
};
