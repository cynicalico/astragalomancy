#include "astra/core/log.hpp"
#include "astra/util/constexpr_hash.hpp"
#include "astra/util/platform.hpp"
#include "astra/util/rng.hpp"

int main(int, char *[]) {
    astra::log_platform();
    astra::rng::debug_show_seed();

    static_assert(astra::murmur_x86_32("helloworld", 0) == 0xa03719ca);
    static_assert(
            astra::murmur_x86_128("helloworld", 0) ==
            astra::hex_constant<astra::uint128_t>("b04928f6b6b9b0a3df7f9ce23e9a3020"));
    static_assert(
            astra::murmur_x64_128("helloworld", 0) ==
            astra::hex_constant<astra::uint128_t>("8e56e0eb7b45bc7aa2b611c5e57d9d77"));
}
