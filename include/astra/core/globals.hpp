#pragma once

#include "astra/core/hermes.hpp"
#include "astra/gfx/shader_mgr.hpp"
#include "astra/util/module/dear.hpp"
#include "astra/util/time.hpp"
#include "sdl3_raii/window.hpp"

#include <memory>

namespace astra {
namespace detail {
struct Globals {
    std::unique_ptr<Hermes> hermes{nullptr};
    std::unique_ptr<sdl3::Window> window{nullptr};
    std::unique_ptr<Dear> dear{nullptr};

    std::unique_ptr<ShaderMgr> shaders{nullptr};

    bool running{false};
    FrameCounter frame_counter;

    struct {
        Hermes::ID hermes_id;
    } internal; /// INTERNAL ENGINE USE ONLY
};
} // namespace detail

extern detail::Globals g;
} // namespace astra
