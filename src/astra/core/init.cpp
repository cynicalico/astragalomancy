#include "astra/core/init.hpp"
#include "astra/core/color.hpp"
#include "astra/core/globals.hpp"
#include "astra/core/log.hpp"
#include "astra/core/payloads.hpp"
#include "astra/util/platform.hpp"
#include "astra/util/rng.hpp"
#include "gloo/init.hpp"
#include "sdl3_raii/event_pump.hpp"
#include "sdl3_raii/events/quit.hpp"
#include "sdl3_raii/gl_attr.hpp"

#include <spdlog/sinks/callback_sink.h>

#include <deque>
#include <string>

class MessengerSink final : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg &msg) override {
        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        astra::g.msg->publish<astra::LogMessage>(msg.level, fmt::to_string(formatted));
    }
    void flush_() override { /* do nothing */ }
};

void setup_engine_callbacks();

void astra::init(const sdl3::AppInfo &app_info, const std::function<sdl3::WindowBuilder()> &window_builder_f) {
    g.msg = std::make_unique<Messenger>();

    setup_engine_callbacks();

    const auto callback_sink = std::make_shared<MessengerSink>();
    callback_sink->set_pattern("[%H:%M:%S] [%L] %v");
    callback_sink->set_level(spdlog::level::trace);
    logger_sinks()->add_sink(callback_sink);

    log_platform();
    rng::log_seed();

    if (!sdl3::init(app_info)) {
        ASTRA_LOG_CRITICAL("Failed to initialize SDL3");
        throw std::runtime_error("Failed to initialize SDL3");
    }

    sdl3::GlAttr::set_context_version(4, 6);
    sdl3::GlAttr::set_context_profile(sdl3::GlProfile::Core);
#if !defined(NDEBUG)
    sdl3::GlAttr::set_context_flags().debug().set();
#endif

    g.window = window_builder_f().opengl().build();
    if (!g.window) {
        ASTRA_LOG_CRITICAL("Failed to build SDL3 window");
        throw std::runtime_error("Failed to build SDL3 window");
    }
    gloo::init();

    g.dear = std::make_unique<Dear>(*g.window);
}

void astra::shutdown() {
    g.dear.reset();
    g.window.reset();
    g.msg.reset();

    sdl3::exit();
}

struct LogFlyout_ {
    spdlog::level::level_enum level;
    std::string text;
    double acc;
};
std::deque<LogFlyout_> &log_flyouts() {
    static std::deque<LogFlyout_> flyouts;
    return flyouts;
}

void draw_debug_overlay();

void astra::mainloop() {
    g.running = true;

    while (g.running) {
        sdl3::pump_events();

        g.msg->publish<PreUpdate>(g.frame_counter.dt());
        g.msg->publish<Update>(g.frame_counter.dt());
        g.msg->publish<PostUpdate>(g.frame_counter.dt());

        g.msg->publish<PreDraw>();
        g.msg->publish<Draw>();
        draw_debug_overlay();
        g.msg->publish<PostDraw>();

        g.window->swap();

        g.frame_counter.update();
    }
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

void draw_fps_and_latency(ImDrawList *dl) {
    auto pos = ImGui::GetStyle().WindowPadding;
    float max_w = 0;

    auto fps_history = astra::g.frame_counter.fps_history();

    const auto max_fps = std::ranges::max_element(fps_history);
    std::string max_fps_text;
    if (max_fps != fps_history.end()) max_fps_text = fmt::format("MAX: {:.0f}", *max_fps);
    else max_fps_text = "MAX: -";
    max_w = std::max(
            max_w, text_with_bg(dl, pos, astra::rgb(0x000000), astra::rgb(0xffffff), 255, max_fps_text.c_str()));
    pos.y += ImGui::GetTextLineHeightWithSpacing();

    const auto avg_fps_text = fmt::format("AVG: {:.0f}", astra::g.frame_counter.fps());
    max_w = std::max(
            max_w, text_with_bg(dl, pos, astra::rgb(0x000000), astra::rgb(0xffffff), 255, avg_fps_text.c_str()));
    pos.y += ImGui::GetTextLineHeightWithSpacing();

    const auto min_fps = std::ranges::min_element(fps_history);
    std::string min_fps_text;
    if (min_fps != fps_history.end()) min_fps_text = fmt::format("MIN: {:.0f}", *min_fps);
    else min_fps_text = "MIN: -";
    max_w = std::max(
            max_w, text_with_bg(dl, pos, astra::rgb(0x000000), astra::rgb(0xffffff), 255, min_fps_text.c_str()));
    pos.y += ImGui::GetTextLineHeightWithSpacing();

    ImGui::SetCursorPos({ImGui::GetStyle().WindowPadding.x + max_w + 4.0f, ImGui::GetStyle().WindowPadding.y});
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0.0f, 0.0f));
    if (ImPlot::BeginPlot(
                "latency",
                {300, ImGui::GetTextLineHeightWithSpacing() * 3},
                ImPlotFlags_NoTitle | ImPlotFlags_NoFrame)) {
        ImPlot::SetupAxes(
                "",
                "",
                ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit,
                ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("", fps_history.data(), static_cast<int>(fps_history.size()));
        ImPlot::EndPlot();
    }
    ImPlot::PopStyleVar();
}

