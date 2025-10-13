#pragma once

#include "astra/util/averagers.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <ctime>
#include <deque>
#include <string>

namespace astra {
template<typename S>
std::string timestamp(const S &format) {
    const auto t = std::time(nullptr);
    const auto lt = std::localtime(&t);
    auto args = fmt::make_format_args(*lt);
    return fmt::vformat(format, args);
}

std::string timestamp();

std::int64_t time_ns();
std::int64_t time_us();
std::int64_t time_ms();
std::int64_t time_s();

class FrameCounter {
public:
    FrameCounter();

    void update();

    double dt() const;
    double fps() const;

    std::vector<double> fps_history() const;

private:
    EMA averager_;
    std::int64_t last_alpha_update_;
    std::deque<std::int64_t> timestamps;
};
} // namespace astra
