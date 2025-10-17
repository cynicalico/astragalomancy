#include "gloo/wrap.hpp"

void gloo::clear(const astra::Color &color, GLenum clear_bits) {
    const auto gl_color = color.gl_color();
    glClearColor(gl_color.r, gl_color.g, gl_color.b, gl_color.a);
    glClear(clear_bits);
}
