#pragma once

#include <glad/gl.h>

#if !defined(NDEBUG)
namespace gloo {
void gl_debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam);
} // namespace gloo
#endif
