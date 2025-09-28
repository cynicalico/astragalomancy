#pragma once

#include "astra/core/messenger.hpp"
#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

#include <memory>
#include <optional>

namespace astra {
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

    void mainloop();

    void shutdown();

private:
    bool running_{true};

    std::optional<Messenger::ID> callback_id_;
    void register_callbacks_();
    void unregister_callbacks_();
};
} // namespace astra
