#pragma once

#include "astra/core/messenger.hpp"

namespace sdl3 {
struct SystemThemeChangedEvent {
    ASTRA_TAG_MEMBER(sdl3::SystemThemeChangedEvent);
    std::uint64_t timestamp;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::SystemThemeChangedEvent> : formatter<std::string_view> {
    auto format(const sdl3::SystemThemeChangedEvent &e, format_context &ctx) const {
        return format_to(ctx.out(), "SystemThemeChangedEvent(timestamp={})", e.timestamp);
    }
};
