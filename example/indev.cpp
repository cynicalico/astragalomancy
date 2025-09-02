#include "astra/core/log.hpp"
#include "astra/util/constexpr_hash.hpp"
#include "astra/util/platform.hpp"

int main(int, char *[]) {
    astra::log_platform();

    fmt::println("{}", astra::murmur_x86_32("helloworld", 0));
    fmt::println("2687965642");

    fmt::println("{}", astra::murmur_x86_128("helloworld", 0));
    fmt::println("234323995771881117847335316008709271584");

    fmt::println("{}", astra::murmur_x64_128("helloworld", 0));
    fmt::println("189201474847812420111393511484868631927");
}
