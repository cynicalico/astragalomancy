#include "astra/core/engine.hpp"

#include "astra/core/log.hpp"
#include "astra/util/platform.hpp"
#include "astra/util/time.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/event_pump.hpp"
#include "sdl3_raii/events/quit.hpp"
#include "sdl3_raii/gl_attr.hpp"

#include <pcg_random.hpp>
#include <spdlog/sinks/callback_sink.h>
#include <string_view>

astra::Application::Application(Engine *engine)
    : engine(engine) {
    callback_id_ = engine->messenger->get_id();
}

astra::Application::~Application() {
    engine->messenger->release_id(*callback_id_);
    callback_id_ = std::nullopt;
}

void astra::Application::update(double dt) {}

void astra::Application::draw() {}

class MessengerSink final : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
public:
    // FIXME: Time bomb of a lifetime problem
    astra::Messenger *messenger;

    explicit MessengerSink(astra::Messenger *messenger)
        : messenger(messenger) {}

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override;

    void flush_() override { /* do nothing */ }
};

void MessengerSink::sink_it_(const spdlog::details::log_msg &msg) {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    messenger->publish<astra::LogMessage>(msg.level, fmt::to_string(formatted));
}

astra::Engine::Engine(
        const sdl3::AppInfo &app_info,
        const glm::ivec2 window_size,
        const std::function<void(sdl3::WindowBuilder &)> &window_build_f) {
    messenger = std::make_unique<Messenger>();
    register_callbacks_();

    const auto callback_sink = std::make_shared<MessengerSink>(messenger.get());
    callback_sink->set_pattern("[%H:%M:%S] [%L] %v");
    callback_sink->set_level(spdlog::level::trace);
    sinks()->add_sink(callback_sink);

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

    dear = std::make_unique<Dear>(messenger.get(), window.get());
}

astra::Engine::~Engine() {
    if (callback_id_) {
        unregister_callbacks_();

        dear.reset();
        window.reset();
        messenger.reset();

        sdl3::exit();
    }
}

astra::Engine::Engine(Engine &&other) noexcept
    : window(std::move(other.window)),
      dear(std::move(other.dear)) {
    other.unregister_callbacks_();
    messenger = std::move(other.messenger);
    register_callbacks_();
}

astra::Engine &astra::Engine::operator=(Engine &&other) noexcept {
    if (this != &other) {
        window = std::move(other.window);
        dear = std::move(other.dear);

        unregister_callbacks_();
        other.unregister_callbacks_();
        messenger = std::move(other.messenger);
        register_callbacks_();
    }
    return *this;
}

void astra::Engine::shutdown() {
    running_ = false;
}

void text_with_bg(ImDrawList *dl, ImVec2 pos, ImVec4 bg_color, ImVec4 fg_color, float alpha, const char *text) {
    const ImVec2 text_size = ImGui::CalcTextSize(text);
    dl->AddRectFilled(
            pos,
            {pos.x + text_size.x, pos.y + text_size.y + ImGui::GetStyle().ItemSpacing.y},
            ImGui::ColorConvertFloat4ToU32({bg_color.x, bg_color.y, bg_color.z, alpha}));
    dl->AddText(pos, ImGui::ColorConvertFloat4ToU32({fg_color.x, fg_color.y, fg_color.z, alpha}), text);
}

void astra::Engine::draw_debug_overlay_() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(glm::vec2(window->pixel_size()));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 2.0f));
    if (ImGui::Begin(
                "##overlay",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoInputs)) {
        const auto draw_list = ImGui::GetWindowDrawList();

        const auto fps_text = fmt::format("FPS {:.2f}", ImGui::GetIO().Framerate);
        text_with_bg(
                draw_list,
                ImGui::GetStyle().WindowPadding,
                {0.0f, 0.0f, 0.0f, 1.0f},
                {1.0f, 1.0f, 1.0f, 1.0f},
                1.0f,
                fps_text.c_str());

        ImVec2 pos = {ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y};
        for (auto &[level, text, acc]: log_flyouts_) {
            const auto lh = ImGui::GetTextLineHeightWithSpacing();
            pos.y -= lh;

            ImVec4 bg_color;
            ImVec4 fg_color;
            switch (level) {
            case spdlog::level::trace:
                bg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
                fg_color = ImVec4(0.50f, 0.50f, 0.50f, 1.0f); // 7f7f7f
                break;
            case spdlog::level::debug:
                bg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
                fg_color = ImVec4(0.36F, 0.36F, 1.0f, 1.0f); // 5c5cff
                break;
            case spdlog::level::info:
                bg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
                fg_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 00ff00
                break;
            case spdlog::level::warn:
                bg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
                fg_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // ffff00
                break;
            case spdlog::level::err:
                bg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
                fg_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // ff0000
                break;
            case spdlog::level::critical:
                bg_color = ImVec4(0.80f, 0.0f, 0.0f, 1.0f); // cd0000
                fg_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // ffffff
                break;
            default:
                bg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
                fg_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                break;
            }

            text_with_bg(
                    draw_list, pos, bg_color, fg_color, static_cast<float>(std::clamp(acc, 0.0, 1.0)), text.c_str());
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void astra::Engine::mainloop_() {
    auto event_pump = sdl3::EventPump(messenger.get());

    auto last_time = time_ns();
    while (running_) {
        event_pump.pump();

        const auto now = time_ns();
        const auto dt = (now - last_time) / 1e9;
        last_time = now;
        messenger->publish<PreUpdate>(dt);
        messenger->publish<Update>(dt);
        messenger->publish<PostUpdate>(dt);

        messenger->publish<PreDraw>();
        messenger->publish<Draw>();
        draw_debug_overlay_();
        messenger->publish<PostDraw>();

        window->swap();
    }
}

void astra::Engine::register_callbacks_() {
    callback_id_ = messenger->get_id();

    messenger->subscribe<sdl3::QuitEvent>(*callback_id_, [this](auto) { shutdown(); });

    messenger->subscribe<PreUpdate>(*callback_id_, [this](const auto *p) {
        for (auto &[level, text, acc]: log_flyouts_)
            acc -= p->dt;
        while (!log_flyouts_.empty() && log_flyouts_.back().acc <= 0)
            log_flyouts_.pop_back();
    });

    messenger->subscribe<LogMessage>(*callback_id_, [this](const auto *p) {
        log_flyouts_.emplace_front(p->level, p->text, 5.0);
    });
}

void astra::Engine::unregister_callbacks_() {
    messenger->release_id(*callback_id_);
    callback_id_ = std::nullopt;
}
