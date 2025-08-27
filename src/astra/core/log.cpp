#include "astra/core/log.hpp"

#include "astra/util/platform.hpp"
#include "astra/util/time.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>

#if defined(ASTRA_PLATFORM_WINDOWS)
#include "windows.h"
#endif

std::shared_ptr<spdlog::sinks::dist_sink_mt> astra::sinks() {
    static auto s = std::make_shared<spdlog::sinks::dist_sink_mt>();
    return s;
}

std::shared_ptr<spdlog::logger> astra::logger() {
    static auto logger = std::invoke([&] {
        auto s = sinks();

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_color_mode(spdlog::color_mode::always);
        s->add_sink(console_sink);

        auto filename = fmt::format("{}.log", timestamp());
        auto path = (std::filesystem::path(".log") / filename).string();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path);
        s->add_sink(file_sink);

        auto l = std::make_shared<spdlog::logger>("astra", s);
        l->set_level(spdlog::level::trace);

#if defined(ASTRA_PLATFORM_WINDOWS)
        SetConsoleOutputCP(CP_UTF8);
#endif

        return l;
    });

    return logger;
}
