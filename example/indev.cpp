#include "astra/astra.hpp"
#include "gloo/gloo.hpp"
#include "sdl3_raii/sdl3_raii.hpp"

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
    const auto window = sdl3::WindowBuilder("Indev", {1280, 720}).opengl().build();
    gloo::init();

    const auto messenger = std::make_unique<astra::Messenger>();
    auto event_pump = sdl3::EventPump(messenger.get());

    bool running = true;

    messenger->subscribe<sdl3::AudioDeviceEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::CameraDeviceEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::ClipboardEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::DisplayEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::DropEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::GamepadAxisEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::GamepadButtonEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::GamepadDeviceEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::GamepadSensorEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::GamepadTouchpadEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::JoystickAxisEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::JoystickBallEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::JoystickBatteryEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::JoystickButtonEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::JoystickDeviceEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::JoystickHatEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::KeyboardDeviceEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::KeyboardEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::MouseButtonEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::MouseDeviceEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::MouseMotionEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::MouseWheelEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::PenAxisEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::PenButtonEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::PenMotionEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::PenProximityEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::PenTouchEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::QuitEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::RenderEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::SensorEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::TextEditingCandidatesEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::TextEditingEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::TextInputEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::UserEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });
    messenger->subscribe<sdl3::WindowEvent>([](const auto e) { ASTRA_LOG_INFO("{}", *e); });

    messenger->subscribe<sdl3::QuitEvent>([&running](auto) { running = false; });

    messenger->subscribe<sdl3::KeyboardEvent>([&running](const auto e) {
        if (e->type == sdl3::KeyboardEventType::Up && e->key == SDLK_ESCAPE)
            running = false;
    });

    while (running) {
        event_pump.pump();

        window->swap();
    }

    sdl3::exit();
}
