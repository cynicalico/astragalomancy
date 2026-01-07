#pragma once

#include "astra/util/is_any_of.hpp"

#include <boost/multiprecision/cpp_int.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <ranges>

namespace astra {
using int128_t = boost::multiprecision::int128_t;
using int256_t = boost::multiprecision::int256_t;
using int512_t = boost::multiprecision::int512_t;
using int1024_t = boost::multiprecision::int1024_t;

using uint128_t = boost::multiprecision::uint128_t;
using uint256_t = boost::multiprecision::uint256_t;
using uint512_t = boost::multiprecision::uint512_t;
using uint1024_t = boost::multiprecision::uint1024_t;

using ap_int = boost::multiprecision::cpp_int;

template<typename T, uint64_t N>
constexpr T constant(const char (&s)[N]) {
    auto i = static_cast<T>(0);
    for (int64_t j = 0; j < N - 1; ++j) i = i * 10 + s[j] - '0';
    return i;
}

template<typename T, uint64_t N>
constexpr T hex_constant(const char (&s)[N]) {
    auto i = static_cast<T>(0);
    for (int64_t j = 0; j < N - 1; ++j) {
        i *= 16;
        switch (s[j]) {
        case 'a': i += 10; break;
        case 'b': i += 11; break;
        case 'c': i += 12; break;
        case 'd': i += 13; break;
        case 'e': i += 14; break;
        case 'f': i += 15; break;
        default: i += s[j] - '0'; break;
        }
    }
    return i;
}
} // namespace astra

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
    char fill_char = ' ';
    align align_option = align::none;
    bool alternative = false;
    sign sign_option = sign::none;
    bool zero_padding = false;
    int width = 0;
    int arg_id = -1;
    bool uppercase = false;
    presentation_type type = presentation_type::dec;

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin();

        if (it != ctx.end() && it + 1 != ctx.end() && (*(it + 1) == '<' || *(it + 1) == '>' || *(it + 1) == '^')) {
            fill_char = *it;
            switch (*(it + 1)) {
            case '<': align_option = align::left; break;
            case '>': align_option = align::right; break;
            case '^': align_option = align::center; break;
            default: std::unreachable();
            }
            it += 2;
        } else if (it != ctx.end() && (*it == '<' || *it == '>' || *it == '^')) {
            switch (*it) {
            case '<': align_option = align::left; break;
            case '>': align_option = align::right; break;
            case '^': align_option = align::center; break;
            default: std::unreachable();
            }
            ++it;
        }

        // Handle sign
        if (it != ctx.end() && (*it == '+' || *it == '-' || *it == ' ')) {
            switch (*it) {
            case '+': sign_option = sign::plus; break;
            case '-': sign_option = sign::minus; break;
            case ' ': sign_option = sign::space; break;
            default: std::unreachable();
            }
            ++it;
        }

        if (it != ctx.end() && *it == '#') {
            alternative = true;
            ++it;
        }

        if (it != ctx.end() && *it == '0') {
            zero_padding = true;
            ++it;
        }

        if (it != ctx.end() && *it == '{' && *(it + 1) == '}') {
            arg_id = ctx.next_arg_id();
            it += 2;
        } else if (it != ctx.end() && std::isdigit(*it)) {
            width = 0;
            while (it != ctx.end() && std::isdigit(*it)) {
                width = width * 10 + (*it - '0');
                ++it;
            }
        }

        if (it != ctx.end()) {
            switch (*it) {
            case 'd': type = presentation_type::dec; break;
            case 'x':
                type = presentation_type::hex;
                uppercase = false;
                break;
            case 'X':
                type = presentation_type::hex;
                uppercase = true;
                break;
            case 'o': type = presentation_type::oct; break;
            case 'b':
                type = presentation_type::bin;
                uppercase = false;
                break;
            case 'B':
                type = presentation_type::bin;
                uppercase = true;
                break;
            default:
                if (*it != '}') throw format_error("invalid format");
                --it; // Step back since we'll increment at the end
                break;
            }
            if (*it != '}') ++it;
        }

        if (it != ctx.end() && *it != '}') throw format_error("invalid format");
        return it;
    }

    auto format(const T &value, format_context &ctx) const {
        int actual_width = width;

        if (arg_id >= 0) {
            ctx.arg(arg_id).visit([&actual_width](auto arg) {
                if constexpr (std::is_integral_v<std::decay_t<decltype(arg)>>) actual_width = static_cast<int>(arg);
                else throw format_error("invalid format");
            });
        }

        std::ostringstream oss;
        switch (type) {
        case presentation_type::hex:
            if (alternative) oss << (uppercase ? "0X" : "0x");
            oss << std::hex << (uppercase ? std::uppercase : std::nouppercase) << value;
            break;
        case presentation_type::oct:
            if (alternative) oss << "0o";
            oss << std::oct << value;
            break;
        case presentation_type::bin:
            if (alternative) oss << (uppercase ? "0B" : "0b");
            oss << format_binary(value);
            break;
        default:
        case presentation_type::dec: oss << value; break;
        }

        std::string result = oss.str();
        result = apply_sign(result, value);
        if (actual_width > 0) result = apply_alignment_with_width(result, actual_width);

        return fmt::format_to(ctx.out(), "{}", result);
    }

private:
    std::string format_binary(const T &value) const {
        if (value == 0) return "0";

        std::string binary;
        T n = abs(value);
        while (n > 0) {
            binary += "01"[static_cast<std::size_t>(n % 2)];
            n /= 2;
        }
        if (value < 0) binary += '-';

        std::ranges::reverse(binary);
        return binary;
    }

    std::string apply_sign(const std::string &str, const T &value) const {
        switch (sign_option) {
        case sign::plus:
            if (value >= 0 && !str.starts_with('+') && !str.starts_with('-')) return '+' + str;
            return str;
        case sign::space:
            if (value >= 0 && !str.starts_with('+') && !str.starts_with('-')) return ' ' + str;
            return str;
        case sign::minus:
        case sign::none: return str;
        default: std::unreachable();
        }
    }

    std::string apply_alignment_with_width(const std::string &str, int actual_width) const {
        if (actual_width <= static_cast<int>(str.length())) return str;

        const int padding = actual_width - static_cast<int>(str.length());

        if (zero_padding && align_option == align::none) {
            size_t insert_pos = 0;

            if (!str.empty() && (str[0] == '+' || str[0] == '-' || str[0] == ' ')) insert_pos = 1;

            if (alternative && str.length() > insert_pos + 1) {
                if (str.substr(insert_pos, 2) == "0x" || str.substr(insert_pos, 2) == "0X" ||
                    str.substr(insert_pos, 2) == "0o" || str.substr(insert_pos, 2) == "0b" ||
                    str.substr(insert_pos, 2) == "0B") {
                    insert_pos += 2;
                }
            }

            return str.substr(0, insert_pos) + std::string(padding, '0') + str.substr(insert_pos);
        }

        switch (align_option) {
        case align::left: return str + std::string(padding, fill_char);
        case align::right: return std::string(padding, fill_char) + str;
        case align::center: {
            const int left_padding = padding / 2;
            const int right_padding = padding - left_padding;
            return std::string(left_padding, fill_char) + str + std::string(right_padding, fill_char);
        }
        default: return std::string(padding, fill_char) + str;
        }
    }
};
