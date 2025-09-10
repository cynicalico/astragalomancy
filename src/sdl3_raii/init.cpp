#include "sdl3_raii/init.hpp"

#include "astra/core/log.hpp"

#include <SDL3/SDL.h>
#include <fmt/format.h>

const char *app_type_str(const sdl3::AppType &t) {
    switch (t) {
    case sdl3::AppType::Game: return "game";
    case sdl3::AppType::MediaPlayer: return "mediaplayer";
    case sdl3::AppType::Application: return "application";
    }
    std::unreachable();
}

bool sdl3::init(AppInfo app_info) {
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, app_info.name.c_str()))
        ASTRA_LOG_ERROR("Failed to set metadata, name: {}", SDL_GetError());

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, app_info.version.c_str()))
        ASTRA_LOG_ERROR("Failed to set metadata, version: {}", SDL_GetError());

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, app_info.identifier.c_str()))
        ASTRA_LOG_ERROR("Failed to set metadata, identifier: {}", SDL_GetError());

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, app_info.creator.c_str()))
        ASTRA_LOG_ERROR("Failed to set metadata, creator: {}", SDL_GetError());

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, app_info.copyright.c_str()))
        ASTRA_LOG_ERROR("Failed to set metadata, copyright: {}", SDL_GetError());

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, app_info.url.c_str()))
        ASTRA_LOG_ERROR("Failed to set metadata, url: {}", SDL_GetError());

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, app_type_str(app_info.type)))
        ASTRA_LOG_ERROR("Failed to set metadata, type: {}", SDL_GetError());

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        ASTRA_LOG_CRITICAL("Failed to initialize SDL3: {}", SDL_GetError());
        return false;
    }

    const auto version = SDL_GetVersion();
    ASTRA_LOG_DEBUG(
            "SDL version: {}.{}.{}",
            SDL_VERSIONNUM_MAJOR(version),
            SDL_VERSIONNUM_MINOR(version),
            SDL_VERSIONNUM_MICRO(version));

    return true;
}

void sdl3::exit() { SDL_Quit(); }
