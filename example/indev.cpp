#include "astra/astra.hpp"

#include "gloo/init.hpp"

#include "sdl3_raii/gl_attr.hpp"
#include "sdl3_raii/gl_context_flags.hpp"
#include "sdl3_raii/init.hpp"
#include "sdl3_raii/window.hpp"

int main(int, char *[]) {
    astra::log_platform();

    if (!sdl3::init())
        return 1;

    sdl3::GlAttr::set_context_version(4, 6);
    sdl3::GlAttr::set_context_profile(sdl3::GlProfile::Core);
#if !defined(NDEBUG)
    sdl3::GlAttr::set_context_flags().debug().set();
#endif
    auto window = sdl3::WindowBuilder("Indev", {1280, 720}).opengl().build();

    gloo::init();

    sdl3::exit();
}
