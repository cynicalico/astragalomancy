#pragma once

#include "astra/core/payloads.hpp"

#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
struct QuitEvent {
    ASTRA_TAG_MEMBER(sdl3::QuitEvent);
    std::uint64_t timestamp;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::QuitEvent> : formatter<std::string_view> {
    auto format(const sdl3::QuitEvent &e, format_context &ctx) const {
        return format_to(ctx.out(), "QuitEvent(timestamp={})", e.timestamp);
    }
};
