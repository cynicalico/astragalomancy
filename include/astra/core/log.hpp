#pragma once

#if !defined(SPDLOG_ACTIVE_LEVEL)
#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#endif

#include "astra/core/types.hpp"
#include "astra/util/is_any_of.hpp"

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

template<typename T>
    requires astra::IsAnyOf<
            T,
            astra::int128_t,
            astra::int256_t,
            astra::int512_t,
            astra::int1024_t,
            astra::uint128_t,
            astra::uint256_t,
            astra::uint512_t,
            astra::uint1024_t,
            astra::ap_int>
struct fmt::formatter<T> {
    char format_type = 'd';
    bool show_base = false;

    constexpr auto parse(const format_parse_context &ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it == '#') {
            show_base = true;
            ++it;
        }
        if (it != ctx.end() && (*it == 'd' || *it == 'x' || *it == 'X' || *it == 'o' || *it == 'b' || *it == 'B'))
            format_type = *it++;
        if (it != ctx.end() && *it != '}')
            throw format_error("invalid format");
        return it;
    }

private:
    std::string format_binary(const T &value) const {
        if (value == 0)
            return "0";

        std::string binary;
        T n = abs(value);
        while (n > 0) {
            binary += "01"[static_cast<std::size_t>(n % 2)];
            n /= 2;
        }
        if (value < 0)
            binary += '-';

        std::ranges::reverse(binary);
        return binary;
    }

public:
    template<typename FormatContext>
    auto format(const T &value, FormatContext &ctx) const -> decltype(ctx.out()) {
        std::ostringstream oss;
        switch (format_type) {
        case 'x':
            if (show_base)
                oss << "0x";
            oss << std::hex << std::nouppercase << value;
            break;
        case 'X':
            if (show_base)
                oss << "0X";
            oss << std::hex << std::uppercase << value;
            break;
        case 'o':
            if (show_base)
                oss << "0o";
            oss << std::oct << value;
            break;
        case 'b':
            if (show_base)
                oss << "0b";
            oss << format_binary(value);
            break;
        case 'B':
            if (show_base)
                oss << "0B";
            oss << format_binary(value);
            break;
        default: oss << value; break;
        }
        return fmt::format_to(ctx.out(), "{}", oss.str());
    }
};
