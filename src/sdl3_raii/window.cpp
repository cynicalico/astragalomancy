#include "sdl3_raii/window.hpp"

#include "sdl3_raii/gl_attr.hpp"

#include "astra/core/log.hpp"

// #include "bismuth/util/io.hpp"

sdl3::Window::~Window() {
    SDL_GL_DestroyContext(gl_context_);
    SDL_DestroyWindow(handle_);
}

SDL_Window *sdl3::Window::handle() const {
    return handle_;
}

SDL_GLContext sdl3::Window::gl_context() const {
    return gl_context_;
}

// void sdl3::Window::set_icon(const std::filesystem::path &path) {
//     if (std::filesystem::is_directory(path)) {
//         SDL_Surface *surf = nullptr;
//
//         for (const auto &e: std::filesystem::directory_iterator(path)) {
//             if (!e.is_regular_file())
//                 continue;
//
//             const auto e_surf = read_image_to_sdl_surface(e.path());
//             if (!e_surf)
//                 continue;
//
//             if (surf) {
//                 if (!SDL_AddSurfaceAlternateImage(surf, e_surf))
//                     ASTRA_LOG_ERROR("Failed to add alternate image to SDL surface: {}", SDL_GetError());
//                 SDL_DestroySurface(e_surf);
//             } else {
//                 surf = e_surf;
//             }
//         }
//
//         if (surf && !SDL_SetWindowIcon(handle_, surf))
//             ASTRA_LOG_ERROR("Failed to set window icon: {}", SDL_GetError());
//     } else {
//         // TODO
//     }
// }

void sdl3::Window::hide() {
    if (!SDL_HideWindow(handle_))
        ASTRA_LOG_ERROR("Failed to hide window: {}", SDL_GetError());
}

void sdl3::Window::show() {
    if (!SDL_ShowWindow(handle_))
        ASTRA_LOG_ERROR("Failed to show window: {}", SDL_GetError());
}

glm::ivec2 sdl3::Window::size() const {
    glm::ivec2 s;
    if (!SDL_GetWindowSize(handle_, &s.x, &s.y))
        ASTRA_LOG_ERROR("Failed to get window size: {}", SDL_GetError());
    return s;
}

int sdl3::Window::width() const {
    int w;
    if (!SDL_GetWindowSize(handle_, &w, nullptr))
        ASTRA_LOG_ERROR("Failed to get window size: {}", SDL_GetError());
    return w;
}

int sdl3::Window::height() const {
    int h;
    if (!SDL_GetWindowSize(handle_, nullptr, &h))
        ASTRA_LOG_ERROR("Failed to get window size: {}", SDL_GetError());
    return h;
}

glm::ivec2 sdl3::Window::pixel_size() const {
    glm::ivec2 s;
    if (!SDL_GetWindowSizeInPixels(handle_, &s.x, &s.y))
        ASTRA_LOG_ERROR("Failed to get window size in pixels: {}", SDL_GetError());
    return s;
}

int sdl3::Window::pixel_width() const {
    int w;
    if (!SDL_GetWindowSizeInPixels(handle_, &w, nullptr))
        ASTRA_LOG_ERROR("Failed to get window size in pixels: {}", SDL_GetError());
    return w;
}

int sdl3::Window::pixel_height() const {
    int h;
    if (!SDL_GetWindowSizeInPixels(handle_, nullptr, &h))
        ASTRA_LOG_ERROR("Failed to get window size in pixels: {}", SDL_GetError());
    return h;
}

float sdl3::Window::aspect_ratio() const {
    return static_cast<float>(width()) / static_cast<float>(height());
}

void sdl3::Window::swap() {
    if (!SDL_GL_SwapWindow(handle_))
        ASTRA_LOG_ERROR("Failed to swap window: {}", SDL_GetError());
}

sdl3::Window::Window(SDL_Window *handle, SDL_GLContext gl_context, std::optional<std::filesystem::path> icon_path)
    : handle_(handle),
      gl_context_(gl_context) {
    // if (icon_path)
    //     set_icon(*icon_path);
}

