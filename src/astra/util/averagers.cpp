#include "astra/util/averagers.hpp"

void astra::CMA::update(double sample) {
    value_ += (sample - value_) / ++sample_count_;
}

double astra::CMA::value() const {
    return value_;
}

astra::SMA::SMA(double sample_count)
    : sample_count(sample_count) {}

void astra::SMA::update(double sample) {
    samples_.push(sample);
    if (samples_.size() <= sample_count) {
        value_ += (sample - value_) / samples_.size();
    } else {
        value_ += (1.0 / sample_count) * (sample - samples_.front());
        samples_.pop();
    }
}

double astra::SMA::value() const {
    return value_;
}

astra::EMA::EMA(double alpha)
    : alpha(alpha) {}

void astra::EMA::update(double sample) {
    value_ = alpha * sample + (1.0 - alpha) * value_;
}

double astra::EMA::value() const {
    return value_;
}
