#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

#include "astra/gfx/2d/module/painter.hpp"
#include "glm/ext/matrix_clip_space.hpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 circle;
};

class Indev final : public astra::Application {
public:
    astra::TimerMgr timer_mgr;
    astra::Painter painter;

    std::unique_ptr<gloo::Shader> shader;
    std::unique_ptr<gloo::Buffer<Vertex>> vbo;
    std::unique_ptr<gloo::VertexArray> vao;

    explicit Indev(astra::Engine *engine);

    Indev(const Indev &other) = delete;
    Indev &operator=(const Indev &other) = delete;

    Indev(Indev &&other) noexcept = delete;
    Indev &operator=(Indev &&other) noexcept = delete;

    void update(double dt) override;

    void draw() override;

private:
    void keyboard_event_callback_(const sdl3::KeyboardEvent *e);
    void mouse_event_callback_(const sdl3::MouseButtonEvent *e);
};

Indev::Indev(astra::Engine *engine)
    : Application(engine),
      timer_mgr(messenger),
      painter(messenger, engine->window.get()) {
    messenger->subscribe<sdl3::KeyboardEvent>(*callback_id, [this](const auto e) { keyboard_event_callback_(e); });
    messenger->subscribe<sdl3::MouseButtonEvent>(*callback_id, [this](const auto e) { mouse_event_callback_(e); });

    shader = gloo::ShaderBuilder()
                     .add_stage_src(gloo::ShaderType::Vertex, R"(
#version 460 core
in vec3 in_pos;
in vec3 in_circle;

out vec3 circle;

uniform mat4 projection;

void main() {
    circle = in_circle;
    gl_Position = projection * vec4(in_pos, 1.0);
}
)")
                     .add_stage_src(gloo::ShaderType::Fragment, R"(
#version 460 core
in vec3 circle;

out vec4 FragColor;

float sdf_circle(vec2 p, float r) {
    return length(p) - r;
}

void main() {
    vec2 sdf_p = vec2(gl_FragCoord.x - circle.x, gl_FragCoord.y - circle.y);
    float distance = sdf_circle(sdf_p, circle.z);
    if (distance > 0) {
        discard;
    }
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
)")
                     .build();

    vbo = std::make_unique<gloo::Buffer<Vertex>>(6);
    const auto center = engine->window->size() / 2;
    const auto radius = 25;
    vbo->add({
            {{center.x - radius, center.y - radius, 0}, {center.x, center.y, radius}},
            {{center.x + radius, center.y - radius, 0}, {center.x, center.y, radius}},
            {{center.x - radius, center.y + radius, 0}, {center.x, center.y, radius}},
            {{center.x + radius, center.y - radius, 0}, {center.x, center.y, radius}},
            {{center.x + radius, center.y + radius, 0}, {center.x, center.y, radius}},
            {{center.x - radius, center.y + radius, 0}, {center.x, center.y, radius}},
    });
    vbo->sync();

    const auto pos_loc = shader->try_get_attrib_location("in_pos");
    const auto circle_loc = shader->try_get_attrib_location("in_circle");
    vao = gloo::VertexArrayBuilder()
                  .attrib(*pos_loc, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos), 0)
                  .attrib(*circle_loc, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, circle), 0)
                  .build();
}

void Indev::update(double dt) {}

void Indev::draw() {
    gloo::clear(astra::rgb(0x0f0f0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->uniform_mat4(
            "projection",
            glm::ortho(
                    0.0f,
                    static_cast<float>(engine->window->width()),
                    0.0f,
                    static_cast<float>(engine->window->height())));

    vao->bind();
    vbo->bind(0, vbo->front() * sizeof(Vertex), sizeof(Vertex));
    glDrawArrays(GL_TRIANGLES, 0, vbo->size());
}

void Indev::keyboard_event_callback_(const sdl3::KeyboardEvent *e) {
    switch (e->type) {
    case sdl3::KeyboardEventType::Down:
        break;
    case sdl3::KeyboardEventType::Up:
        switch (e->key) {
        case SDLK_ESCAPE:
            engine->shutdown();
            break;
        default:;
        }
        break;
    default:
        std::unreachable();
    }
}

void Indev::mouse_event_callback_(const sdl3::MouseButtonEvent *e) {
    switch (e->type) {
    case sdl3::MouseButtonEventType::Down:
        break;
    case sdl3::MouseButtonEventType::Up:
        break;
    default:
        std::unreachable();
    }
}

int main(int, char *[]) {
    const auto app_info = sdl3::AppInfo{
            .name = "Indev",
            .version = "0.0.1",
            .identifier = "gay.cynicalico.indev",
            .creator = "cynicalico",
            .copyright = "This is free and unencumbered software released into the public domain.",
            .url = "https://github.com/cynicalico/astragalomancy",
            .type = sdl3::AppType::Game,
    };

    astra::Engine(app_info, {1280, 720}, [](sdl3::WindowBuilder &window_builder) {
        window_builder.icon("assets/icon/png");
    }).mainloop<Indev>();
}
