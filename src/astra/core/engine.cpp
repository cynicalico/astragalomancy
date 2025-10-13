#include "astra/core/engine.hpp"

#include "astra/core/color.hpp"
#include "astra/core/log.hpp"
#include "astra/util/platform.hpp"
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

float text_with_bg(
        ImDrawList *dl,
        ImVec2 pos,
        const astra::Color &bg_color,
        const astra::Color &fg_color,
        std::uint8_t alpha,
        const char *text) {
    const ImVec2 text_size = ImGui::CalcTextSize(text);
    dl->AddRectFilled(
            pos,
            {pos.x + text_size.x, pos.y + text_size.y + ImGui::GetStyle().ItemSpacing.y},
            bg_color.imgui_color_u32(alpha));
    dl->AddText(pos, fg_color.imgui_color_u32(alpha), text);
    return text_size.x;
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

        auto pos = ImGui::GetStyle().WindowPadding;
        const auto lh = ImGui::GetTextLineHeightWithSpacing();

        float max_w = 0;

        auto fps_history = frame_counter.fps_history();

        const auto max_fps = std::ranges::max_element(fps_history);
        std::string max_fps_text;
        if (max_fps != fps_history.end())
            max_fps_text = fmt::format("MAX: {:.0f}", *max_fps);
        else
            max_fps_text = "MAX: -";
        max_w = std::max(max_w, text_with_bg(draw_list, pos, rgb(0x000000), rgb(0xffffff), 255, max_fps_text.c_str()));
        pos.y += lh;

        const auto avg_fps_text = fmt::format("AVG: {:.0f}", frame_counter.fps());
        max_w = std::max(max_w, text_with_bg(draw_list, pos, rgb(0x000000), rgb(0xffffff), 255, avg_fps_text.c_str()));
        pos.y += lh;

        const auto min_fps = std::ranges::min_element(fps_history);
        std::string min_fps_text;
        if (min_fps != fps_history.end())
            min_fps_text = fmt::format("MIN: {:.0f}", *min_fps);
        else
            min_fps_text = "MIN: -";
        max_w = std::max(max_w, text_with_bg(draw_list, pos, rgb(0x000000), rgb(0xffffff), 255, min_fps_text.c_str()));
        pos.y += lh;

        ImGui::SetCursorPos({ImGui::GetStyle().WindowPadding.x + max_w + 4.0f, ImGui::GetStyle().WindowPadding.y});
        ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
        if (ImPlot::BeginPlot("latency", {300, lh * 3}, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame)) {
            ImPlot::SetupAxes(
                    "",
                    "",
                    ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit,
                    ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("", fps_history.data(), fps_history.size());
            ImPlot::EndPlot();
        }
        ImPlot::PopStyleVar();

        pos = {ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y};
        for (auto &[level, text, acc]: log_flyouts_) {
            pos.y -= lh;

            RGB bg_color, fg_color;
            switch (level) {
            case spdlog::level::trace:
                bg_color = rgb(0x000000);
                fg_color = rgb(0x7f7f7f);
                break;
            case spdlog::level::debug:
                bg_color = rgb(0x000000);
                fg_color = rgb(0x5c5cff);
                break;
            case spdlog::level::info:
                bg_color = rgb(0x000000);
                fg_color = rgb(0x00ff00);
                break;
            case spdlog::level::warn:
                bg_color = rgb(0x000000);
                fg_color = rgb(0xffff00);
                break;
            case spdlog::level::err:
                bg_color = rgb(0x000000);
                fg_color = rgb(0xff0000);
                break;
            case spdlog::level::critical:
                bg_color = rgb(0xcd0000);
                fg_color = rgb(0xffffff);
                break;
            default:
                bg_color = rgb(0x000000);
                fg_color = rgb(0xffffff);
                break;
            }

            text_with_bg(
                    draw_list,
                    pos,
                    bg_color,
                    fg_color,
                    static_cast<std::uint8_t>(255.0 * std::clamp(acc, 0.0, 1.0)),
                    text.c_str());
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void astra::Engine::mainloop_() {
    auto event_pump = sdl3::EventPump(messenger.get());

    while (running_) {
        event_pump.pump();

        messenger->publish<PreUpdate>(frame_counter.dt());
        messenger->publish<Update>(frame_counter.dt());
        messenger->publish<PostUpdate>(frame_counter.dt());

        messenger->publish<PreDraw>();
        messenger->publish<Draw>();
        draw_debug_overlay_();
        messenger->publish<PostDraw>();

        window->swap();

        frame_counter.update();
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
