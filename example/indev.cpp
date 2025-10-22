#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

#include "glm/ext/matrix_clip_space.hpp"

class Indev final : public astra::Application {
public:
    astra::TimerMgr timer_mgr;

    std::unique_ptr<gloo::Shader> shader;
    std::unique_ptr<gloo::Buffer<float>> vertices;
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
};

Indev::Indev(astra::Engine *engine)
    : Application(engine),
      timer_mgr(messenger) {
    shader = gloo::ShaderBuilder()
                     .add_stage_path(gloo::ShaderType::Vertex, "assets/shader/triangles.vert")
                     .add_stage_path(gloo::ShaderType::Fragment, "assets/shader/triangles.frag")
                     .build();

    vertices = std::make_unique<gloo::Buffer<float>>(6 * 3 * 10, gloo::BufferFillDirection::Forward);
    // clang-format off
    vertices->add({
        100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        150.0f, 100.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        100.0f, 150.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    });
    // clang-format on

    vao = gloo::VertexArrayBuilder()
                  .attrib(*shader->try_get_attrib_location("in_pos"), 3, GL_FLOAT, GL_FALSE, 0, 0)
                  .attrib(*shader->try_get_attrib_location("in_color"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)
                  .build();

    messenger->subscribe<sdl3::KeyboardEvent>(*callback_id, [this](const auto e) { keyboard_event_callback_(e); });
}

void Indev::update(double dt) {
    vertices->sync();
}

void Indev::draw() {
    gloo::clear(astra::rgb(0x0f0f0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto projection = glm::ortho(
            0.0f, static_cast<float>(engine->window->width()), static_cast<float>(engine->window->height()), 0.0f);

    shader->use();
    shader->uniform_mat4("projection", projection);
    vao->bind();
    vertices->bind(0, 0, sizeof(float) * 6);
    glDrawArrays(GL_TRIANGLES, vertices->front() / 6, vertices->size() / 6);
    vertices->unbind(0);
    vao->unbind();
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
        window_builder.icon("assets/icon/png").fullscreen();
    }).mainloop<Indev>();
}
