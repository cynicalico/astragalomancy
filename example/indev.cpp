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
    });

    ASTRA_LOG_TRACE("Indev initialized");
}

void Indev::update(double dt) {}

void Indev::draw() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