sdl3::WindowBuilder::WindowBuilder(const std::string_view title, const glm::ivec2 size)
    : props_(SDL_CreateProperties()),
      location_(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y) {
    if (!SDL_SetStringProperty(props_, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title.data()))
        ASTRA_LOG_ERROR("Failed to set property, window title: {}", SDL_GetError());
}

sdl3::WindowBuilder &sdl3::WindowBuilder::fullscreen() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window fullscreen: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::opengl() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window opengl: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::hidden() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window hidden: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::borderless() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window borderless: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::resizable() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window resizable: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::minimized() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window minimized: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::maximized() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window maximized: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::mouse_grabbed() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window mouse grabbed: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::external() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window external: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::modal() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window modal: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::high_pixel_density() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window high pixel density: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::always_on_top() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window always on top: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::utility() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window utility: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::tooltip() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window tooltip: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::popup_menu() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window popup_menu: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::vulkan() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window vulkan: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::metal() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window metal: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::transparent() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true))
        ASTRA_LOG_ERROR("Failed to set property, window transparent: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::not_focusable() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN, false))
        ASTRA_LOG_ERROR("Failed to set property, window not focusable: {}", SDL_GetError());
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::position(int x, int y) {
    location_.x = x;
    location_.y = y;
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::display(int idx) {
    display_idx_ = idx;
    return *this;
}

sdl3::WindowBuilder &sdl3::WindowBuilder::icon(const std::filesystem::path &path) {
    icon_path_ = path;
    return *this;
}

std::unique_ptr<sdl3::Window> sdl3::WindowBuilder::build() {
    const auto window_size_rect_result = get_window_size_rect();
    if (!window_size_rect_result)
        ASTRA_LOG_ERROR("Failed to build window: {}", window_size_rect_result.error());

    const auto [x, y, w, h] = *window_size_rect_result;
    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_X_NUMBER, x))
        ASTRA_LOG_ERROR("Failed to set property, window create x number: {}", SDL_GetError());
    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y))
        ASTRA_LOG_ERROR("Failed to set property, window create y number: {}", SDL_GetError());
    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, w))
        ASTRA_LOG_ERROR("Failed to set property, window create width: {}", SDL_GetError());
    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, h))
        ASTRA_LOG_ERROR("Failed to set property, window create height: {}", SDL_GetError());

    auto window = SDL_CreateWindowWithProperties(props_);
    if (!window) {
        ASTRA_LOG_ERROR("Failed to create SDL window: {}", SDL_GetError());
        return nullptr;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        ASTRA_LOG_ERROR("Failed to create OpenGL context: {}", SDL_GetError());
        return nullptr;
    }
    SDL_GL_MakeCurrent(window, gl_context);

    return std::unique_ptr<Window>(new Window(window, gl_context, icon_path_));
}

std::expected<SDL_DisplayID, std::string> sdl3::WindowBuilder::get_display_id() {
    int display_count;
    auto displays = SDL_GetDisplays(&display_count);
    if (!displays)
        return std::unexpected(std::string(SDL_GetError()));

    if (display_count <= display_idx_) {
        SPDLOG_WARN("Requested display out of bounds ({} <= {}), using display 0", display_count, display_idx_);
        display_idx_ = 0;
    }

    SDL_DisplayID display_id = displays[display_idx_];
    SDL_free(displays);

    return display_id;
}

std::expected<SDL_Rect, std::string> sdl3::WindowBuilder::get_window_size_rect() {
    auto display_id_result = get_display_id();
    if (!display_id_result)
        return std::unexpected(display_id_result.error());
    auto display_id = display_id_result.value();

    SDL_Rect display_bounds;
    if (!SDL_GetDisplayBounds(display_id, &display_bounds))
        return std::unexpected(std::string(SDL_GetError()));

    if (SDL_GetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, false))
        return display_bounds;

    if (location_.x == SDL_WINDOWPOS_CENTERED && location_.y == SDL_WINDOWPOS_CENTERED) {
        location_.x = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
        location_.y = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
    } else {
        location_.x += display_bounds.x;
        location_.y += display_bounds.y;
    }

    return location_;
}
