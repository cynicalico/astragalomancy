#pragma once

#include "astra/core/messenger.hpp"
#include "astra/core/payloads.hpp"
#include "sdl3_raii/event_pump.hpp"
#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

#include <memory>
#include <optional>

namespace astra {
class Engine;

class Application {
public:
    Engine *engine;

    explicit Application(Engine *engine);
    virtual ~Application();

    virtual void update(double dt);

    virtual void draw();

protected:
    std::optional<Messenger::ID> callback_id_;
};

class Engine {
public:
    std::unique_ptr<sdl3::Window> window;
    std::unique_ptr<Messenger> messenger;

    Engine(const sdl3::AppInfo &app_info,
           glm::ivec2 window_size,
           const std::function<void(sdl3::WindowBuilder &)> &window_build_f);

    ~Engine();

    Engine(const Engine &other) = delete;
    Engine &operator=(const Engine &other) = delete;

    Engine(Engine &&other) noexcept;
    Engine &operator=(Engine &&other) noexcept;

    template<typename T>
        requires std::derived_from<T, Application>
    void mainloop();

    void shutdown();

private:
    bool running_{true};

    std::optional<Messenger::ID> callback_id_;
    void register_callbacks_();
    void unregister_callbacks_();
};

template<typename T>
    requires std::derived_from<T, Application>
void Engine::mainloop() {
    auto app = T(this);

    messenger->subscribe<Update>(*callback_id_, [&app](const Update *p) { app.update(p->dt); });
    messenger->subscribe<Draw>(*callback_id_, [&app](const Draw *) { app.draw(); });

    auto event_pump = sdl3::EventPump(messenger.get());

    while (running_) {
        event_pump.pump();

        messenger->publish<PreUpdate>(0.0);
        messenger->publish<Update>(0.0);
        messenger->publish<PostUpdate>(0.0);

        messenger->publish<PreDraw>();
        messenger->publish<Draw>();
        messenger->publish<PostDraw>();

        window->swap();
    }

    messenger->unsubscribe<Draw>(*callback_id_);
    messenger->unsubscribe<Update>(*callback_id_);
}
} // namespace astra
