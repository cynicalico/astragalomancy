#include "astra/astra.hpp"

class Indev {
public:
    std::optional<astra::Messenger::ID> callback_id;

    Indev();
    ~Indev();

    void update(double dt);
    void draw();

private:
    void keyboard_event_callback_(const sdl3::KeyboardEvent *e);
    void mouse_event_callback_(const sdl3::MouseButtonEvent *e);
};

Indev::Indev() {
    callback_id = astra::g.msg->get_id();
    astra::g.msg->subscribe<astra::Update>(*callback_id, [this](const auto e) { update(e->dt); });
    astra::g.msg->subscribe<astra::Draw>(*callback_id, [this](const auto) { draw(); });
    astra::g.msg->subscribe<sdl3::KeyboardEvent>(*callback_id, [this](const auto e) { keyboard_event_callback_(e); });
    astra::g.msg->subscribe<sdl3::MouseButtonEvent>(*callback_id, [this](const auto e) { mouse_event_callback_(e); });
}

Indev::~Indev() {
    astra::g.msg->release_id(*callback_id);
    callback_id = std::nullopt;
}

void Indev::update(double dt) {}

void Indev::draw() {
    gloo::clear(astra::rgb(0x0f0f0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Indev::keyboard_event_callback_(const sdl3::KeyboardEvent *e) {
    switch (e->type) {
    case sdl3::KeyboardEventType::Down: break;
    case sdl3::KeyboardEventType::Up:
        switch (e->key) {
        case SDLK_ESCAPE: astra::g.running = false; break;
        default:;
        }
        break;
    default: std::unreachable();
    }
}

void Indev::mouse_event_callback_(const sdl3::MouseButtonEvent *e) {
    switch (e->type) {
    case sdl3::MouseButtonEventType::Down: break;
    case sdl3::MouseButtonEventType::Up: break;
    default: std::unreachable();
    }
}

int main(int, char *[]) {
    try {
        const auto app_info = sdl3::AppInfo{
                .name = "Indev",
                .version = "0.0.1",
                .identifier = "gay.cynicalico.indev",
                .creator = "cynicalico",
                .copyright = "This is free and unencumbered software released into the public domain.",
                .url = "https://github.com/cynicalico/astragalomancy",
                .type = sdl3::AppType::Game,
        };
        astra::init(app_info, [&] {
            return std::move(
                    sdl3::WindowBuilder(app_info.name, {1920, 1080}).resizable().icon("assets/icon/light/png"));
        });

        { // scope to destroy Indev before shutdown
            const auto app = Indev();
            astra::mainloop();
        }

        astra::shutdown();
    } catch (const std::exception &e) {
        fmt::println("");
        fmt::println(stderr, "Exception: {}", e);
        return 1;
    }

    return 0;
}
