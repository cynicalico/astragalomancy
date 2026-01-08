#pragma once

#include "astra/core/messenger.hpp"
#include "astra/util/module/dear.hpp"
#include "astra/util/time.hpp"
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

    struct {
        Messenger::ID engine_callback_id;
    } internal; /// INTERNAL ENGINE USE ONLY
};
} // namespace detail

extern detail::Globals g;
} // namespace astra
