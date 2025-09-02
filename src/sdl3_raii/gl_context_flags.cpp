#include "sdl3_raii/gl_context_flags.hpp"

#include "astra/core/log.hpp"

sdl3::GlContextFlagsBuilder::GlContextFlagsBuilder()
    : flags_(GlContextFlags::None) {}

sdl3::GlContextFlagsBuilder &sdl3::GlContextFlagsBuilder::debug() {
    flags_ |= GlContextFlags::Debug;
    return *this;
}

sdl3::GlContextFlagsBuilder &sdl3::GlContextFlagsBuilder::forward_compatible() {
    flags_ |= GlContextFlags::ForwardCompatible;
    return *this;
}

sdl3::GlContextFlagsBuilder &sdl3::GlContextFlagsBuilder::robust_access() {
    flags_ |= GlContextFlags::RobustAccess;
    return *this;
}

sdl3::GlContextFlagsBuilder &sdl3::GlContextFlagsBuilder::reset_isolation() {
    flags_ |= GlContextFlags::ResetIsolation;
    return *this;
}

void sdl3::GlContextFlagsBuilder::set() {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, unwrap(flags_)))
        ASTRA_LOG_ERROR("Failed to set context flags: {}", SDL_GetError());
}