void draw_log_flyouts(ImDrawList *dl) {
    auto pos = ImVec2{ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y};
    for (auto &[level, text, acc]: log_flyouts()) {
        astra::RGB bg_color, fg_color;
        switch (level) {
        case spdlog::level::trace:
            bg_color = astra::rgb(0x000000);
            fg_color = astra::rgb(0x7f7f7f);
            break;
        case spdlog::level::debug:
            bg_color = astra::rgb(0x000000);
            fg_color = astra::rgb(0x5c5cff);
            break;
        case spdlog::level::info:
            bg_color = astra::rgb(0x000000);
            fg_color = astra::rgb(0x00ff00);
            break;
        case spdlog::level::warn:
            bg_color = astra::rgb(0x000000);
            fg_color = astra::rgb(0xffff00);
            break;
        case spdlog::level::err:
            bg_color = astra::rgb(0x000000);
            fg_color = astra::rgb(0xff0000);
            break;
        case spdlog::level::critical:
            bg_color = astra::rgb(0xcd0000);
            fg_color = astra::rgb(0xffffff);
            break;
        default:
            bg_color = astra::rgb(0x000000);
            fg_color = astra::rgb(0xffffff);
            break;
        }

        pos.y -= ImGui::GetTextLineHeightWithSpacing();
        text_with_bg(
                dl,
                pos,
                bg_color,
                fg_color,
                static_cast<std::uint8_t>(255.0 * std::clamp(acc, 0.0, 1.0)),
                text.c_str());
    }
}

void draw_debug_overlay() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(glm::vec2(astra::g.window->pixel_size()));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 2.0f));
    if (ImGui::Begin(
                "##overlay",
                nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoInputs)) {
        const auto draw_list = ImGui::GetWindowDrawList();
        draw_fps_and_latency(draw_list);
        draw_log_flyouts(draw_list);
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void setup_engine_callbacks() {
    using namespace astra;

    g.engine_callback_id = g.msg->get_id();
    g.msg->subscribe<sdl3::QuitEvent>(g.engine_callback_id, [&](auto) { g.running = false; });

    g.msg->subscribe<PreUpdate>(g.engine_callback_id, [&](const auto *p) {
        auto &flyouts = log_flyouts();
        for (auto &[level, text, acc]: flyouts) acc -= p->dt;
        while (!flyouts.empty() && flyouts.back().acc <= 0) flyouts.pop_back();
    });
    g.msg->subscribe<LogMessage>(g.engine_callback_id, [&](const auto *p) {
        log_flyouts().emplace_front(p->level, p->text, 5.0);
    });
}
