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

astra::FrameCounter::FrameCounter()
    : averager_(1.0),
      last_alpha_update_(time_ns()) {}

void astra::FrameCounter::update() {
    const auto now = time_ns();

    timestamps.push_back(now);
    while (timestamps.size() >= 2 && timestamps.back() - timestamps.front() > 1e9) timestamps.pop_front();
    averager_.update(timestamps.size());

    if (now - last_alpha_update_ >= 1e9) {
        averager_.alpha = 2.0 / (averager_.value() + 1.0);
        last_alpha_update_ = now;
    }
}

double astra::FrameCounter::dt() const {
    if (timestamps.size() < 2) return 0.0;
    return (timestamps[timestamps.size() - 1] - timestamps[timestamps.size() - 2]) / 1e9;
}

double astra::FrameCounter::fps() const {
    return averager_.value();
}

std::vector<double> astra::FrameCounter::fps_history() const {
    if (timestamps.size() < 2) return {};

    std::vector<double> history;
    for (std::size_t i = 0; i < timestamps.size() - 2; ++i)
        history.push_back(1.0 / (static_cast<double>(timestamps[i + 1] - timestamps[i]) / 1e9));
    return history;
}
