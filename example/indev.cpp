#include "astra/astra.hpp"
#include "glm/ext/matrix_clip_space.hpp"

class Indev {
public:
    std::optional<astra::Hermes::ID> hermes_id;

    std::shared_ptr<gloo::Shader> shader;
    std::unique_ptr<gloo::VertexArray> vao;
    std::unique_ptr<gloo::Buffer<float>> vbo;
    glm::mat4 projection{};

    Indev();
    ~Indev();

    void update(double dt);
    void draw();

private:
    void keyboard_event_callback_(const sdl3::KeyboardEvent *e);
    void mouse_event_callback_(const sdl3::MouseButtonEvent *e);
};

Indev::Indev() {
    hermes_id = astra::g.hermes->acquire_id();
    astra::g.hermes->subscribe<astra::Update>(*hermes_id, [this](const auto e) { update(e->dt); });
    astra::g.hermes->subscribe<astra::Draw>(*hermes_id, [this](const auto) { draw(); });
    astra::g.hermes->subscribe<sdl3::KeyboardEvent>(*hermes_id, [this](const auto e) { keyboard_event_callback_(e); });
    astra::g.hermes->subscribe<sdl3::MouseButtonEvent>(*hermes_id, [this](const auto e) { mouse_event_callback_(e); });

    shader = astra::g.shaders->add_shader("assets/shader/triangles.shader");
    if (!shader) throw std::runtime_error("Failed to load shader");

    const auto in_pos_loc = shader->try_get_attrib_location("in_pos").value();
    const auto in_color_loc = shader->try_get_attrib_location("in_color").value();
    vao = gloo::VertexArrayBuilder()
                  .attrib(in_pos_loc, 3, GL_FLOAT, GL_FALSE, 0, 0)
                  .attrib(in_color_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)
                  .build();

    vbo = std::make_unique<gloo::Buffer<float>>(6 * 3);
    // clang-format off
    vbo->add({
        100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        150.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        100.0f, 150.0f, 0.0f, 1.0f, 0.0f, 0.0f
    });
    // clang-format on
    vbo->sync();
}

Indev::~Indev() {
    astra::g.hermes->release_id(*hermes_id);
    hermes_id = std::nullopt;
}

void Indev::update(double dt) {
    projection = glm::ortho(
            0.0f, static_cast<float>(astra::g.window->width()), static_cast<float>(astra::g.window->height()), 0.0f);
}

void Indev::draw() {
    glViewport(0, 0, astra::g.window->width(), astra::g.window->height());

    gloo::clear(astra::rgb(0x0f0f0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->uniform_mat4("projection", projection);

    vao->bind();
    vbo->bind(0, 0, 6 * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vbo->unbind(0);
    vao->unbind();
}

void Indev::keyboard_event_callback_(const sdl3::KeyboardEvent *e) {
    switch (e->type) {
    case sdl3::KeyboardEventType::Down: break;
    case sdl3::KeyboardEventType::Up:
        switch (e->key) {
        case SDLK_ESCAPE: astra::g.running = false; break;
        default:;
        }
        break;
    default: std::unreachable();
    }
}

void Indev::mouse_event_callback_(const sdl3::MouseButtonEvent *e) {
    switch (e->type) {
    case sdl3::MouseButtonEventType::Down: break;
    case sdl3::MouseButtonEventType::Up: break;
    default: std::unreachable();
    }
}

int main(int, char *[]) {
    try {
        const auto app_info = sdl3::AppInfo{
                .name = "Indev",
                .version = "0.0.1",
                .identifier = "gay.cynicalico.indev",
                .creator = "cynicalico",
                .copyright = "This is free and unencumbered software released into the public domain.",
                .url = "https://github.com/cynicalico/astragalomancy",
                .type = sdl3::AppType::Game,
        };
        astra::init(app_info, [&] {
            return std::move(
                    sdl3::WindowBuilder(app_info.name, {1920, 1080}).resizable().icon("assets/icon/light/png"));
        });

        { // scope to destroy Indev before shutdown
            const auto app = Indev();
            astra::mainloop();
        }

        astra::shutdown();
    } catch (const std::exception &e) {
        fmt::println("");
        fmt::println(stderr, "Exception: {}", e);
        return 1;
    }

    return 0;
}
