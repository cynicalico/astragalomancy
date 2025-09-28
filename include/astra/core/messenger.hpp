#pragma once

#include <cstdint>
#include <functional>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

namespace astra {
template<typename T>
concept HasAstraTag = std::same_as<decltype(T::ASTRA_MESSENGER_TAG), const std::uint32_t>;

class Messenger {
    using Payload = std::span<const std::byte>;
    using Receiver = std::function<void(Payload)>;

public:
    using ID = std::size_t;

    ID get_id();
    void release_id(ID id);

    template<typename T, typename Func>
        requires HasAstraTag<T> and std::invocable<Func, const T *>
    void subscribe(ID id, Func &&f);

    template<typename T>
        requires HasAstraTag<T>
    void unsubscribe(ID id);

    template<typename T, typename... Args>
        requires HasAstraTag<T>
    void publish(Args &&...args);

private:
    ID next_id_ = 0;
    std::vector<ID> recycled_ids_{};

    std::unordered_map<std::uint32_t, std::vector<Receiver>> receivers_;

    template<typename T, typename... Args>
    std::span<std::byte> make_payload_(Args &&...args);
};
} // namespace astra

inline astra::Messenger::ID astra::Messenger::get_id() {
    if (recycled_ids_.empty())
        return next_id_++;

    const auto id = recycled_ids_.back();
    recycled_ids_.pop_back();
    return id;
}

inline void astra::Messenger::release_id(const ID id) {
    for (auto &receivers: receivers_ | std::views::values)
        if (receivers.size() > id)
            receivers[id] = nullptr;

    recycled_ids_.push_back(id);
}

template<typename T, typename Func>
    requires astra::HasAstraTag<T> and std::invocable<Func, const T *>
void astra::Messenger::subscribe(ID id, Func &&f) {
    auto &receivers = receivers_[T::ASTRA_MESSENGER_TAG];
    if (receivers.size() <= id)
        receivers.resize(id + 1);
    receivers[id] = [f = std::forward<Func>(f)](const Payload buffer) {
        f(reinterpret_cast<const T *>(buffer.data()));
    };
}

template<typename T>
    requires astra::HasAstraTag<T>
void astra::Messenger::unsubscribe(ID id) {
    auto &receivers = receivers_[T::ASTRA_MESSENGER_TAG];
    if (receivers.size() > id)
        receivers[id] = nullptr;
}

template<typename T, typename... Args>
    requires astra::HasAstraTag<T>
void astra::Messenger::publish(Args &&...args) {
    const auto payload = make_payload_<T>(std::forward<Args>(args)...);
    for (auto &r: receivers_[T::ASTRA_MESSENGER_TAG])
        if (r)
            r(payload);
    operator delete(payload.data(), payload.size());
}

template<typename T, typename... Args>
std::span<std::byte> astra::Messenger::make_payload_(Args &&...args) {
    return std::span(reinterpret_cast<std::byte *>(new T{std::forward<Args>(args)...}), sizeof(T));
}
