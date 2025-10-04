#include "astra/core/color.hpp"

#include <algorithm>

astra::RGB::RGB()
    : RGB(0, 0, 0, 0) {}

glm::vec4 astra::RGB::gl_color() const {
    return {static_cast<float>(r) / 255.0f,
            static_cast<float>(g) / 255.0f,
            static_cast<float>(b) / 255.0f,
            static_cast<float>(a) / 255.0f};
}

astra::RGB::RGB(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
    : r(r),
      g(g),
      b(b),
      a(a) {}

astra::RGB::RGB(const HSL &hsl)
    : a(hsl.a) {
    const auto c = (1.0f - std::abs(2.0f * hsl.l - 1.0f)) * hsl.s;
    const auto hp = hsl.h / 60.0f;
    const auto x = c * (1.0f - std::abs(std::fmodf(hp, 2.0f) - 1.0f));
    const auto m = hsl.l - c / 2.0f;

    float r1, g1, b1;
    if (hp < 1.0f) {
        r1 = c;
        g1 = x;
        b1 = 0.0f;
    } else if (hp < 2.0f) {
        r1 = x;
        g1 = c;
        b1 = 0.0f;
    } else if (hp < 3.0f) {
        r1 = 0.0f;
        g1 = c;
        b1 = x;
    } else if (hp < 4.0f) {
        r1 = 0.0f;
        g1 = x;
        b1 = c;
    } else if (hp < 5.0f) {
        r1 = x;
        g1 = 0.0f;
        b1 = c;
    } else {
        r1 = c;
        g1 = 0.0;
        b1 = x;
    }

    r = static_cast<std::uint8_t>(std::round(255.0f * (r1 + m)));
    g = static_cast<std::uint8_t>(std::round(255.0f * (g1 + m)));
    b = static_cast<std::uint8_t>(std::round(255.0f * (b1 + m)));
}

astra::RGB::RGB(const HSV &hsv)
    : a(hsv.a) {
    const auto c = hsv.v * hsv.s;
    const auto hp = hsv.h / 60.0f;
    const auto x = c * (1.0f - std::abs(std::fmodf(hp, 2.0f) - 1.0f));
    const auto m = hsv.v - c;

    float r1, g1, b1;
    if (hp < 1.0f) {
        r1 = c;
        g1 = x;
        b1 = 0.0f;
    } else if (hp < 2.0f) {
        r1 = x;
        g1 = c;
        b1 = 0.0f;
    } else if (hp < 3.0f) {
        r1 = 0.0f;
        g1 = c;
        b1 = x;
    } else if (hp < 4.0f) {
        r1 = 0.0f;
        g1 = x;
        b1 = c;
    } else if (hp < 5.0f) {
        r1 = x;
        g1 = 0.0f;
        b1 = c;
    } else {
        r1 = c;
        g1 = 0.0;
        b1 = x;
    }

    r = static_cast<std::uint8_t>(std::round(255.0f * (r1 + m)));
    g = static_cast<std::uint8_t>(std::round(255.0f * (g1 + m)));
    b = static_cast<std::uint8_t>(std::round(255.0f * (b1 + m)));
}

astra::RGB astra::rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    return {r, g, b, a};
}

astra::RGB astra::rgba(const std::uint64_t hex) {
    return {static_cast<std::uint8_t>(hex >> 24 & 0xff),
            static_cast<std::uint8_t>(hex >> 16 & 0xff),
            static_cast<std::uint8_t>(hex >> 8 & 0xff),
            static_cast<std::uint8_t>(hex & 0xff)};
}

astra::RGB astra::rgba_f(const float r, const float g, const float b, const float a) {
    return {static_cast<std::uint8_t>(std::round(255.0f * r)),
            static_cast<std::uint8_t>(std::round(255.0f * g)),
            static_cast<std::uint8_t>(std::round(255.0f * b)),
            static_cast<std::uint8_t>(std::round(255.0f * a))};
}

astra::RGB astra::rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    return {r, g, b, 255};
}

astra::RGB astra::rgb(const std::uint32_t hex) {
    return {static_cast<std::uint8_t>(hex >> 16 & 0xff),
            static_cast<std::uint8_t>(hex >> 8 & 0xff),
            static_cast<std::uint8_t>(hex & 0xff),
            255};
}

astra::RGB astra::rgb_f(const float r, const float g, const float b) {
    return {static_cast<std::uint8_t>(std::round(255.0f * r)),
            static_cast<std::uint8_t>(std::round(255.0f * g)),
            static_cast<std::uint8_t>(std::round(255.0f * b)),
            255};
}

astra::HSL::HSL()
    : HSL(0, 0, 0, 0) {}

