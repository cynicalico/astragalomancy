#pragma once

#if !defined(SPDLOG_ACTIVE_LEVEL)
#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#endif

#include "astra/core/types.hpp"

#include <fmt/chrono.h> // Allow logging chrono types
#include <fmt/ranges.h> // Allow logging ranges (vector, etc.)
#include <fmt/std.h> // Allow logging STL types
#include <fmt/xchar.h> // Allow logging wchar

#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace astra {
std::shared_ptr<spdlog::sinks::dist_sink_mt> sinks();

std::shared_ptr<spdlog::logger> logger();
} // namespace astra

#define ASTRA_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_INFO(...) SPDLOG_LOGGER_INFO(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_WARN(...) SPDLOG_LOGGER_WARN(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(astra::logger(), __VA_ARGS__)
#define ASTRA_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(astra::logger(), __VA_ARGS__)

template<>
struct fmt::formatter<astra::int128_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::int256_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::int512_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::int1024_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::uint128_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::uint256_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::uint512_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::uint1024_t> : ostream_formatter {};

template<>
struct fmt::formatter<astra::ap_int> : ostream_formatter {};
