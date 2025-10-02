#include "astra/util/time.hpp"

std::string astra::timestamp() {
    return timestamp("{:%Y-%m-%d_%H-%M-%S}");
}

std::int64_t astra::time_ns() {
    const auto now = std::chrono::steady_clock::now();
    const auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

std::int64_t astra::time_us() {
    const auto now = std::chrono::steady_clock::now();
    const auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

std::int64_t astra::time_ms() {
    const auto now = std::chrono::steady_clock::now();
    const auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

std::int64_t astra::time_s() {
    const auto now = std::chrono::steady_clock::now();
    const auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}
