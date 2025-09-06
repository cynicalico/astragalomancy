#pragma once

#include <functional>
#include <span>
#include <unordered_map>
#include <vector>

namespace astra {
template<typename T>
concept HasAstraTag = std::same_as<decltype(T::ASTRA_MESSENGER_TAG), const std::uint32_t>;

template<typename T>
const T *extract_payload(const std::span<const std::byte> &buffer);

class Messenger {
    using Receiver = std::function<void(std::span<const std::byte>)>;

public:
    template<typename T>
        requires HasAstraTag<T>
    void subscribe(Receiver &&r);

    template<typename T, typename... Args>
        requires HasAstraTag<T>
    void publish(Args &&...args);

private:
    std::unordered_map<std::uint32_t, std::vector<Receiver>> receivers_;

    template<typename T, typename... Args>
    std::span<std::byte> make_payload_(Args &&...args);
};
} // namespace astra

template<typename T>
const T *astra::extract_payload(const std::span<const std::byte> &buffer) {
    return reinterpret_cast<const T *>(buffer.data());
}

template<typename T>
    requires astra::HasAstraTag<T>
void astra::Messenger::subscribe(Receiver &&r) {
    receivers_[T::ASTRA_MESSENGER_TAG].push_back(std::forward<Receiver>(r));
}

template<typename T, typename... Args>
    requires astra::HasAstraTag<T>
void astra::Messenger::publish(Args &&...args) {
    const auto payload = make_payload_<T>(std::forward<Args>(args)...);
    for (auto &r: receivers_[T::ASTRA_MESSENGER_TAG])
        r(payload);
    operator delete(payload.data(), payload.size());
}

template<typename T, typename... Args>
std::span<std::byte> astra::Messenger::make_payload_(Args &&...args) {
    return std::span(reinterpret_cast<std::byte *>(new T{std::forward<Args>(args)...}), sizeof(T));
}
