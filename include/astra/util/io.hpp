#pragma once

#include <SDL3/SDL_surface.h>

#include <filesystem>
#include <optional>

namespace astra {
SDL_Surface *read_image_to_sdl_surface(const std::filesystem::path &path);

std::optional<std::string> read_file_to_string(const std::filesystem::path &path);
} // namespace astra
