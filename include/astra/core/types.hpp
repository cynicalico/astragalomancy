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
} // namespace astra
