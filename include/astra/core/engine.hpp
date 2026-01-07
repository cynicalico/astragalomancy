#pragma once

#include "astra/core/messenger.hpp"
#include "astra/core/payloads.hpp"
#include "astra/util/module/dear.hpp"
#include "astra/util/time.hpp"

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
    std::optional<Messenger::ID> callback_id;
};

class Engine {
public:
    std::unique_ptr<sdl3::Window> window;
    std::unique_ptr<Dear> dear;

    FrameCounter frame_counter;

    Engine(const sdl3::AppInfo &app_info, const std::function<sdl3::WindowBuilder()> &window_builder_f);
    ~Engine();

    Engine(const Engine &other) = delete;
    Engine &operator=(const Engine &other) = delete;

    Engine(Engine &&other) = delete;
    Engine &operator=(Engine &&other) = delete;

    template<typename T, typename... Args>
        requires std::derived_from<T, Application>
    void mainloop(Args &&...args);

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

template<typename T, typename... Args>
    requires std::derived_from<T, Application>
void Engine::mainloop(Args &&...args) {
    auto app = T(this, std::forward<Args>(args)...);

    Messenger::instance().subscribe<Update>(*callback_id_, [&app](const Update *p) { app.update(p->dt); });
    Messenger::instance().subscribe<Draw>(*callback_id_, [&app](const Draw *) { app.draw(); });

    mainloop_();

    Messenger::instance().unsubscribe<Draw>(*callback_id_);
    Messenger::instance().unsubscribe<Update>(*callback_id_);
}
} // namespace astra
