#include "gloo/gl.hpp"

#include "astra/core/log.hpp"

#if !defined(NDEBUG)
void gloo::gl_debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam) {
    const std::string source_str = std::invoke([source] {
        switch (source) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION: return "Application";
        case GL_DEBUG_SOURCE_OTHER: return "Other";
        default: std::unreachable();
        }
    });

    const std::string type_str = std::invoke([type] {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR: return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
        case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
        case GL_DEBUG_TYPE_MARKER: return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
        case GL_DEBUG_TYPE_OTHER: return "Other";
        default: std::unreachable();
        }
    });

    const auto text = fmt::format("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: ASTRA_LOG_ERROR(text); break;
    case GL_DEBUG_SEVERITY_MEDIUM: ASTRA_LOG_WARN(text); break;
    case GL_DEBUG_SEVERITY_LOW: ASTRA_LOG_DEBUG(text); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: ASTRA_LOG_TRACE(text); break;
    default: std::unreachable();
    }
}
#endif
