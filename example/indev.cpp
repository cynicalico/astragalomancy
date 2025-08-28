#include "astra/core/log.hpp"
#include "astra/util/constexpr_hash.hpp"
#include "astra/util/platform.hpp"

int main(int, char *[]) {
    astra::log_platform();

    constexpr auto hash = murmur::x86_32("hellopleasework", 123'321);
    ASTRA_LOG_INFO("{}", hash);

    constexpr auto hash2 = murmur::x86_128("helloworld", 123'321);
    ASTRA_LOG_INFO("{}", hash2);
}
