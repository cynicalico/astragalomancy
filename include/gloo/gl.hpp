#pragma once

#include <glad/gl.h>

namespace gloo {
#if !defined(NDEBUG)
void debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam);
#endif
} // namespace gloo
