#include "gloo/shader.hpp"

#include "astra/core/log.hpp"
#include "astra/util/io.hpp"

#include <glm/gtc/type_ptr.hpp>

gloo::Shader::Shader(const GLuint id)
    : id(id) {}

gloo::Shader::~Shader() {
    if (id != 0) {
        glDeleteProgram(id);
        ASTRA_LOG_TRACE("Deleted shader program (id={})", id);
    }
}

gloo::Shader::Shader(Shader &&other) noexcept
    : id(other.id),
      uniform_locations_(std::move(other.uniform_locations_)),
      bad_uniform_locations_(std::move(other.bad_uniform_locations_)),
      attrib_locations_(std::move(other.attrib_locations_)),
      bad_attrib_locations_(std::move(other.bad_attrib_locations_)) {
    other.id = 0;
}

gloo::Shader &gloo::Shader::operator=(Shader &&other) noexcept {
    if (this != &other) {
        id = other.id;
        uniform_locations_ = std::move(other.uniform_locations_);
        bad_uniform_locations_ = std::move(other.bad_uniform_locations_);
        attrib_locations_ = std::move(other.attrib_locations_);
        bad_attrib_locations_ = std::move(other.bad_attrib_locations_);
        other.id = 0;
    }
    return *this;
}

void gloo::Shader::use() const {
    glUseProgram(id);
}

std::optional<GLuint> gloo::Shader::try_get_attrib_location(const std::string &name) {
    auto it = attrib_locations_.find(name);
    if (it == attrib_locations_.end()) {
        GLint loc = glGetAttribLocation(id, name.c_str());
        if (loc == -1) {
            if (const auto it2 = bad_attrib_locations_.find(name); it2 == bad_attrib_locations_.end()) {
                ASTRA_LOG_ERROR("Attrib \"{}\" not found", name);
                bad_attrib_locations_.insert(it2, name);
            }
            return std::nullopt;
        }
        it = attrib_locations_.emplace_hint(it, name, loc);
    }
    return it->second;
}

void gloo::Shader::uniform(const std::string &name, const float v0) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform1f(*loc, v0);
}

void gloo::Shader::uniform(const std::string &name, const float v0, const float v1) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform2f(*loc, v0, v1);
}

void gloo::Shader::uniform(const std::string &name, const float v0, const float v1, const float v2) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform3f(*loc, v0, v1, v2);
}

void gloo::Shader::uniform(const std::string &name, const float v0, const float v1, const float v2, const float v3) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform4f(*loc, v0, v1, v2, v3);
}

void gloo::Shader::uniform(const std::string &name, const int v0) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform1i(*loc, v0);
}

void gloo::Shader::uniform(const std::string &name, const int v0, const int v1) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform2i(*loc, v0, v1);
}

void gloo::Shader::uniform(const std::string &name, const int v0, const int v1, const int v2) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform3i(*loc, v0, v1, v2);
}

void gloo::Shader::uniform(const std::string &name, const int v0, const int v1, const int v2, const int v3) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform4i(*loc, v0, v1, v2, v3);
}

void gloo::Shader::uniform(const std::string &name, const unsigned int v0) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform1ui(*loc, v0);
}

void gloo::Shader::uniform(const std::string &name, const unsigned int v0, const unsigned int v1) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform2ui(*loc, v0, v1);
}

void gloo::Shader::uniform(
        const std::string &name, const unsigned int v0, const unsigned int v1, const unsigned int v2) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform3ui(*loc, v0, v1, v2);
}

void gloo::Shader::uniform(
        const std::string &name,
        const unsigned int v0,
        const unsigned int v1,
        const unsigned int v2,
        const unsigned int v3) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform4ui(*loc, v0, v1, v2, v3);
}

void gloo::Shader::uniform(const std::string &name, const glm::vec1 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform1fv(*loc, 1, &v.x);
}

void gloo::Shader::uniform(const std::string &name, const glm::vec2 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform2fv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::vec3 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform3fv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::vec4 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform4fv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::ivec1 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform1iv(*loc, 1, &v.x);
}

void gloo::Shader::uniform(const std::string &name, const glm::ivec2 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform2iv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::ivec3 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform3iv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::ivec4 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform4iv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::uvec1 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform1uiv(*loc, 1, &v.x);
}

