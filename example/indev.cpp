#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

class Indev final : public astra::Application {
public:
    astra::TimerMgr timer_mgr;

    std::unique_ptr<gloo::Shader> shader;

    explicit Indev(astra::Engine *engine);

    void update(double dt) override;

    void draw() override;

private:
    astra::Messenger *messenger_;

    void keyboard_event_callback_(const sdl3::KeyboardEvent *e);
};

Indev::Indev(astra::Engine *engine)
    : Application(engine),
      timer_mgr(engine->messenger.get()),
      messenger_(engine->messenger.get()) {
    shader = gloo::ShaderBuilder()
                     .add_stage_src(gloo::ShaderType::Vertex, R"glsl(
#version 460 core

in vec3 in_pos;
in vec3 in_color;

out vec3 color;

uniform mat4 projection;

void main() {
    color = in_color;
    gl_Position = projection * vec4(in_pos, 1.0);
}
)glsl")
                     .add_stage_src(gloo::ShaderType::Fragment, R"glsl(
#version 460 core

in vec3 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0);
}
)glsl")
                     .build();

    messenger_->subscribe<sdl3::KeyboardEvent>(*callback_id_, [this](const auto e) { keyboard_event_callback_(e); });
}

void Indev::update(double dt) {}

void Indev::draw() {
    gloo::clear(astra::rgb(0x0f0f0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Indev::keyboard_event_callback_(const sdl3::KeyboardEvent *e) {
    switch (e->type) {
    case sdl3::KeyboardEventType::Down:
        break;
    case sdl3::KeyboardEventType::Up:
        switch (e->key) {
        case SDLK_ESCAPE:
            this->engine->shutdown();
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
