#pragma once

#include "astra/core/messenger.hpp"
#include "astra/core/payloads.hpp"
#include "astra/util/module/dear.hpp"
#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

#include <deque>
#include <memory>
#include <optional>
#include <string>

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
    std::unique_ptr<Messenger> messenger;
    std::unique_ptr<sdl3::Window> window;
    std::unique_ptr<Dear> dear;

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

    struct LogFlyout_ {
        spdlog::level::level_enum level;
        std::string text;
        double acc;
    };
    std::deque<LogFlyout_> log_flyouts_;
    void draw_debug_overlay_();

    void mainloop_();

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

    mainloop_();

    messenger->unsubscribe<Draw>(*callback_id_);
    messenger->unsubscribe<Update>(*callback_id_);
}
} // namespace astra
