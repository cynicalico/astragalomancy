#pragma once

#include "astra/core/messenger.hpp"

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace astra {
class TimerMgr {
    struct Timer {
        bool paused{false};

        virtual ~Timer() = default;
        virtual bool update(double dt) = 0;
        virtual bool fire() = 0;
    };

    struct EveryTimer final : Timer {
        std::vector<double> intervals;
        std::optional<std::size_t> count;
        std::function<void()> callback;
        double acc{0.0};

        EveryTimer(std::vector<double> intervals, std::optional<std::size_t> count, std::function<void()> callback);

        bool update(double dt) override;
        bool fire() override;
    };

    struct UntilTimer final : Timer {
        std::vector<double> intervals;
        std::optional<std::size_t> count;
        std::function<bool()> callback;
        double acc{0.0};

        UntilTimer(std::vector<double> intervals, std::optional<std::size_t> count, std::function<bool()> callback);

        bool update(double dt) override;
        bool fire() override;
    };

    struct AfterTimer final : Timer {
        std::function<void()> callback;
        double acc{0.0};

        AfterTimer(double interval, std::function<void()> callback);

        bool update(double dt) override;
        bool fire() override;
    };

    struct DuringTimer final : Timer {
        std::function<void()> callback;
        std::function<void()> after_callback;
        double acc{0.0};

        DuringTimer(double duration, std::function<void()> callback, std::function<void()> after_callback);

        bool update(double dt) override;
        bool fire() override;
    };

public:
    explicit TimerMgr(Messenger *messenger);
    ~TimerMgr();

    TimerMgr(const TimerMgr &other) = delete;
    TimerMgr &operator=(const TimerMgr &other) = delete;

    TimerMgr(TimerMgr &&other) noexcept;
    TimerMgr &operator=(TimerMgr &&other) noexcept;

    void cancel(const std::string &id);

    void clear();

    template<typename T>
    std::string every(const std::vector<double> &intervals, std::size_t count, T &&callback);

    template<typename T>
    std::string every(const std::vector<double> &intervals, T &&callback);

    template<typename T>
    std::string every(double interval, std::size_t count, T &&callback);

    template<typename T>
    std::string every(double interval, T &&callback);

    template<typename T>
    std::string until(const std::vector<double> &intervals, std::size_t count, T &&callback);

    template<typename T>
    std::string until(const std::vector<double> &intervals, T &&callback);

    template<typename T>
    std::string until(double interval, std::size_t count, T &&callback);

    template<typename T>
    std::string until(double interval, T &&callback);

    template<typename T>
    std::string after(double interval, T &&callback);

    template<typename T>
    std::string during(double duration, T &&callback);

    template<typename T, typename U>
    std::string during(double duration, T &&callback, U &&after_callback);

private:
    std::unordered_map<std::string, Timer *> timers_;

    std::string
    every_(const std::vector<double> &intervals, std::optional<std::size_t> count, std::function<void()> callback);

    std::string
    until_(const std::vector<double> &intervals, std::optional<std::size_t> count, std::function<bool()> callback);

    std::string after_(double interval, std::function<void()> callback);

    std::string during_(double duration, std::function<void()> callback, std::function<void()> after_callback);

    void update_(double dt);

    Messenger *messenger;
    std::optional<Messenger::ID> callback_id_;
    void register_callbacks_();
    void unregister_callbacks_();
};
} // namespace astra

template<typename T>
std::string astra::TimerMgr::every(const std::vector<double> &intervals, std::size_t count, T &&callback) {
    return every_(intervals, count, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::every(const std::vector<double> &intervals, T &&callback) {
    return every_(intervals, std::nullopt, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::every(double interval, std::size_t count, T &&callback) {
    return every_(std::vector{interval}, count, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::every(double interval, T &&callback) {
    return every_(std::vector{interval}, std::nullopt, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::until(const std::vector<double> &intervals, std::size_t count, T &&callback) {
    return until_(intervals, count, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::until(const std::vector<double> &intervals, T &&callback) {
    return until_(intervals, std::nullopt, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::until(double interval, std::size_t count, T &&callback) {
    return until_(std::vector{interval}, count, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::until(double interval, T &&callback) {
    return until_(std::vector{interval}, std::nullopt, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::after(double interval, T &&callback) {
    return after_(interval, std::forward<T>(callback));
}

template<typename T>
std::string astra::TimerMgr::during(double duration, T &&callback) {
    return during_(duration, std::forward<T>(callback), nullptr);
}

template<typename T, typename U>
std::string astra::TimerMgr::during(double duration, T &&callback, U &&after_callback) {
    return during_(duration, std::forward<T>(callback), std::forward<U>(after_callback));
}
