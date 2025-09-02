#pragma once

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

#include <expected>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

namespace sdl3 {
class Window {
    friend class WindowBuilder;

public:
    ~Window();

    Window(const Window &other) = delete;
    Window &operator=(const Window &other) = delete;

    Window(Window &&other) noexcept = delete;
    Window &operator=(Window &&other) noexcept = delete;

    [[nodiscard]] SDL_Window *handle() const;
    [[nodiscard]] SDL_GLContext gl_context() const;

    // void set_icon(const std::filesystem::path &path);

    void hide();
    void show();

    [[nodiscard]] glm::ivec2 size() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    [[nodiscard]] glm::ivec2 pixel_size() const;
    [[nodiscard]] int pixel_width() const;
    [[nodiscard]] int pixel_height() const;

    [[nodiscard]] float aspect_ratio() const;

    void swap();

private:
    SDL_Window *handle_;
    SDL_GLContext gl_context_;

    explicit Window(SDL_Window *handle, SDL_GLContext gl_context, std::optional<std::filesystem::path> icon_path);
};

class WindowBuilder {
    friend class Context;

public:
    WindowBuilder(std::string_view title, glm::ivec2 size);
    ~WindowBuilder() = default;

    WindowBuilder(const WindowBuilder &other) = delete;
    WindowBuilder &operator=(const WindowBuilder &other) = delete;

    WindowBuilder(WindowBuilder &&other) noexcept = delete;
    WindowBuilder &operator=(WindowBuilder &&other) noexcept = delete;

    WindowBuilder &fullscreen();
    WindowBuilder &opengl();
    WindowBuilder &hidden();
    WindowBuilder &borderless();
    WindowBuilder &resizable();
    WindowBuilder &minimized();
    WindowBuilder &maximized();
    WindowBuilder &mouse_grabbed();
    WindowBuilder &external();
    WindowBuilder &modal();
    WindowBuilder &high_pixel_density();
    WindowBuilder &always_on_top();
    WindowBuilder &utility();
    WindowBuilder &tooltip();
    WindowBuilder &popup_menu();
    WindowBuilder &vulkan();
    WindowBuilder &metal();
    WindowBuilder &transparent();
    WindowBuilder &not_focusable();
    WindowBuilder &position(int x, int y);
    WindowBuilder &display(int idx);

    WindowBuilder &icon(const std::filesystem::path &path);

    [[nodiscard]] std::unique_ptr<Window> build();

private:
    SDL_PropertiesID props_;
    SDL_Rect location_;
    int display_idx_{0};
    std::optional<std::filesystem::path> icon_path_{std::nullopt};

    std::expected<SDL_DisplayID, std::string> get_display_id();
    std::expected<SDL_Rect, std::string> get_window_size_rect();
};
} // namespace sdl3
