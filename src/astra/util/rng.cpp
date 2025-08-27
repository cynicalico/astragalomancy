#include "astra/util/rng.hpp"

#include "astra/core/log.hpp"

void gen_seed_vals() {
    pcg_extras::pcg128_t seed_vals[4];
    auto seed_seq = pcg_extras::seed_seq_from<std::random_device>();
    pcg_extras::generate_to<2>(seed_seq, seed_vals);

    astra::rng::seed_info().seed = seed_vals[0];
    astra::rng::seed_info().stream = seed_vals[1];
}

pcg64 &astra::rng::generator() {
    thread_local pcg64 g = std::invoke([] {
        gen_seed_vals();
        return pcg64(seed_info().seed, seed_info().stream);
    });

    return g;
}

astra::rng::seed_data &astra::rng::seed_info() {
    thread_local seed_data s{};
    return s;
}

void astra::rng::reseed() {
    generator(); // Make sure the generator has been invoked before
    gen_seed_vals();
    generator().seed(seed_info().seed, seed_info().stream);
}

void astra::rng::seed128(
        const std::uint64_t seed_hi,
        const std::uint64_t seed_lo,
        const std::uint64_t stream_hi,
        const std::uint64_t stream_lo) {
    pcg_extras::pcg128_t seed = PCG_128BIT_CONSTANT(seed_hi, seed_lo);
    pcg_extras::pcg128_t stream = PCG_128BIT_CONSTANT(stream_hi, stream_lo);
    generator().seed(seed, stream);
    seed_info().seed = seed;
    seed_info().stream = stream;
}

void astra::rng::seed(std::uint64_t seed, std::uint64_t stream) {
    generator().seed(seed, stream);
    seed_info().seed = seed;
    seed_info().stream = stream;
}

void astra::rng::debug_show_seed() {
    generator(); // Make sure the generator has been invoked before

    if (static_cast<std::uint64_t>(seed_info().seed >> 64) == 0 &&
        static_cast<std::uint64_t>(seed_info().stream >> 64) == 0) {
        ASTRA_LOG_DEBUG(
                "Seed statement: rnd::seed({:#x}, {:#x});",
                static_cast<std::uint64_t>(seed_info().seed),
                static_cast<std::uint64_t>(seed_info().stream));

    } else {
        auto seed_hi = static_cast<std::uint64_t>(seed_info().seed >> 64);
        auto seed_lo = static_cast<std::uint64_t>(seed_info().seed);
        auto stream_hi = static_cast<std::uint64_t>(seed_info().stream >> 64);
        auto stream_lo = static_cast<std::uint64_t>(seed_info().stream);
        ASTRA_LOG_DEBUG(
                "Seed statement: rnd::seed128({:#x}, {:#x}, {:#x}, {:#x});", seed_hi, seed_lo, stream_hi, stream_lo);
    }
}

std::string astra::rng::base58(const std::size_t length) {
    static constexpr char B58_ALPHABET[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    std::string id(length, 0);
    std::generate_n(id.begin(), length, [] {
        return B58_ALPHABET[get<unsigned char>(0x00, 0xff) % 58];
    });

    return id;
}
