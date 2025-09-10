#pragma once

#include "astra/core/payloads.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class AudioEventType {
    Added = SDL_EVENT_AUDIO_DEVICE_ADDED,
    Removed = SDL_EVENT_AUDIO_DEVICE_REMOVED,
    FormatChanged = SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,
};

struct AudioDeviceEvent {
    ASTRA_TAG_MEMBER(sdl3::AudioEvent);
    AudioEventType type;
    std::uint64_t timestamp;
    SDL_AudioDeviceID which;
    bool recording;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::AudioEventType> : formatter<std::string_view> {
    auto format(const sdl3::AudioEventType e, format_context &ctx) const {
        switch (e) {
        case sdl3::AudioEventType::Added:
            return formatter<std::string_view>::format("Added", ctx);
        case sdl3::AudioEventType::Removed:
            return formatter<std::string_view>::format("Removed", ctx);
        case sdl3::AudioEventType::FormatChanged:
            return formatter<std::string_view>::format("FormatChanged", ctx);
        default:
            return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::AudioDeviceEvent> : formatter<std::string_view> {
    auto format(const sdl3::AudioDeviceEvent &e, format_context &ctx) const {
        return format_to(
                ctx.out(),
                "AudioEvent(type={}, timestamp={}, which={}, recording={})",
                e.type,
                e.timestamp,
                e.which,
                e.recording);
    }
};
