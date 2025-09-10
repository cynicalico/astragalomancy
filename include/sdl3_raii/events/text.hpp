#pragma once

#include "astra/core/payloads.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace sdl3 {
struct TextEditingCandidatesEvent {
    ASTRA_TAG_MEMBER(sdl3::TextEditingCandidatesEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    std::vector<std::string> candidates;
    std::optional<std::size_t> selected_candidate;
    bool horizontal;
};

struct TextEditingEvent {
    ASTRA_TAG_MEMBER(sdl3::TextEditingEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    std::string text;
    std::optional<std::size_t> start;
    std::optional<std::size_t> length;
};

struct TextInputEvent {
    ASTRA_TAG_MEMBER(sdl3::TextInputEvent);
    std::uint64_t timestamp;
    SDL_WindowID window_id;
    std::string text;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::TextEditingCandidatesEvent> : formatter<std::string_view> {
    auto format(const sdl3::TextEditingCandidatesEvent &e, format_context &ctx) const {
        auto selected = e.selected_candidate ? std::to_string(*e.selected_candidate) : "none";
        return fmt::format_to(
                ctx.out(),
                "TextEditingCandidatesEvent(timestamp={}, window_id={}, candidates=[{}], selected_candidate={}, "
                "horizontal={})",
                e.timestamp,
                e.window_id,
                join(e.candidates, ", "),
                selected,
                e.horizontal);
    }
};

template<>
struct fmt::formatter<sdl3::TextEditingEvent> : formatter<std::string_view> {
    auto format(const sdl3::TextEditingEvent &e, format_context &ctx) const {
        auto start = e.start ? std::to_string(*e.start) : "none";
        auto length = e.length ? std::to_string(*e.length) : "none";
        return fmt::format_to(
                ctx.out(),
                "TextEditingEvent(timestamp={}, window_id={}, text=\"{}\", start={}, length={})",
                e.timestamp,
                e.window_id,
                e.text,
                start,
                length);
    }
};

template<>
struct fmt::formatter<sdl3::TextInputEvent> : formatter<std::string_view> {
    auto format(const sdl3::TextInputEvent &e, format_context &ctx) const {
        return fmt::format_to(
                ctx.out(), "TextInputEvent(timestamp={}, window_id={}, text=\"{}\")", e.timestamp, e.window_id, e.text);
    }
};
