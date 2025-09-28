#include "astra/core/engine.hpp"

#include "astra/util/platform.hpp"
#include "gloo/init.hpp"
#include "sdl3_raii/event_pump.hpp"
#include "sdl3_raii/events/quit.hpp"
#include "sdl3_raii/gl_attr.hpp"

#include <pcg_random.hpp>

astra::Engine::Engine(
        const sdl3::AppInfo &app_info,
        const glm::ivec2 window_size,
        const std::function<void(sdl3::WindowBuilder &)> &window_build_f) {
    log_platform();

    if (!sdl3::init(app_info))
        throw std::runtime_error("Failed to initialize SDL3");

    sdl3::GlAttr::set_context_version(4, 6);
    sdl3::GlAttr::set_context_profile(sdl3::GlProfile::Core);
#if !defined(NDEBUG)
    sdl3::GlAttr::set_context_flags().debug().set();
#endif

    auto window_builder = sdl3::WindowBuilder(app_info.name, window_size);
    window_build_f(window_builder);
    window = window_builder.opengl().build();
    if (!window)
        throw std::runtime_error("Failed to build SDL3 window");
    gloo::init();

    messenger = std::make_unique<Messenger>();

    register_callbacks_();
}

astra::Engine::~Engine() {
    if (callback_id_)
        unregister_callbacks_();

    if (window)
        sdl3::exit();
}

astra::Engine::Engine(Engine &&other) noexcept
    : window(std::move(other.window)) {
    other.unregister_callbacks_();
    messenger = std::move(other.messenger);

    register_callbacks_();
}

astra::Engine &astra::Engine::operator=(Engine &&other) noexcept {
    if (this != &other) {
        window = std::move(other.window);

        other.unregister_callbacks_();
        messenger = std::move(other.messenger);

        register_callbacks_();
    }
    return *this;
}

void astra::Engine::mainloop() {
    auto event_pump = sdl3::EventPump(messenger.get());

    while (running_) {
        event_pump.pump();

        messenger->publish<PreUpdate>(0.0);
        messenger->publish<Update>(0.0);
        messenger->publish<PostUpdate>(0.0);

        messenger->publish<PreDraw>();
        messenger->publish<Draw>();
        messenger->publish<PostDraw>();

        messenger->publish<Present>();
    }
}

void astra::Engine::shutdown() {
    running_ = false;
}

void astra::Engine::register_callbacks_() {
    callback_id_ = messenger->get_id();

    messenger->subscribe<sdl3::QuitEvent>(*callback_id_, [this](auto) { shutdown(); });

    messenger->subscribe<Present>(*callback_id_, [this](auto) { window->swap(); });
}

void astra::Engine::unregister_callbacks_() {
    messenger->release_id(*callback_id_);
    callback_id_ = std::nullopt;
}
