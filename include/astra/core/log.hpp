#pragma once

#include "astra/core/types.hpp"

#include <fmt/chrono.h> // Allow logging chrono types
#include <fmt/ranges.h> // Allow logging ranges (vector, etc.)
#include <fmt/std.h> // Allow logging STL types
#include <fmt/xchar.h> // Allow logging wchar

#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace astra {
std::shared_ptr<spdlog::sinks::dist_sink_mt> logger_sinks();

std::shared_ptr<spdlog::logger> logger();
} // namespace astra

#define ASTRA_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_INFO(...) SPDLOG_LOGGER_INFO(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_WARN(...) SPDLOG_LOGGER_WARN(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(astra::logger(), __VA_ARGS__)
