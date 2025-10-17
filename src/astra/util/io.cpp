#include "astra/util/io.hpp"

#include "astra/core/log.hpp"
#include "astra/util/platform.hpp"

#define STB_IMAGE_IMPLEMENTATION
#if defined(ASTRA_PLATFORM_WINDOWS)
#define STBI_WINDOWS_UTF8
#include <windows.h>
#endif
#include "stb_image.h"

#include <cstdio>

SDL_Surface *astra::read_image_to_sdl_surface(const std::filesystem::path &path) {
    FILE *fp;
#if defined(ASTRA_PLATFORM_WINDOWS)
    char buffer[1024];
    if (stbi_convert_wchar_to_utf8(buffer, 1024, path.c_str()) == 0) {
        switch (GetLastError()) {
        case ERROR_INSUFFICIENT_BUFFER:
            ASTRA_LOG_ERROR("Failed to convert '{}' to UTF-8: Insufficient buffer", path);
            break;
        case ERROR_INVALID_FLAGS:
            ASTRA_LOG_ERROR("Failed to convert '{}' to UTF-8: Invalid flags", path);
            break;
        case ERROR_INVALID_PARAMETER:
            ASTRA_LOG_ERROR("Failed to convert '{}' to UTF-8: Invalid parameter", path);
            break;
        case ERROR_NO_UNICODE_TRANSLATION:
            ASTRA_LOG_ERROR("Failed to convert '{}' to UTF-8: No unicode translation", path);
            break;
        default:
            std::unreachable();
        }
        return nullptr;
    }

    if (fopen_s(&fp, buffer, "rb") != 0) {
        ASTRA_LOG_ERROR("Failed to open file: '{}'", path);
        return nullptr;
    }
#else
    fp = fopen(path.c_str(), "rb");
    if (!fp) {
        ASTRA_LOG_ERROR("Failed to open file: '{}'", path);
        return nullptr;
    }
#endif

    int w, h, channels;
    const auto bytes = stbi_load_from_file(fp, &w, &h, &channels, STBI_rgb_alpha);
    if (!bytes) {
        ASTRA_LOG_ERROR("Failed to load image from '{}': {}", path, stbi_failure_reason());
        return nullptr;
    }

    const auto surf = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, bytes, w * STBI_rgb_alpha);
    if (!surf)
        ASTRA_LOG_ERROR("Failed to create surface from '{}': {}", path, SDL_GetError());
    return surf;
}

std::optional<std::string> astra::read_file_to_string(const std::filesystem::path &path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        ASTRA_LOG_ERROR("Failed to open file: '{}'", path);
        return std::nullopt;
    }

    std::string contents;
    ifs.seekg(0, std::ios::end);
    contents.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    contents.assign(std::istreambuf_iterator(ifs), std::istreambuf_iterator<char>());

    return contents;
}
