#pragma once

#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

#include <functional>

namespace astra {
void init(const sdl3::AppInfo &app_info, const std::function<sdl3::WindowBuilder()> &window_builder_f);
void shutdown();

void mainloop();
} // namespace astra
