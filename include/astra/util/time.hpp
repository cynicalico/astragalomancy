#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <ctime>
#include <string>

namespace astra {
template<typename S>
std::string timestamp(const S &format) {
    auto t = std::time(nullptr);
    auto lt = std::localtime(&t);
    auto args = fmt::make_format_args(*lt);

    return fmt::vformat(format, args);
}

std::string timestamp();
} // namespace astra
