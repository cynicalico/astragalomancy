#pragma once

#include "astra/core/color.hpp"
#include "astra/core/hermes.hpp"

#include "sdl3_raii/window.hpp"

namespace astra {
class Painter {
public:
    Painter(sdl3::Window *window);

    void point(glm::vec2 p, const Color &color);

    void line(glm::vec2 p0, glm::vec2 p1, const Color &color);

    void triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color);

    void rectangle(glm::vec2 p, glm::vec2 size, const Color &color);

    void ellipse(glm::vec2 p, glm::vec2 size, const Color &color);

private:
    sdl3::Window *window_;
};
} // namespace astra
