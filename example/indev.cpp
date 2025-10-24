#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

#include "astra/gfx/2d/module/painter.hpp"
#include "glm/ext/matrix_clip_space.hpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec4 circle;
};

class Indev final : public astra::Application {
public:
    astra::TimerMgr timer_mgr;
    astra::Painter painter;

    std::unique_ptr<gloo::Shader> shader;
    std::unique_ptr<gloo::Buffer<Vertex>> vbo;
    std::unique_ptr<gloo::VertexArray> vao;

    float radius{25.0f};
    float thickness{1.0f};

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
                     .add_stage_path(gloo::ShaderType::Vertex, "assets/shader/sdf_circle.vert")
                     .add_stage_path(gloo::ShaderType::Fragment, "assets/shader/sdf_circle.frag")
                     .build();

    vbo = std::make_unique<gloo::Buffer<Vertex>>(6);

    const auto pos_loc = shader->try_get_attrib_location("in_pos");
    const auto circle_loc = shader->try_get_attrib_location("in_circle");
    vao = gloo::VertexArrayBuilder()
                  .attrib(*pos_loc, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos), 0)
                  .attrib(*circle_loc, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, circle), 0)
                  .build();
}

void Indev::update(double dt) {}

void Indev::draw() {
    gloo::clear(astra::rgb(0x0f0f0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (ImGui::Begin("Controls")) {
        ImGui::SliderFloat("radius", &radius, 1.0f, 250.0f, "%.0f");
        ImGui::SliderFloat("thickness", &thickness, 0.0f, 10.0f, "%.0f");
    }
    ImGui::End();

    const auto center = engine->window->size() / 2;
    const auto p0 = glm::vec3{center.x - radius - thickness, center.y - radius - thickness, 0};
    const auto p1 = glm::vec3{center.x + radius + thickness, center.y - radius - thickness, 0};
    const auto p2 = glm::vec3{center.x - radius - thickness, center.y + radius + thickness, 0};
    const auto p3 = glm::vec3{center.x + radius + thickness, center.y + radius + thickness, 0};
    vbo->clear();
    vbo->add({
            {p0, {center.x, center.y, radius, thickness}},
            {p1, {center.x, center.y, radius, thickness}},
            {p2, {center.x, center.y, radius, thickness}},
            {p1, {center.x, center.y, radius, thickness}},
            {p3, {center.x, center.y, radius, thickness}},
            {p2, {center.x, center.y, radius, thickness}},
    });
    vbo->sync();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    vbo->unbind(0);
    vao->unbind();

    glDisable(GL_BLEND);
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
