#pragma once

#include "gloo/gl.hpp"

#include <fmt/format.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace gloo {
class Shader {
    friend class ShaderBuilder;

    explicit Shader(GLuint id);

public:
    GLuint id;

    ~Shader();

    void use() const;

    std::optional<GLuint> try_get_attrib_location(const std::string &name);

    void uniform_1f(const std::string &name, float v0);
    void uniform_2f(const std::string &name, float v0, float v1);
    void uniform_3f(const std::string &name, float v0, float v1, float v2);
    void uniform_4f(const std::string &name, float v0, float v1, float v2, float v3);

    void uniform_1i(const std::string &name, int v0);
    void uniform_2i(const std::string &name, int v0, int v1);
    void uniform_3i(const std::string &name, int v0, int v1, int v2);
    void uniform_4i(const std::string &name, int v0, int v1, int v2, int v3);

    void uniform_1u(const std::string &name, unsigned int v0);
    void uniform_2u(const std::string &name, unsigned int v0, unsigned int v1);
    void uniform_3u(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2);
    void uniform_4u(const std::string &name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);

    void uniform_1f(const std::string &name, const glm::vec1 &v);
    void uniform_2f(const std::string &name, const glm::vec2 &v);
    void uniform_3f(const std::string &name, const glm::vec3 &v);
    void uniform_4f(const std::string &name, const glm::vec4 &v);

    void uniform_1i(const std::string &name, const glm::ivec1 &v);
    void uniform_2i(const std::string &name, const glm::ivec2 &v);
    void uniform_3i(const std::string &name, const glm::ivec3 &v);
    void uniform_4i(const std::string &name, const glm::ivec4 &v);

    void uniform_1u(const std::string &name, const glm::uvec1 &v);
    void uniform_2u(const std::string &name, const glm::uvec2 &v);
    void uniform_3u(const std::string &name, const glm::uvec3 &v);
    void uniform_4u(const std::string &name, const glm::uvec4 &v);

    void uniform_mat2(const std::string &name, const glm::mat2 &v);
    void uniform_mat3(const std::string &name, const glm::mat3 &v);
    void uniform_mat4(const std::string &name, const glm::mat4 &v);

    void uniform_mat2x3(const std::string &name, const glm::mat2x3 &v);
    void uniform_mat3x2(const std::string &name, const glm::mat3x2 &v);

    void uniform_mat2x4(const std::string &name, const glm::mat2x4 &v);
    void uniform_mat4x2(const std::string &name, const glm::mat4x2 &v);

    void uniform_mat3x4(const std::string &name, const glm::mat3x4 &v);
    void uniform_mat4x3(const std::string &name, const glm::mat4x3 &v);

    Shader(const Shader &other) = delete;
    Shader &operator=(const Shader &other) = delete;

    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

private:
    std::unordered_map<std::string, GLint> uniform_locations_{};
    std::unordered_set<std::string> bad_uniform_locations_{};
    std::unordered_map<std::string, GLint> attrib_locations_{};
    std::unordered_set<std::string> bad_attrib_locations_{};

    std::optional<GLint> try_get_uniform_location_(const std::string &name);
};

enum class ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

class ShaderBuilder {
public:
    ShaderBuilder();
    ~ShaderBuilder();

    ShaderBuilder(const ShaderBuilder &other) = delete;
    ShaderBuilder &operator=(const ShaderBuilder &other) = delete;

    ShaderBuilder(ShaderBuilder &&other) noexcept = delete;
    ShaderBuilder &operator=(ShaderBuilder &&other) noexcept = delete;

    ShaderBuilder &add_stage_src(ShaderType type, std::string source);
    ShaderBuilder &add_stage_path(ShaderType type, const std::filesystem::path &path);

    std::unique_ptr<Shader> build();

private:
    GLuint id_;
    std::vector<std::tuple<ShaderType, GLuint>> shader_ids_;

    struct ShaderSource_ {
        ShaderType type;
        std::variant<std::string, std::filesystem::path> source;
    };
    std::vector<ShaderSource_> stages_;

    bool try_compile_(GLuint id, ShaderType type, const std::string &src);
    bool try_link_(GLuint id);
};
} // namespace gloo

template<>
struct fmt::formatter<gloo::ShaderType> : formatter<std::string_view> {
    template<typename FormatContext>
    auto format(gloo::ShaderType type, FormatContext &ctx) const {
        switch (type) {
        case gloo::ShaderType::Vertex:
            return formatter<std::string_view>::format("vertex", ctx);
        case gloo::ShaderType::Fragment:
            return formatter<std::string_view>::format("fragment", ctx);
        default:
            std::unreachable();
        }
    }
};
