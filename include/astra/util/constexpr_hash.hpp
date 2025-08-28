#pragma once

#include <cstdint>

#if defined(ASTRA_ENABLE_UINT128)
#include <boost/multiprecision/cpp_int.hpp>
#endif

namespace murmur {
namespace internal {
constexpr std::uint32_t rotl32(const std::uint32_t x, const std::int8_t r) {
    return x << r | x >> (32 - r);
}

constexpr std::uint32_t getblock32(const char *p, const std::uint32_t i) {
    return static_cast<uint32_t>(p[0 + i * 4]) << 0 | static_cast<uint32_t>(p[1 + i * 4]) << 8 |
           static_cast<uint32_t>(p[2 + i * 4]) << 16 | static_cast<uint32_t>(p[3 + i * 4]) << 24;
}

constexpr std::uint32_t fmix32(std::uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}
} // namespace internal

constexpr std::uint32_t x86_32(const char *key, const std::uint32_t len, const std::uint32_t seed) {
    using namespace internal;

    const std::uint32_t nblocks = len / 4;

    std::uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    // body

    for (std::uint32_t i = 0; i < nblocks; i++) {
        std::uint32_t k1 = getblock32(key, i);

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    // tail

    std::uint32_t k1 = 0;

    const std::uint32_t tail_start = len - len % 4;
    switch (len & 3) {
    case 3: k1 ^= key[tail_start + 2] << 16;
    case 2: k1 ^= key[tail_start + 1] << 8;
    case 1:
        k1 ^= key[tail_start + 0];
        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    }

    // finalization

    h1 ^= len;
    h1 = fmix32(h1);

    return h1;
}

template<uint64_t N>
constexpr std::uint32_t x86_32(const char (&s)[N], const std::uint32_t seed) {
    return x86_32(s, N - 1, seed);
}

#if defined(ASTRA_ENABLE_UINT128)
constexpr boost::multiprecision::uint128_t x86_128(const char *key, const std::uint32_t len, const std::uint32_t seed) {
    using namespace internal;
    using namespace boost::multiprecision;

    const std::uint32_t nblocks = len / 16;

    std::uint32_t h1 = seed;
    std::uint32_t h2 = seed;
    std::uint32_t h3 = seed;
    std::uint32_t h4 = seed;

    const std::uint32_t c1 = 0x239b961b;
    const std::uint32_t c2 = 0xab0e9789;
    const std::uint32_t c3 = 0x38b34ae5;
    const std::uint32_t c4 = 0xa1e38b93;

    // body

    for (std::uint32_t i = 0; i < nblocks; i++) {
        std::uint32_t k1 = getblock32(key, i * 4 + 0);
        std::uint32_t k2 = getblock32(key, i * 4 + 1);
        std::uint32_t k3 = getblock32(key, i * 4 + 2);
        std::uint32_t k4 = getblock32(key, i * 4 + 3);

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;

        h1 = rotl32(h1, 19);
        h1 += h2;
        h1 = h1 * 5 + 0x561ccd1b;

        k2 *= c2;
        k2 = rotl32(k2, 16);
        k2 *= c3;
        h2 ^= k2;

        h2 = rotl32(h2, 17);
        h2 += h3;
        h2 = h2 * 5 + 0x0bcaa747;

        k3 *= c3;
        k3 = rotl32(k3, 17);
        k3 *= c4;
        h3 ^= k3;

        h3 = rotl32(h3, 15);
        h3 += h4;
        h3 = h3 * 5 + 0x96cd1c35;

        k4 *= c4;
        k4 = rotl32(k4, 18);
        k4 *= c1;
        h4 ^= k4;

        h4 = rotl32(h4, 13);
        h4 += h1;
        h4 = h4 * 5 + 0x32ac3b17;
    }

    std::uint32_t k1 = 0;
    std::uint32_t k2 = 0;
    std::uint32_t k3 = 0;
    std::uint32_t k4 = 0;

    const std::uint32_t tail_start = len - len % 16;
    switch (len & 15) {
    case 15: k4 ^= key[tail_start + 14] << 16;
    case 14: k4 ^= key[tail_start + 13] << 8;
    case 13:
        k4 ^= key[tail_start + 12] << 0;
        k4 *= c4;
        k4 = rotl32(k4, 18);
        k4 *= c1;
        h4 ^= k4;
    case 12: k3 ^= key[tail_start + 11] << 24;
    case 11: k3 ^= key[tail_start + 10] << 16;
    case 10: k3 ^= key[tail_start + 9] << 8;
    case 9:
        k3 ^= key[tail_start + 8] << 0;
        k3 *= c3;
        k3 = rotl32(k3, 17);
        k3 *= c4;
        h3 ^= k3;
    case 8: k2 ^= key[tail_start + 7] << 24;
    case 7: k2 ^= key[tail_start + 6] << 16;
    case 6: k2 ^= key[tail_start + 5] << 8;
    case 5:
        k2 ^= key[tail_start + 4] << 0;
        k2 *= c2;
        k2 = rotl32(k2, 16);
        k2 *= c3;
        h2 ^= k2;
    case 4: k1 ^= key[tail_start + 3] << 24;
    case 3: k1 ^= key[tail_start + 2] << 16;
    case 2: k1 ^= key[tail_start + 1] << 8;
    case 1:
        k1 ^= key[tail_start + 0] << 0;
        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    }

    // finalization

    h1 ^= len;
    h2 ^= len;
    h3 ^= len;
    h4 ^= len;

    h1 += h2;
    h1 += h3;
    h1 += h4;
    h2 += h1;
    h3 += h1;
    h4 += h1;

    h1 = fmix32(h1);
    h2 = fmix32(h2);
    h3 = fmix32(h3);
    h4 = fmix32(h4);

    h1 += h2;
    h1 += h3;
    h1 += h4;
    h2 += h1;
    h3 += h1;
    h4 += h1;

    return (uint128_t)h1 << 96 | (uint128_t)h2 << 64 | (uint128_t)h3 << 32 | h4;
}

template<uint64_t N>
constexpr boost::multiprecision::uint128_t x86_128(const char (&s)[N], const std::uint32_t seed) {
    return x86_128(s, N - 1, seed);
}

constexpr boost::multiprecision::uint128_t x64_128(const char *key, const std::uint32_t len, const std::uint32_t seed) {
    using namespace internal;
    using namespace boost::multiprecision;

    // TODO
    return 0;
}

template<uint64_t N>
constexpr boost::multiprecision::uint128_t x64_128(const char (&s)[N], const std::uint32_t seed) {
    return x64_128(s, N - 1, seed);
}
#endif
} // namespace murmur
