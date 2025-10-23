#pragma once

#include "astra/util/rng.hpp"

#include <fmt/format.h>
#include <glm/vec4.hpp>
#include <imgui.h>

// fuck windows
#if defined(RGB)
#undef RGB
#endif

namespace astra {
class Color {
public:
    virtual ~Color() = default;

    [[nodiscard]] virtual glm::vec4 gl_color() const = 0;
    [[nodiscard]] virtual glm::vec4 gl_color(std::uint8_t override_alpha);

    [[nodiscard]] ImVec4 imgui_color_vec4() const;
    [[nodiscard]] ImVec4 imgui_color_vec4(std::uint8_t override_alpha) const;

    [[nodiscard]] ImU32 imgui_color_u32() const;
    [[nodiscard]] ImU32 imgui_color_u32(std::uint8_t override_alpha) const;
};

class RGB;
class HSL;
class HSV;

class RGB final : public Color {
public:
    std::uint8_t r, g, b, a;

    RGB();
    explicit RGB(const HSL &hsl);
    explicit RGB(const HSV &hsv);

    friend RGB rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
    friend RGB rgba(std::uint64_t hex);
    friend RGB rgba_f(float r, float g, float b, float a);

    friend RGB rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
    friend RGB rgb(std::uint32_t hex);
    friend RGB rgb_f(float r, float g, float b);

    [[nodiscard]] glm::vec4 gl_color() const override;

private:
    RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
};

RGB rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
RGB rgba(std::uint64_t hex);
RGB rgba_f(float r, float g, float b, float a);

RGB rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
RGB rgb(std::uint32_t hex);
RGB rgb_f(float r, float g, float b);

class HSL final : public Color {
public:
    float h, s, l;
    std::uint8_t a;

    HSL();
    explicit HSL(const RGB &rgb);
    explicit HSL(const HSV &hsv);

    friend HSL hsla(float h, float s, float l, std::uint8_t a);
    friend HSL hsla_f(float h, float s, float l, float a);

    friend HSL hsl(float h, float s, float l);

    void rotate(float offset);

    [[nodiscard]] glm::vec4 gl_color() const override;

private:
    HSL(float h, float s, float l, std::uint8_t a);
};

HSL hsla(float h, float s, float l, std::uint8_t a);
HSL hsla_f(float h, float s, float l, float a);

HSL hsl(float h, float s, float l);

class HSV final : public Color {
public:
    float h, s, v;
    std::uint8_t a;

    HSV();
    explicit HSV(const RGB &rgb);
    explicit HSV(const HSL &hsl);

    friend HSV hsva(float h, float s, float v, std::uint8_t a);
    friend HSV hsva_f(float h, float s, float v, float a);

    friend HSV hsv(float h, float s, float v);

    void rotate(float offset);

    [[nodiscard]] glm::vec4 gl_color() const override;

private:
    HSV(float h, float s, float v, std::uint8_t a);
};

HSV hsva(float h, float s, float v, std::uint8_t a);
HSV hsva_f(float h, float s, float v, float a);

HSV hsv(float h, float s, float v);

namespace rng {
RGB rgb();
RGB rgba();

RGB rgb(glm::tvec2<std::uint8_t> r_range, glm::tvec2<std::uint8_t> g_range, glm::tvec2<std::uint8_t> b_range);
RGB rgba(
        glm::tvec2<std::uint8_t> r_range,
        glm::tvec2<std::uint8_t> g_range,
        glm::tvec2<std::uint8_t> b_range,
        glm::tvec2<std::uint8_t> a_range);
} // namespace rng
} // namespace astra

template<>
struct fmt::formatter<astra::RGB> {
    bool zero_padding = false;
    int width = 0;

    constexpr auto parse(const format_parse_context &ctx) {
        auto it = ctx.begin();

        if (it != ctx.end() && *it == '0') {
            zero_padding = true;
            ++it;
        }

        if (it != ctx.end() && *it >= '0' && *it <= '9') {
            width = 0;
            while (it != ctx.end() && *it >= '0' && *it <= '9') {
                width = width * 10 + (*it - '0');
                ++it;
            }
        }

        if (it != ctx.end() && *it != '}')
            throw format_error("invalid format specifier");

        return it;
    }

    auto format(const astra::RGB &rgb, const format_context &ctx) const {
        if (zero_padding && width > 0) {
            return format_to(
                    ctx.out(),
                    "RGB({:0{}}, {:0{}}, {:0{}}, {:0{}})",
                    rgb.r,
                    width,
                    rgb.g,
                    width,
                    rgb.b,
                    width,
                    rgb.a,
                    width);
        }

        return format_to(
                ctx.out(), "RGB({:{}}, {:{}}, {:{}}, {:{}})", rgb.r, width, rgb.g, width, rgb.b, width, rgb.a, width);
    }
};

template<>
struct fmt::formatter<astra::HSL> {
    int precision = 2;

    constexpr auto parse(const format_parse_context &ctx) {
        auto it = ctx.begin();

        if (it != ctx.end() && *it == '.') {
            ++it;
            precision = 0;
            while (it != ctx.end() && *it >= '0' && *it <= '9') {
                precision = precision * 10 + (*it - '0');
                ++it;
            }
        }

        if (it != ctx.end() && *it != '}')
            throw format_error("invalid format specifier");

        return it;
    }

    auto format(const astra::HSL &hsl, const format_context &ctx) const {
        return format_to(
                ctx.out(),
                "HSL({:.{}f}, {:.{}f}, {:.{}f}, {})",
                hsl.h,
                precision,
                hsl.s,
                precision,
                hsl.l,
                precision,
                hsl.a);
    }
};

template<>
struct fmt::formatter<astra::HSV> {
    int precision = 2;

    constexpr auto parse(const format_parse_context &ctx) {
        auto it = ctx.begin();

        if (it != ctx.end() && *it == '.') {
            ++it;
            precision = 0;
            while (it != ctx.end() && *it >= '0' && *it <= '9') {
                precision = precision * 10 + (*it - '0');
                ++it;
            }
        }

        if (it != ctx.end() && *it != '}')
            throw format_error("invalid format specifier");

        return it;
    }

    auto format(const astra::HSV &hsv, const format_context &ctx) const {
        return format_to(
                ctx.out(),
                "HSV({:.{}f}, {:.{}f}, {:.{}f}, {})",
                hsv.h,
                precision,
                hsv.s,
                precision,
                hsv.v,
                precision,
                hsv.a);
    }
};