void gloo::Shader::uniform(const std::string &name, const glm::uvec2 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform2uiv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::uvec3 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform3uiv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::uvec4 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniform4uiv(*loc, 1, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat2 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix2fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat3 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix3fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat4 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix4fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat2x3 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix2x3fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat3x2 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix3x2fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat2x4 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix2x4fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat4x2 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix4x2fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat3x4 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix3x4fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

void gloo::Shader::uniform(const std::string &name, const glm::mat4x3 &v) {
    if (const auto loc = try_get_uniform_location_(name))
        glUniformMatrix4x3fv(*loc, 1, GL_FALSE, glm::value_ptr(v));
}

std::optional<GLint> gloo::Shader::try_get_uniform_location_(const std::string &name) {
    auto it = uniform_locations_.find(name);
    if (it == uniform_locations_.end()) {
        GLint loc = glGetUniformLocation(id, name.c_str());
        if (loc == -1) {
            if (const auto it2 = bad_uniform_locations_.find(name); it2 == bad_uniform_locations_.end()) {
                ASTRA_LOG_ERROR("Uniform \"{}\" not found", name);
                bad_uniform_locations_.insert(it2, name);
            }
            return std::nullopt;
        }
        it = uniform_locations_.emplace_hint(it, name, loc);
    }
    return it->second;
}

gloo::ShaderBuilder::ShaderBuilder() {
    id_ = glCreateProgram();
    ASTRA_LOG_TRACE("Created shader program (id={})", id_);
}

gloo::ShaderBuilder::~ShaderBuilder() {
    for (const auto &[type, id]: shader_ids_) {
        glDeleteShader(id);
        ASTRA_LOG_TRACE("Deleted {} shader (id={})", type, id);
    }

    if (id_ != 0) {
        glDeleteProgram(id_);
        ASTRA_LOG_TRACE("Deleted unfinished shader program (id={})", id_);
    }
}

gloo::ShaderBuilder &gloo::ShaderBuilder::add_stage_src(ShaderType type, std::string source) {
    stages_.emplace_back(type, source);
    return *this;
}

gloo::ShaderBuilder &gloo::ShaderBuilder::add_stage_path(ShaderType type, const std::filesystem::path &path) {
    stages_.emplace_back(type, path);
    return *this;
}

std::unique_ptr<gloo::Shader> gloo::ShaderBuilder::build() {
    for (auto &[type, source]: stages_) {
        const auto id = glCreateShader(static_cast<GLenum>(type));
        ASTRA_LOG_TRACE("Created {} shader (id={})", type, id);
        shader_ids_.emplace_back(type, id);

        std::optional<std::string> source_opt;
        if (std::holds_alternative<std::filesystem::path>(source))
            source_opt = astra::read_file_to_string(std::get<std::filesystem::path>(source));
        else
            source_opt = std::get<std::string>(source);

        if (source_opt && try_compile_(id, type, *source_opt))
            glAttachShader(id_, id);
        else
            return nullptr;
    }

    if (!try_link_(id_))
        return nullptr;

    auto shader = std::unique_ptr<Shader>(new Shader(id_));
    id_ = 0; // don't delete the program when we go out of scope
    return shader;
}

bool gloo::ShaderBuilder::try_compile_(GLuint id, ShaderType type, const std::string &src) {
    const auto src_p = src.c_str();
    glShaderSource(id, 1, &src_p, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        int info_log_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);

        std::vector<GLchar> info_log(info_log_length);
        glGetShaderInfoLog(id, info_log_length, nullptr, &info_log[0]);
        const auto info_log_str = std::string(&info_log[0]);

        ASTRA_LOG_ERROR("Failed to compile {} shader id={}: {}", type, id, info_log_str);
    }

    return success == GL_TRUE;
}

bool gloo::ShaderBuilder::try_link_(GLuint id) {
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        int info_log_length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);

        std::vector<GLchar> info_log(info_log_length);
        glGetProgramInfoLog(id, info_log_length, nullptr, &info_log[0]);
        const auto info_log_str = std::string(&info_log[0]);

        ASTRA_LOG_ERROR("Failed to link shader program id={}: {}", id, info_log_str);
    }

    ASTRA_LOG_TRACE("Linked shader program id={}", id);
    return success == GL_TRUE;
}
