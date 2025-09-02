#include "gloo/init.hpp"

#include "astra/core/log.hpp"
#include "gloo/gl.hpp"

#include <SDL3/SDL.h>

bool gloo::init() {
    if (gladLoadGL(SDL_GL_GetProcAddress) == 0) {
        ASTRA_LOG_ERROR("Failed to initialize OpenGL context");
        return false;
    }

#if !defined(NDEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_message_callback, nullptr);
#endif

    ASTRA_LOG_DEBUG(
            "OpenGL v{}, vendor - {}, renderer - {}",
            reinterpret_cast<const char *>(glGetString(GL_VERSION)),
            reinterpret_cast<const char *>(glGetString(GL_VENDOR)),
            reinterpret_cast<const char *>(glGetString(GL_RENDERER)));

    return true;
}
