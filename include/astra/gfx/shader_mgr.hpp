#pragma once

#include "astra/core/hermes.hpp"
#include "gloo/shader.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace astra {
struct ShaderSrc {
    // path to this shader (also key in map)
    std::filesystem::path path;
    // plain file contents (for editing)
    std::string src;
    // is this a shader or just an included file
    bool is_shader;
    // parsed content (for compilation)
    std::vector<std::pair<gloo::ShaderType, std::string>> stages;
    // paths to files this shader includes (directly or indirectly)
    std::unordered_set<std::string> deps;
};

class ShaderMgr {
public:
    ShaderMgr();
    ~ShaderMgr();

    ShaderMgr(const ShaderMgr &other) = delete;
    ShaderMgr &operator=(const ShaderMgr &other) = delete;

    ShaderMgr(ShaderMgr &&other) noexcept = delete;
    ShaderMgr &operator=(ShaderMgr &&other) noexcept = delete;

    std::shared_ptr<gloo::Shader> add_shader(const std::filesystem::path &path);

    void draw_editor();

private:
    Hermes::ID hermes_id_;

    std::vector<std::pair<std::string, std::shared_ptr<gloo::Shader>>> pending_shaders_{};
    std::unordered_map<std::string, ShaderSrc> shader_src_{};
    std::unordered_map<std::string, std::shared_ptr<gloo::Shader>> shaders_{};

    std::optional<ShaderSrc> read_parse_shader_src_(const std::filesystem::path &path);
    std::optional<ShaderSrc> parse_shader_src_(const std::string &src);
    void parse_shader_stages_(ShaderSrc &shader_src);
    std::optional<std::string> parse_includes_(std::unordered_set<std::string> &deps, const std::string &src);

    void try_recompile_shader_(const std::string &path, const std::string &src);
    void sub_pending_shaders_();
};
} // namespace astra
