#include "astra/util/module/timer_mgr.hpp"

#include "astra/core/payloads.hpp"

astra::TimerMgr::EveryTimer::EveryTimer(
        std::vector<double> intervals, const std::optional<std::size_t> count, std::function<void()> callback)
    : intervals(std::move(intervals)),
      count(count),
      callback(std::move(callback)) {}

bool astra::TimerMgr::EveryTimer::update(double dt) {
    acc -= dt;
    if (acc <= 0.0) {
        acc += intervals[rng::get<std::size_t>(intervals.size() - 1)];
        return true;
    }
    return false;
}

bool astra::TimerMgr::EveryTimer::fire() {
    callback();
    if (count) {
        count = count.transform([](auto c) { return c > 0 ? c - 1 : c; });
        if (*count == 0)
            return false;
    }
    return true;
}

astra::TimerMgr::UntilTimer::UntilTimer(
        std::vector<double> intervals, const std::optional<std::size_t> count, std::function<bool()> callback)
    : intervals(std::move(intervals)),
      count(count),
      callback(std::move(callback)) {}

bool astra::TimerMgr::UntilTimer::update(double dt) {
    acc -= dt;
    if (acc <= 0.0) {
        acc += intervals[rng::get<std::size_t>(intervals.size() - 1)];
        return true;
    }
    return false;
}

bool astra::TimerMgr::UntilTimer::fire() {
    bool should_continue = callback();
    if (count) {
        count = count.transform([](auto c) { return c > 0 ? c - 1 : c; });
        if (*count == 0)
            return false;
    }
    return should_continue;
}

astra::TimerMgr::AfterTimer::AfterTimer(double interval, std::function<void()> callback)
    : callback(std::move(callback)),
      acc(interval) {}

bool astra::TimerMgr::AfterTimer::update(double dt) {
    acc -= dt;
    return acc <= 0.0;
}

bool astra::TimerMgr::AfterTimer::fire() {
    callback();
    return false;
}

astra::TimerMgr::DuringTimer::DuringTimer(
        double duration, std::function<void()> callback, std::function<void()> after_callback)
    : callback(std::move(callback)),
      after_callback(std::move(after_callback)),
      acc(duration) {}

bool astra::TimerMgr::DuringTimer::update(double dt) {
    acc -= dt;
    return true;
}

bool astra::TimerMgr::DuringTimer::fire() {
    callback();
    if (acc <= 0.0) {
        if (after_callback)
            after_callback();
        return false;
    }
    return true;
}

astra::TimerMgr::TimerMgr(Messenger *messenger)
    : messenger(messenger) {
    register_callbacks_();
}

astra::TimerMgr::~TimerMgr() {
    if (callback_id_)
        unregister_callbacks_();
}

astra::TimerMgr::TimerMgr(TimerMgr &&other) noexcept
    : messenger(other.messenger) {
    other.unregister_callbacks_();
    other.messenger = nullptr;
    register_callbacks_();
}

astra::TimerMgr &astra::TimerMgr::operator=(TimerMgr &&other) noexcept {
    if (this != &other) {
        unregister_callbacks_();
        other.unregister_callbacks_();
        messenger = other.messenger;
        other.messenger = nullptr;
        register_callbacks_();
    }
    return *this;
}

void astra::TimerMgr::cancel(const std::string &id) {
    if (auto it = timers_.find(id); it != timers_.end())
        timers_.erase(it);
}

void astra::TimerMgr::clear() {
    timers_.clear();
}

std::string astra::TimerMgr::every_(
        const std::vector<double> &intervals, std::optional<std::size_t> count, std::function<void()> callback) {
    const auto id = rng::base58(11);
    timers_.emplace(id, new EveryTimer(std::move(intervals), count, std::move(callback)));
    return id;
}

std::string astra::TimerMgr::until_(
        const std::vector<double> &intervals, std::optional<std::size_t> count, std::function<bool()> callback) {
    const auto id = rng::base58(11);
    timers_.emplace(id, new UntilTimer(std::move(intervals), count, std::move(callback)));
    return id;
}
std::string astra::TimerMgr::after_(double interval, std::function<void()> callback) {
    const auto id = rng::base58(11);
    timers_.emplace(id, new AfterTimer(interval, std::move(callback)));
    return id;
}

std::string
astra::TimerMgr::during_(double duration, std::function<void()> callback, std::function<void()> after_callback) {
    const auto id = rng::base58(11);
    timers_.emplace(id, new DuringTimer(duration, std::move(callback), std::move(after_callback)));
    return id;
}

void astra::TimerMgr::update_(double dt) {
    for (auto it = timers_.begin(); it != timers_.end();) {
        if (it->second->paused) {
            ++it;
            continue;
        }

        if (it->second->update(dt)) {
            if (!it->second->fire()) {
                it = timers_.erase(it);
                continue;
            }
        }

        ++it;
    }
}

void astra::TimerMgr::register_callbacks_() {
    callback_id_ = messenger->get_id();

    messenger->subscribe<PreUpdate>(*callback_id_, [&](const auto *p) { update_(p->dt); });
}

void astra::TimerMgr::unregister_callbacks_() {
    messenger->release_id(*callback_id_);

    callback_id_ = std::nullopt;
}
