#include "astra/astra.hpp"

#include "gloo/init.hpp"

#include "sdl3_raii/gl_attr.hpp"
#include "sdl3_raii/gl_context_flags.hpp"
#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

int main(int, char *[]) {
    astra::log_platform();

    const auto app_info = sdl3::AppInfo{
            .name = "Indev",
            .version = "0.0.1",
            .identifier = "gay.cynicalico.indev",
            .creator = "cynicalico",
            .copyright = "This is free and unencumbered software released into the public domain.",
            .url = "https://github.com/cynicalico/astragalomancy",
            .type = sdl3::AppType::Game,
    };
    if (!sdl3::init(app_info))
        return 1;

    sdl3::GlAttr::set_context_version(4, 6);
    sdl3::GlAttr::set_context_profile(sdl3::GlProfile::Core);
#if !defined(NDEBUG)
    sdl3::GlAttr::set_context_flags().debug().set();
#endif
    auto window = sdl3::WindowBuilder("Indev", {1280, 720}).opengl().build();

    gloo::init();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT: running = false; break;
            default: break;
            }
        }

        window->swap();
    }

    sdl3::exit();
}
