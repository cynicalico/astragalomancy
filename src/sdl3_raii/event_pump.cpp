#include "sdl3_raii/event_pump.hpp"

#include "sdl3_raii/events/audio.hpp"
#include "sdl3_raii/events/camera.hpp"
#include "sdl3_raii/events/clipboard.hpp"
#include "sdl3_raii/events/display.hpp"
#include "sdl3_raii/events/drop.hpp"
#include "sdl3_raii/events/gamepad.hpp"
#include "sdl3_raii/events/joystick.hpp"
#include "sdl3_raii/events/keyboard.hpp"
#include "sdl3_raii/events/mouse.hpp"
#include "sdl3_raii/events/pen.hpp"
#include "sdl3_raii/events/quit.hpp"
#include "sdl3_raii/events/raw.hpp"
#include "sdl3_raii/events/render.hpp"
#include "sdl3_raii/events/sensor.hpp"
#include "sdl3_raii/events/text.hpp"
#include "sdl3_raii/events/touch.hpp"
#include "sdl3_raii/events/user.hpp"
#include "sdl3_raii/events/window.hpp"

#include "astra/core/globals.hpp"

void publish_event(const SDL_Event &e);

namespace sdl3 {
void pump_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) publish_event(e);
}
} // namespace sdl3

void publish_event(const SDL_Event &e) {
    astra::g.msg->publish<sdl3::RawEvent>(e);

    switch (e.type) {
    case SDL_EVENT_AUDIO_DEVICE_ADDED:
    case SDL_EVENT_AUDIO_DEVICE_REMOVED:
    case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
        astra::g.msg->publish<sdl3::AudioDeviceEvent>(
                static_cast<sdl3::AudioEventType>(e.type), e.adevice.timestamp, e.adevice.which, e.adevice.recording);
        break;

    case SDL_EVENT_CAMERA_DEVICE_ADDED:
    case SDL_EVENT_CAMERA_DEVICE_REMOVED:
    case SDL_EVENT_CAMERA_DEVICE_APPROVED:
    case SDL_EVENT_CAMERA_DEVICE_DENIED:
        astra::g.msg->publish<sdl3::CameraDeviceEvent>(
                static_cast<sdl3::CameraEventType>(e.type), e.adevice.timestamp, e.adevice.which);
        break;

    case SDL_EVENT_CLIPBOARD_UPDATE: {
        std::vector<std::string> mime_types;
        for (std::size_t i = 0; i < e.clipboard.num_mime_types; ++i) mime_types.emplace_back(e.clipboard.mime_types[i]);
        astra::g.msg->publish<sdl3::ClipboardEvent>(e.clipboard.timestamp, e.clipboard.owner, mime_types);
    } break;

    case SDL_EVENT_DISPLAY_ORIENTATION:
    case SDL_EVENT_DISPLAY_ADDED:
    case SDL_EVENT_DISPLAY_REMOVED:
    case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
    case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
    case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
        astra::g.msg->publish<sdl3::DisplayEvent>(
                static_cast<sdl3::DisplayEventType>(e.type),
                e.display.timestamp,
                e.display.displayID,
                e.display.data1,
                e.display.data2);
        break;

    case SDL_EVENT_DROP_BEGIN:
    case SDL_EVENT_DROP_FILE:
    case SDL_EVENT_DROP_TEXT:
    case SDL_EVENT_DROP_COMPLETE:
    case SDL_EVENT_DROP_POSITION:
        astra::g.msg->publish<sdl3::DropEvent>(
                static_cast<sdl3::DropEventType>(e.type),
                e.drop.timestamp,
                e.drop.windowID,
                e.drop.x,
                e.drop.y,
                e.drop.source ? std::optional(e.drop.source) : std::nullopt,
                e.drop.data ? std::optional(e.drop.data) : std::nullopt);
        break;

    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        astra::g.msg->publish<sdl3::GamepadAxisEvent>(e.gaxis.timestamp, e.gaxis.which, e.gaxis.axis, e.gaxis.value);
        break;

    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        astra::g.msg->publish<sdl3::GamepadButtonEvent>(
                static_cast<sdl3::GamepadButtonEventType>(e.type),
                e.gbutton.timestamp,
                e.gbutton.which,
                e.gbutton.button,
                e.gbutton.down);
        break;

    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMOVED:
    case SDL_EVENT_GAMEPAD_REMAPPED:
    case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
    case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
        astra::g.msg->publish<sdl3::GamepadDeviceEvent>(
                static_cast<sdl3::GamepadDeviceEventType>(e.type), e.gdevice.timestamp, e.gdevice.which);
        break;

    case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: {
        std::array<float, 3> data;
        std::memcpy(data.data(), e.gsensor.data, sizeof(float) * 3);
        astra::g.msg->publish<sdl3::GamepadSensorEvent>(
                e.gsensor.timestamp,
                e.gsensor.which,
                static_cast<sdl3::GamepadSensorType>(e.gsensor.sensor),
                data,
                e.gsensor.sensor_timestamp);
    } break;

    case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
        astra::g.msg->publish<sdl3::GamepadTouchpadEvent>(
                static_cast<sdl3::GamepadTouchpadEventType>(e.type),
                e.gtouchpad.timestamp,
                e.gtouchpad.which,
                e.gtouchpad.touchpad,
                e.gtouchpad.finger,
                e.gtouchpad.x,
                e.gtouchpad.y,
                e.gtouchpad.pressure);
        break;

    case SDL_EVENT_JOYSTICK_AXIS_MOTION:
        astra::g.msg->publish<sdl3::JoystickAxisEvent>(e.jaxis.timestamp, e.jaxis.which, e.jaxis.axis, e.jaxis.value);
        break;

    case SDL_EVENT_JOYSTICK_BALL_MOTION:
        astra::g.msg->publish<sdl3::JoystickBallEvent>(
                e.jball.timestamp, e.jball.which, e.jball.ball, e.jball.xrel, e.jball.yrel);
        break;

    case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
        astra::g.msg->publish<sdl3::JoystickBatteryEvent>(
                e.jbattery.timestamp,
                e.jbattery.which,
                static_cast<sdl3::JoystickBatteryState>(e.jbattery.state),
                e.jbattery.percent);
        break;

    case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
    case SDL_EVENT_JOYSTICK_BUTTON_UP:
        astra::g.msg->publish<sdl3::JoystickButtonEvent>(
                static_cast<sdl3::JoystickButtonEventType>(e.type),
                e.jbutton.timestamp,
                e.jbutton.which,
                e.jbutton.button,
                e.jbutton.down);
        break;

    case SDL_EVENT_JOYSTICK_ADDED:
    case SDL_EVENT_JOYSTICK_REMOVED:
    case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
        astra::g.msg->publish<sdl3::JoystickDeviceEvent>(
                static_cast<sdl3::JoystickDeviceEventType>(e.type), e.jdevice.timestamp, e.jdevice.which);
        break;

    case SDL_EVENT_JOYSTICK_HAT_MOTION:
        astra::g.msg->publish<sdl3::JoystickHatEvent>(
                e.jhat.timestamp, e.jhat.which, e.jhat.hat, static_cast<sdl3::JoystickHatValue>(e.jhat.value));
        break;

    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
        astra::g.msg->publish<sdl3::KeyboardDeviceEvent>(
                static_cast<sdl3::KeyboardDeviceEventType>(e.type), e.key.timestamp, e.key.which);
        break;

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        astra::g.msg->publish<sdl3::KeyboardEvent>(
                static_cast<sdl3::KeyboardEventType>(e.type),
                e.key.timestamp,
                e.key.which,
                e.key.scancode,
                e.key.key,
                e.key.mod,
                e.key.raw,
                e.key.down,
                e.key.repeat);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        astra::g.msg->publish<sdl3::MouseButtonEvent>(
                static_cast<sdl3::MouseButtonEventType>(e.type),
                e.button.timestamp,
                e.button.windowID,
                e.button.which,
                static_cast<sdl3::MouseButtonFlags>(e.button.button),
                e.button.down,
                e.button.clicks,
                e.button.x,
                e.button.y);
        break;

    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
        astra::g.msg->publish<sdl3::MouseDeviceEvent>(
                static_cast<sdl3::MouseDeviceEventType>(e.type), e.button.timestamp, e.button.which);
        break;

    case SDL_EVENT_MOUSE_MOTION:
        astra::g.msg->publish<sdl3::MouseMotionEvent>(
                e.motion.timestamp,
                e.motion.windowID,
                e.motion.which,
                static_cast<sdl3::MouseButtonFlags>(e.motion.state),
                e.motion.x,
                e.motion.y,
                e.motion.xrel,
                e.motion.yrel);
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        astra::g.msg->publish<sdl3::MouseWheelEvent>(
                e.wheel.timestamp,
                e.wheel.windowID,
                e.wheel.which,
                e.wheel.x,
                e.wheel.y,
                static_cast<sdl3::MouseWheelDirection>(e.wheel.direction),
                e.wheel.mouse_x,
                e.wheel.mouse_y,
                e.wheel.integer_x,
                e.wheel.integer_y);
        break;

    case SDL_EVENT_PEN_AXIS:
        astra::g.msg->publish<sdl3::PenAxisEvent>(
                e.paxis.timestamp,
                e.paxis.windowID,
                e.paxis.which,
                static_cast<sdl3::PenInputFlags>(e.paxis.pen_state),
                e.paxis.x,
                e.paxis.y,
                static_cast<sdl3::PenAxis>(e.paxis.axis),
                e.paxis.value);
        break;

    case SDL_EVENT_PEN_BUTTON_DOWN:
    case SDL_EVENT_PEN_BUTTON_UP:
        astra::g.msg->publish<sdl3::PenButtonEvent>(
                static_cast<sdl3::PenButtonEventType>(e.type),
                e.pbutton.timestamp,
                e.pbutton.windowID,
                e.pbutton.which,
                static_cast<sdl3::PenInputFlags>(e.pbutton.pen_state),
                e.pbutton.x,
                e.pbutton.y,
                e.pbutton.button,
                e.pbutton.down);
        break;

    case SDL_EVENT_PEN_MOTION:
        astra::g.msg->publish<sdl3::PenMotionEvent>(
                e.pmotion.timestamp,
                e.pmotion.windowID,
                e.pmotion.which,
                static_cast<sdl3::PenInputFlags>(e.pmotion.pen_state),
                e.pmotion.x,
                e.pmotion.y);
        break;

    case SDL_EVENT_PEN_PROXIMITY_IN:
    case SDL_EVENT_PEN_PROXIMITY_OUT:
        astra::g.msg->publish<sdl3::PenProximityEvent>(
                static_cast<sdl3::PenProximityEventType>(e.type),
                e.pproximity.timestamp,
                e.pproximity.windowID,
                e.pproximity.which);
        break;

    case SDL_EVENT_PEN_UP:
    case SDL_EVENT_PEN_DOWN:
        astra::g.msg->publish<sdl3::PenTouchEvent>(
                static_cast<sdl3::PenTouchEventType>(e.type),
                e.ptouch.timestamp,
                e.ptouch.windowID,
                e.ptouch.which,
                static_cast<sdl3::PenInputFlags>(e.ptouch.pen_state),
                e.ptouch.x,
                e.ptouch.y,
                e.ptouch.eraser,
                e.ptouch.down);
        break;

    case SDL_EVENT_QUIT: astra::g.msg->publish<sdl3::QuitEvent>(e.quit.timestamp); break;

    case SDL_EVENT_RENDER_TARGETS_RESET:
    case SDL_EVENT_RENDER_DEVICE_RESET:
    case SDL_EVENT_RENDER_DEVICE_LOST:
        astra::g.msg->publish<sdl3::RenderEvent>(
                static_cast<sdl3::RenderEventType>(e.type), e.render.timestamp, e.render.windowID);
        break;

    case SDL_EVENT_SENSOR_UPDATE: {
        std::array<float, 6> data;
        std::memcpy(data.data(), e.sensor.data, sizeof(float) * 6);
        astra::g.msg->publish<sdl3::SensorEvent>(e.sensor.timestamp, e.sensor.which, data, e.sensor.sensor_timestamp);
    } break;

    case SDL_EVENT_TEXT_EDITING_CANDIDATES: {
        std::vector<std::string> candidates;
        for (std::size_t i = 0; i < e.edit_candidates.num_candidates; ++i) candidates.emplace_back(e.edit.text);
        astra::g.msg->publish<sdl3::TextEditingCandidatesEvent>(
                e.edit_candidates.timestamp,
                e.edit_candidates.windowID,
                candidates,
                e.edit_candidates.selected_candidate == -1
                        ? std::nullopt
                        : std::optional(static_cast<std::size_t>(e.edit_candidates.selected_candidate)),
                e.edit_candidates.horizontal);
    } break;

    case SDL_EVENT_TEXT_EDITING:
        astra::g.msg->publish<sdl3::TextEditingEvent>(
                e.edit.timestamp,
                e.edit.windowID,
                e.edit.text,
                e.edit.start == -1 ? std::nullopt : std::optional(static_cast<std::size_t>(e.edit.start)),
                e.edit.length == -1 ? std::nullopt : std::optional(static_cast<std::size_t>(e.edit.length)));
        break;

    case SDL_EVENT_TEXT_INPUT:
        astra::g.msg->publish<sdl3::TextInputEvent>(e.text.timestamp, e.text.windowID, e.text.text);
        break;

    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_MOTION:
    case SDL_EVENT_FINGER_CANCELED:
        astra::g.msg->publish<sdl3::TouchFingerEvent>(
                static_cast<sdl3::TouchFingerEventType>(e.type),
                e.tfinger.timestamp,
                e.tfinger.touchID,
                e.tfinger.fingerID,
                e.tfinger.x,
                e.tfinger.y,
                e.tfinger.dx,
                e.tfinger.dy,
                e.tfinger.pressure,
                e.tfinger.windowID);
        break;

    case SDL_EVENT_WINDOW_SHOWN:
    case SDL_EVENT_WINDOW_HIDDEN:
    case SDL_EVENT_WINDOW_EXPOSED:
    case SDL_EVENT_WINDOW_MOVED:
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
    case SDL_EVENT_WINDOW_MINIMIZED:
    case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    case SDL_EVENT_WINDOW_HIT_TEST:
    case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
    case SDL_EVENT_WINDOW_OCCLUDED:
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
    case SDL_EVENT_WINDOW_DESTROYED:
        astra::g.msg->publish<sdl3::WindowEvent>(
                static_cast<sdl3::WindowEventType>(e.type),
                e.window.timestamp,
                e.window.windowID,
                e.window.data1,
                e.window.data2);
        break;

    default:
        if (e.type >= SDL_EVENT_USER && e.type < SDL_EVENT_LAST) {
            astra::g.msg->publish<sdl3::UserEvent>(
                    e.type, e.user.timestamp, e.user.windowID, e.user.code, e.user.data1, e.user.data2);
        }
        break;
    }
}
