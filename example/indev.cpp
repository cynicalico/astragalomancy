#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

class Indev final : public astra::Application {
public:
    astra::HSV clear_color;
    astra::TimerMgr timer_mgr;

    explicit Indev(astra::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Indev::Indev(astra::Engine *engine)
    : Application(engine),
      timer_mgr(engine->messenger.get()) {
    this->engine->messenger->subscribe<sdl3::KeyboardEvent>(*callback_id_, [this](const auto e) {
        if (e->type == sdl3::KeyboardEventType::Up && e->key == SDLK_ESCAPE)
            this->engine->shutdown();
    });

    clear_color = astra::hsv(0.0, 0.5, 0.5);

    fmt::println("{}", astra::RGB(clear_color));
    fmt::println("{}", astra::HSV(astra::RGB(clear_color)));
    fmt::println("{}", astra::HSL(astra::HSV(astra::RGB(clear_color))));
    fmt::println("{}", astra::HSV(astra::HSL(astra::HSV(astra::RGB(clear_color)))));
    fmt::println("{}", astra::RGB(astra::HSV(astra::HSL(astra::HSV(astra::RGB(clear_color))))));
}

void Indev::update(double dt) {
    clear_color.rotate(30.0f * dt);
}

void Indev::draw() {
    const auto c = clear_color.gl_color();
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
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
        window_builder.icon("assets/icon/");
    }).mainloop<Indev>();
}
