#include "astra/gfx/2d/module/painter.hpp"

astra::Painter::Painter(Messenger *messenger, sdl3::Window *window)
    : messenger_(messenger),
      window_(window) {}

void astra::Painter::point(glm::vec2 p, const Color &color) {}

void astra::Painter::line(glm::vec2 p0, glm::vec2 p1, const Color &color) {}

void astra::Painter::triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color) {}

void astra::Painter::rectangle(glm::vec2 p, glm::vec2 size, const Color &color) {}

void astra::Painter::ellipse(glm::vec2 p, glm::vec2 size, const Color &color) {}
