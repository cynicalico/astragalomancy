#pragma once

#include <concepts>

namespace astra {
template<typename T>
    requires std::is_floating_point_v<T>
T scale_between(T n, T n_min, T n_max, T t_min, T t_max);
} // namespace astra

template<typename T>
    requires std::is_floating_point_v<T>
T astra::scale_between(T n, T n_min, T n_max, T t_min, T t_max) {
    return (t_max - t_min) * ((n - n_min) / (n_max - n_min)) + t_min;
}
