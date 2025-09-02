#pragma once

#include "astra/util/enum_class_helpers.hpp"

#include <SDL3/SDL_video.h>

namespace sdl3 {
enum class GlContextFlags : int {
    None = 0,
    Debug = SDL_GL_CONTEXT_DEBUG_FLAG,
    ForwardCompatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
    RobustAccess = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
    ResetIsolation = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
};

class GlContextFlagsBuilder {
public:
    GlContextFlagsBuilder();

    GlContextFlagsBuilder &debug();
    GlContextFlagsBuilder &forward_compatible();
    GlContextFlagsBuilder &robust_access();
    GlContextFlagsBuilder &reset_isolation();

    void set();

private:
    GlContextFlags flags_;
};
} // namespace sdl3

ENUM_CLASS_ENABLE_BITOPS(sdl3::GlContextFlags);
