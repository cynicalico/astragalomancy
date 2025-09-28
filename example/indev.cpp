#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

class Indev {
public:
    astra::Engine engine;

    explicit Indev(astra::Engine engine);
    ~Indev();

    Indev(const Indev &other) = delete;
    Indev &operator=(Indev &&other) noexcept = delete;

    Indev(Indev &&other) noexcept = delete;
    Indev &operator=(const Indev &other) = delete;

    void mainloop();

private:
    void update_(double dt);

    void draw_();

    std::optional<astra::Messenger::ID> callback_id_;
    void register_callbacks_();
    void unregister_callbacks_();
};

Indev::Indev(astra::Engine engine)
    : engine(std::move(engine)) {
    register_callbacks_();
}

Indev::~Indev() {
    unregister_callbacks_();
}

void Indev::mainloop() {
    engine.mainloop();
}

void Indev::update_(double dt) {}

void Indev::draw_() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Indev::register_callbacks_() {
    callback_id_ = engine.messenger->get_id();

    engine.messenger->subscribe<sdl3::KeyboardEvent>(*callback_id_, [this](const auto e) {
        if (e->type == sdl3::KeyboardEventType::Up && e->key == SDLK_ESCAPE)
            engine.shutdown();
    });

    engine.messenger->subscribe<astra::Update>(*callback_id_, [this](const auto e) { update_(e->dt); });

    engine.messenger->subscribe<astra::Draw>(*callback_id_, [this](auto) { draw_(); });
}

void Indev::unregister_callbacks_() {
    engine.messenger->release_id(*callback_id_);
    callback_id_ = std::nullopt;
}

int main(int, char *[]) {
    auto engine = astra::Engine(
            sdl3::AppInfo{
                    .name = "Indev",
                    .version = "0.0.1",
                    .identifier = "gay.cynicalico.indev",
                    .creator = "cynicalico",
                    .copyright = "This is free and unencumbered software released into the public domain.",
                    .url = "https://github.com/cynicalico/astragalomancy",
                    .type = sdl3::AppType::Game,
            },
            {1280, 720},
            [](sdl3::WindowBuilder &window_builder) { window_builder.icon("assets/icon/"); });

    Indev(std::move(engine)).mainloop();
}
