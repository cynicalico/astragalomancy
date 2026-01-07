#pragma once

#include "astra/core/messenger.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <cstdint>
#include <string>
#include <vector>

namespace sdl3 {
struct ClipboardEvent {
    ASTRA_TAG_MEMBER(sdl3::ClipboardEvent);
    std::uint64_t timestamp;
    bool owner;
    std::vector<std::string> mime_types;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::ClipboardEvent> : formatter<std::string_view> {
    auto format(const sdl3::ClipboardEvent &e, format_context &ctx) const {
        return fmt::format_to(
                ctx.out(),
                "ClipboardEvent(timestamp={}, owner={}, mime_types=[{}])",
                e.timestamp,
                e.owner,
                join(e.mime_types, ", "));
    }
};
