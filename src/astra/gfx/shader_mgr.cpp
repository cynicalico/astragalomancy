#include "astra/gfx/shader_mgr.hpp"

#include "astra/core/globals.hpp"
#include "astra/core/log.hpp"
#include "astra/core/payloads.hpp"
#include "astra/util/io.hpp"
#include "astra/util/module/dear.hpp"

#include <ctre.hpp>

constexpr ctll::fixed_string INCLUDE_PAT = R"re(#include\s+"(.+)")re";
constexpr ctll::fixed_string VERT_START_PAT = R"re(#pragma\s+vertex)re";
constexpr ctll::fixed_string VERT_END_PAT = R"re(#pragma\s+xetrev)re";
constexpr ctll::fixed_string FRAG_START_PAT = R"re(#pragma\s+fragment)re";
constexpr ctll::fixed_string FRAG_END_PAT = R"re(#pragma\s+tnemgarf)re";

void ensure_newline(std::string &s) {
    if (s.back() != '\n') s += '\n';
}

astra::ShaderMgr::ShaderMgr() {
    hermes_id_ = g.hermes->acquire_id();
    g.hermes->subscribe<PreUpdate>(hermes_id_, [&](const auto *) { sub_pending_shaders_(); });
}

astra::ShaderMgr::~ShaderMgr() {
    g.hermes->release_id(hermes_id_);
}

std::shared_ptr<gloo::Shader> astra::ShaderMgr::add_shader(const std::filesystem::path &path) {
    const auto shader_src = read_parse_shader_src_(path);
    if (!shader_src) return nullptr;

    auto b = gloo::ShaderBuilder();
    for (const auto &[type, src]: shader_src->stages) b.add_stage_src(type, src);
    auto shader = b.build();
    if (!shader) return nullptr;

    shader_src_.emplace(shader_src->path.string(), *shader_src);
    shaders_.emplace(shader_src->path.string(), shader);

    return shader;
}

void astra::ShaderMgr::draw_editor() {
    static bool show_editor = false;
    static char BUF[1024 * 1024];
    static std::optional<std::string> last_selected_shader{std::nullopt};

    if (ImGui::Button("Show editor")) show_editor = true;

    if (show_editor) {
        if (ImGui::Begin("Shader Editor", &show_editor)) {
            if (ImGui::Button("Recompile")) {
                if (last_selected_shader.has_value()) try_recompile_shader_(*last_selected_shader, BUF);
                else ASTRA_LOG_WARN("Can't recompile, no shader selected");
            }

            ImGui::InputTextMultiline("##src", BUF, sizeof(BUF), ImVec2(0, -1), ImGuiInputTextFlags_AllowTabInput);

            ImGui::SameLine();
            if (ImGui::BeginListBox("Shaders", ImVec2(0, -1))) {
                for (const auto &[path, shader_src]: shader_src_) {
                    if (ImGui::Selectable(path.c_str())) {
                        ASTRA_LOG_INFO("Selected shader: {}", path);
                        std::memset(BUF, 0, sizeof(BUF));
                        std::memcpy(BUF, shader_src.src.c_str(), shader_src.src.size());
                        last_selected_shader = path;
                    }
                }
                ImGui::EndListBox();
            }
        }
        ImGui::End();
    }
}

std::optional<astra::ShaderSrc> astra::ShaderMgr::read_parse_shader_src_(const std::filesystem::path &path) {
    if (const auto contents = read_file_to_string(path); contents) {
        auto shader_src = parse_shader_src_(*contents);
        if (shader_src) shader_src->path = path;
        return shader_src;
    }
    return std::nullopt;
}

std::optional<astra::ShaderSrc> astra::ShaderMgr::parse_shader_src_(const std::string &src) {
    ShaderSrc shader_src;
    shader_src.is_shader = true;
    shader_src.src = src;

    parse_shader_stages_(shader_src);
    for (auto &val: shader_src.stages | std::views::values) {
        std::unordered_set<std::string> deps;
        if (const auto src_no_include = parse_includes_(deps, val); src_no_include) val = *src_no_include;
        else return std::nullopt;
        shader_src.deps.insert(deps.begin(), deps.end());
    }

    return shader_src;
}

template<ctll::fixed_string PAT>
void read_until_match(astra::ShaderSrc &shader_src, std::istringstream &iss) {
    std::string line;
    while (std::getline(iss, line)) {
        if (ctre::match<PAT>(line)) return;
        shader_src.stages.back().second += line;
        ensure_newline(shader_src.stages.back().second);
    }
}

void astra::ShaderMgr::parse_shader_stages_(ShaderSrc &shader_src) {
    std::istringstream iss{shader_src.src};
    std::string line;
    while (std::getline(iss, line)) {
        if (ctre::match<VERT_START_PAT>(line)) {
            shader_src.stages.emplace_back(gloo::ShaderType::Vertex, "");
            read_until_match<VERT_END_PAT>(shader_src, iss);
        } else if (ctre::match<FRAG_START_PAT>(line)) {
            shader_src.stages.emplace_back(gloo::ShaderType::Fragment, "");
            read_until_match<FRAG_END_PAT>(shader_src, iss);
        }
    }
}

std::optional<std::string>
astra::ShaderMgr::parse_includes_(std::unordered_set<std::string> &deps, const std::string &src) {
    std::string result;

    std::string line;
    std::istringstream iss{src};
    while (std::getline(iss, line)) {
        if (const auto match = ctre::match<INCLUDE_PAT>(line)) {
            const auto include_path = match.get<1>().to_string();
            if (deps.contains(include_path)) continue;
            deps.insert(include_path);

            if (shader_src_.contains(include_path)) {
                result += shader_src_.at(include_path).src;
                ensure_newline(result);
            } else if (const auto contents = read_file_to_string(include_path); contents) {
                if (const auto contents_no_include = parse_includes_(deps, *contents); contents_no_include) {
                    result += *contents_no_include;
                    ensure_newline(result);
                } else {
                    return std::nullopt;
                }
            } else {
                return std::nullopt;
            }
        } else {
            result += line;
            ensure_newline(result);
        }
    }

    return result;
}

void astra::ShaderMgr::try_recompile_shader_(const std::string &path, const std::string &src) {
    const auto new_shader_src = parse_shader_src_(src);
    if (!new_shader_src) {
        ASTRA_LOG_ERROR("Failed to parse shader");
        return;
    }

    auto b = gloo::ShaderBuilder();
    for (const auto &[type, src]: new_shader_src->stages) b.add_stage_src(type, src);
    auto shader = b.build();
    if (!shader) {
        ASTRA_LOG_ERROR("Failed to build shader");
        return;
    }

    pending_shaders_.emplace_back(path, shader);
}

void astra::ShaderMgr::sub_pending_shaders_() {
    for (auto &[path, shader]: pending_shaders_) *shaders_[path] = std::move(*shader);
    pending_shaders_.clear();
}