astra::HSL::HSL(const RGB &rgb)
    : a(rgb.a) {
    const auto rgb_f = rgb.gl_color();

    const auto x_max = std::max(rgb_f.r, std::max(rgb_f.g, rgb_f.b));
    const auto x_min = std::min(rgb_f.r, std::min(rgb_f.g, rgb_f.b));
    const auto delta = x_max - x_min;

    if (delta == 0.0f)
        h = 0.0f;
    else if (x_max == rgb_f.r)
        h = 60.0f * std::fmodf((rgb_f.g - rgb_f.b) / delta, 6.0f);
    else if (x_max == rgb_f.g)
        h = 60.0f * ((rgb_f.b - rgb_f.r) / delta + 2.0f);
    else
        h = 60.0f * ((rgb_f.r - rgb_f.g) / delta + 4.0f);

    l = (x_max + x_min) / 2.0f;

    s = delta == 0.0f ? 0.0f : delta / (1.0f - std::abs(2.0f * l - 1.0f));
}

astra::HSL::HSL(const HSV &hsv)
    : h(hsv.h),
      a(hsv.a) {
    l = hsv.v * (1.0f - hsv.s / 2.0f);
    s = (l == 0.0 || l == 1.0f) ? 0.0f : (hsv.v - l) / std::min(l, 1.0f - l);
}

void astra::HSL::rotate(float offset) {
    h = std::fmodf(h + offset, 360.0f);
}

glm::vec4 astra::HSL::gl_color() const {
    return RGB(*this).gl_color();
}

astra::HSL::HSL(const float h, const float s, const float l, const std::uint8_t a)
    : h(h),
      s(s),
      l(l),
      a(a) {}

astra::HSL astra::hsla(float h, float s, float l, std::uint8_t a) {
    return {std::fmodf(h, 360.0f), std::clamp(s, 0.0f, 1.0f), std::clamp(l, 0.0f, 1.0f), a};
}

astra::HSL astra::hsla_f(float h, float s, float l, float a) {
    return {std::fmodf(h, 360.0f),
            std::clamp(s, 0.0f, 1.0f),
            std::clamp(l, 0.0f, 1.0f),
            static_cast<std::uint8_t>(255.0f * a)};
}

astra::HSL astra::hsl(float h, float s, float l) {
    return {std::fmodf(h, 360.0f), std::clamp(s, 0.0f, 1.0f), std::clamp(l, 0.0f, 1.0f), 255};
}

astra::HSV::HSV()
    : HSV(0, 0, 0, 0) {}

astra::HSV::HSV(const RGB &rgb)
    : a(rgb.a) {
    const auto rgb_f = rgb.gl_color();

    const auto x_max = std::max(rgb_f.r, std::max(rgb_f.g, rgb_f.b));
    const auto x_min = std::min(rgb_f.r, std::min(rgb_f.g, rgb_f.b));
    const auto delta = x_max - x_min;

    if (delta == 0.0f)
        h = 0.0f;
    else if (x_max == rgb_f.r)
        h = 60.0f * std::fmodf((rgb_f.g - rgb_f.b) / delta, 6.0f);
    else if (x_max == rgb_f.g)
        h = 60.0f * ((rgb_f.b - rgb_f.r) / delta + 2.0f);
    else
        h = 60.0f * ((rgb_f.r - rgb_f.g) / delta + 4.0f);

    s = x_max == 0.0f ? 0.0f : delta / x_max;

    v = x_max;
}

astra::HSV::HSV(const HSL &hsl)
    : h(hsl.h),
      a(hsl.a) {
    v = hsl.l + hsl.s * std::min(hsl.l, 1.0f - hsl.l);
    s = v == 0.0 ? 0.0f : 2.0f * (1.0f - hsl.l / v);
}

void astra::HSV::rotate(float offset) {
    h = std::fmodf(h + offset, 360.0f);
}

glm::vec4 astra::HSV::gl_color() const {
    return RGB(*this).gl_color();
}

astra::HSV::HSV(float h, float s, float v, std::uint8_t a)
    : h(h),
      s(s),
      v(v),
      a(a) {}

astra::HSV astra::hsva(float h, float s, float v, std::uint8_t a) {
    return {std::fmodf(h, 360.0f), std::clamp(s, 0.0f, 1.0f), std::clamp(v, 0.0f, 1.0f), a};
}

astra::HSV astra::hsva_f(float h, float s, float v, float a) {
    return {std::fmodf(h, 360.0f),
            std::clamp(s, 0.0f, 1.0f),
            std::clamp(v, 0.0f, 1.0f),
            static_cast<std::uint8_t>(255.0f * a)};
}

astra::HSV astra::hsv(float h, float s, float v) {
    return {std::fmodf(h, 360.0f), std::clamp(s, 0.0f, 1.0f), std::clamp(v, 0.0f, 1.0f), 255};
}
