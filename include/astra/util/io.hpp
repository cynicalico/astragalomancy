#pragma once

#include <SDL3/SDL_surface.h>

#include <filesystem>

namespace astra {
SDL_Surface *read_image_to_sdl_surface(const std::filesystem::path &path);
} // namespace astra
