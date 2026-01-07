#include "astra/core/engine.hpp"
#include "astra/core/color.hpp"
#include "astra/core/log.hpp"
#include "astra/core/payloads.hpp"
#include "astra/util/platform.hpp"
#include "astra/util/rng.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/event_pump.hpp"
#include "sdl3_raii/events/quit.hpp"
#include "sdl3_raii/gl_attr.hpp"

#include <spdlog/sinks/callback_sink.h>

astra::detail::Globals astra::g;

class MessengerSink final : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        astra::g.msg->publish<astra::LogMessage>(msg.level, fmt::to_string(formatted));
    }
    void flush_() override { /* do nothing */ }
};

void astra::init(const sdl3::AppInfo &app_info, const std::function<sdl3::WindowBuilder()> &window_builder_f) {
    g.msg = std::make_unique<Messenger>();

    const auto callback_sink = std::make_shared<MessengerSink>();
    callback_sink->set_pattern("[%H:%M:%S] [%L] %v");
    callback_sink->set_level(spdlog::level::trace);
    logger_sinks()->add_sink(callback_sink);

    log_platform();
    rng::log_seed();

    if (!sdl3::init(app_info)) throw std::runtime_error("Failed to initialize SDL3");

    sdl3::GlAttr::set_context_version(4, 6);
    sdl3::GlAttr::set_context_profile(sdl3::GlProfile::Core);
#if !defined(NDEBUG)
    sdl3::GlAttr::set_context_flags().debug().set();
#endif

    g.window = window_builder_f().opengl().build();
    if (!g.window) throw std::runtime_error("Failed to build SDL3 window");
    gloo::init();

    g.dear = std::make_unique<Dear>(*g.window);
}

void astra::shutdown() {
    g.dear.reset();
    g.window.reset();
    g.msg.reset();

    sdl3::exit();
}

void astra::mainloop() {
    const auto callback_id = g.msg->get_id();
    g.msg->subscribe<sdl3::QuitEvent>(callback_id, [&](auto) { g.running = false; });

    g.running = true;

    while (g.running) {
        sdl3::pump_events();

        g.msg->publish<PreUpdate>(g.frame_counter.dt());
        g.msg->publish<Update>(g.frame_counter.dt());
        g.msg->publish<PostUpdate>(g.frame_counter.dt());

        g.msg->publish<PreDraw>();
        g.msg->publish<Draw>();
        // draw_debug_overlay_();
        g.msg->publish<PostDraw>();

        g.window->swap();

        g.frame_counter.update();
    }
}
