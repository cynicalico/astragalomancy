#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

class Indev final : public astra::Application {
public:
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

    timer_mgr.every({0.25, 0.5}, 5, [] { fmt::println("wawa"); });

    timer_mgr.until(0.25, [] {
        static int i = 5;
        fmt::println("wawa 2");
        return --i != 0;
    });
}

void Indev::update(double dt) {}

void Indev::draw() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
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
