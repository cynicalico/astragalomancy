#pragma once

#include "astra/core/color.hpp"
#include "gloo/gl.hpp"

namespace gloo {
void clear(const astra::Color &color, GLenum clear_bits);
} // namespace gloo
