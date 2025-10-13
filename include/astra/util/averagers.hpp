#pragma once

#include <cstddef>
#include <queue>

namespace astra {
class Averager {
public:
    virtual ~Averager() = default;

    virtual void update(double sample) = 0;

    [[nodiscard]] virtual double value() const = 0;
};

class CMA final : Averager {
public:
    void update(double sample) override;
    [[nodiscard]] double value() const override;

private:
    double value_{0.0};
    std::size_t sample_count_{0};
};

class SMA final : Averager {
public:
    std::size_t sample_count{0};

    explicit SMA(double sample_count);

    void update(double sample) override;
    [[nodiscard]] double value() const override;

private:
    double value_{0.0};
    std::queue<double> samples_{};
};

class EMA final : Averager {
public:
    double alpha{0.0};

    explicit EMA(double alpha);

    void update(double sample) override;
    [[nodiscard]] double value() const override;

private:
    double value_{0.0};
};
} // namespace astra
