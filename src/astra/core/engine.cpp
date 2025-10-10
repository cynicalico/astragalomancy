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

astra::Engine::Engine(
        const sdl3::AppInfo &app_info,
        const glm::ivec2 window_size,
        const std::function<void(sdl3::WindowBuilder &)> &window_build_f) {
    const auto callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>(
            [&](const spdlog::details::log_msg &msg) { messenger->publish<LogMessage>(msg); });
    callback_sink->set_level(spdlog::level::trace);
    sinks()->add_sink(callback_sink);

    messenger = std::make_unique<Messenger>();
    register_callbacks_();

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

void text_with_bg(ImDrawList *dl, ImVec2 pos, float alpha, const char *text) {
    const ImVec2 text_size = ImGui::CalcTextSize(text);
    const ImU32 bg_color = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.0f, alpha});
    const ImU32 fg_color = ImGui::ColorConvertFloat4ToU32({1.0f, 1.0f, 1.0f, alpha});
    dl->AddRectFilled(pos, {pos.x + text_size.x, pos.y + text_size.y + ImGui::GetStyle().ItemSpacing.y}, bg_color);
    dl->AddText(pos, fg_color, text);
}

void astra::Engine::draw_debug_overlay_() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(glm::vec2(window->pixel_size()));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
    if (ImGui::Begin(
                "##overlay",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoInputs)) {
        const auto draw_list = ImGui::GetWindowDrawList();

        const auto fps_text = fmt::format("{:.2f}fps", ImGui::GetIO().Framerate);
        text_with_bg(draw_list, ImGui::GetStyle().WindowPadding, 1.0f, fps_text.c_str());

        ImVec2 pos = {ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y};
        for (auto &[text, acc]: log_flyouts_) {
            const auto lh = ImGui::GetTextLineHeightWithSpacing();
            pos.y -= lh;
            const auto alpha = static_cast<float>(std::clamp(acc, 0.0, 1.0));
            text_with_bg(draw_list, pos, alpha, text.c_str());
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
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
        for (auto &[text, acc]: log_flyouts_)
            acc -= p->dt;
        while (!log_flyouts_.empty() && log_flyouts_.back().acc <= 0)
            log_flyouts_.pop_back();
    });

    messenger->subscribe<LogMessage>(*callback_id_, [this](const auto *p) {
        log_flyouts_.emplace_front(std::string(p->msg.payload.data(), p->msg.payload.size()), 3.0);
    });
}

void astra::Engine::unregister_callbacks_() {
    messenger->release_id(*callback_id_);
    callback_id_ = std::nullopt;
}
