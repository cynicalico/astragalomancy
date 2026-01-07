// TODO: Rename this file to something that makes more sense

#pragma once

#include "astra/core/messenger.hpp"
#include "astra/util/module/dear.hpp"
#include "astra/util/time.hpp"

#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

#include <memory>

namespace astra {
namespace detail {
struct Globals {
    std::unique_ptr<Messenger> msg{nullptr};
    std::unique_ptr<sdl3::Window> window{nullptr};
    std::unique_ptr<Dear> dear{nullptr};

    bool running{false};
    FrameCounter frame_counter;
};
} // namespace detail

extern detail::Globals g;

void init(const sdl3::AppInfo &app_info, const std::function<sdl3::WindowBuilder()> &window_builder_f);
void shutdown();

void mainloop();
} // namespace astra
