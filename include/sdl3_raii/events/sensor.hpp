#pragma once

#include "astra/core/payloads.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <string_view>

namespace sdl3 {
struct SensorEvent {
    ASTRA_TAG_MEMBER(sdl3::SensorEvent);
    std::uint64_t timestamp;
    SDL_SensorID which;
    std::array<float, 6> data;
    std::uint64_t sensor_timestamp;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::SensorEvent> : formatter<std::string_view> {
    auto format(const sdl3::SensorEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "SensorEvent(timestamp={}, which={}, data=[{}, {}, {}, {}, {}, {}], sensor_timestamp={})",
                e.timestamp,
                e.which,
                e.data[0],
                e.data[1],
                e.data[2],
                e.data[3],
                e.data[4],
                e.data[5],
                e.sensor_timestamp);
    }
};
