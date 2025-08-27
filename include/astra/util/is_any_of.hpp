#pragma once

#include <concepts>

namespace astra {
template<typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);
} // namespace astra
