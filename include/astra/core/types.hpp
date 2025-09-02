#pragma once

#include <boost/multiprecision/cpp_int.hpp>

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
    for (int64_t j = 0; j < N - 1; ++j)
        i = i * 10 + s[j] - '0';
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
