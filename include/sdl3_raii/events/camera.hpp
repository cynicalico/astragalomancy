#pragma once

#include "astra/core/hermes.hpp"

#include <SDL3/SDL_events.h>
#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace sdl3 {
enum class CameraEventType {
    Added = SDL_EVENT_CAMERA_DEVICE_ADDED,
    Removed = SDL_EVENT_CAMERA_DEVICE_REMOVED,
    Approved = SDL_EVENT_CAMERA_DEVICE_APPROVED,
    Denied = SDL_EVENT_CAMERA_DEVICE_DENIED,
};

struct CameraDeviceEvent {
    HERMES_TAG_MEMBER(sdl3::CameraEvent);
    CameraEventType type;
    std::uint64_t timestamp;
    SDL_CameraID which;
};
} // namespace sdl3

template<>
struct fmt::formatter<sdl3::CameraEventType> : formatter<std::string_view> {
    auto format(const sdl3::CameraEventType e, format_context &ctx) const {
        switch (e) {
        case sdl3::CameraEventType::Added: return formatter<std::string_view>::format("Added", ctx);
        case sdl3::CameraEventType::Removed: return formatter<std::string_view>::format("Removed", ctx);
        case sdl3::CameraEventType::Approved: return formatter<std::string_view>::format("Approved", ctx);
        case sdl3::CameraEventType::Denied: return formatter<std::string_view>::format("Denied", ctx);
        default: return formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

template<>
struct fmt::formatter<sdl3::CameraDeviceEvent> : formatter<std::string_view> {
    auto format(const sdl3::CameraDeviceEvent &e, format_context &ctx) const {
        return format_to(ctx.out(), "CameraEvent(type={}, timestamp={}, which={})", e.type, e.timestamp, e.which);
    }
};
