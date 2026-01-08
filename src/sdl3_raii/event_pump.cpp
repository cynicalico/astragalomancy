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
#include "sdl3_raii/events/theme.hpp"
#include "sdl3_raii/events/touch.hpp"
#include "sdl3_raii/events/user.hpp"
#include "sdl3_raii/events/window.hpp"

#include "astra/core/globals.hpp"
#include "astra/core/log.hpp"

void publish_event(const SDL_Event &e);
const char *event_type_to_string(std::uint32_t type);

namespace sdl3 {
void pump_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) publish_event(e);
}
} // namespace sdl3

void publish_event(const SDL_Event &e) {
    astra::g.hermes->publish<sdl3::RawEvent>(e);

    switch (e.type) {
    case SDL_EVENT_SYSTEM_THEME_CHANGED:
        astra::g.hermes->publish<sdl3::SystemThemeChangedEvent>(e.common.timestamp);
        break;

    case SDL_EVENT_AUDIO_DEVICE_ADDED:
    case SDL_EVENT_AUDIO_DEVICE_REMOVED:
    case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
        astra::g.hermes->publish<sdl3::AudioDeviceEvent>(
                static_cast<sdl3::AudioEventType>(e.type), e.adevice.timestamp, e.adevice.which, e.adevice.recording);
        break;

    case SDL_EVENT_CAMERA_DEVICE_ADDED:
    case SDL_EVENT_CAMERA_DEVICE_REMOVED:
    case SDL_EVENT_CAMERA_DEVICE_APPROVED:
    case SDL_EVENT_CAMERA_DEVICE_DENIED:
        astra::g.hermes->publish<sdl3::CameraDeviceEvent>(
                static_cast<sdl3::CameraEventType>(e.type), e.adevice.timestamp, e.adevice.which);
        break;

    case SDL_EVENT_CLIPBOARD_UPDATE: {
        std::vector<std::string> mime_types;
        for (std::size_t i = 0; i < e.clipboard.num_mime_types; ++i) mime_types.emplace_back(e.clipboard.mime_types[i]);
        astra::g.hermes->publish<sdl3::ClipboardEvent>(e.clipboard.timestamp, e.clipboard.owner, mime_types);
    } break;

    case SDL_EVENT_DISPLAY_ORIENTATION:
    case SDL_EVENT_DISPLAY_ADDED:
    case SDL_EVENT_DISPLAY_REMOVED:
    case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
    case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
    case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
        astra::g.hermes->publish<sdl3::DisplayEvent>(
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
        astra::g.hermes->publish<sdl3::DropEvent>(
                static_cast<sdl3::DropEventType>(e.type),
                e.drop.timestamp,
                e.drop.windowID,
                e.drop.x,
                e.drop.y,
                e.drop.source ? std::optional(e.drop.source) : std::nullopt,
                e.drop.data ? std::optional(e.drop.data) : std::nullopt);
        break;

    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        astra::g.hermes->publish<sdl3::GamepadAxisEvent>(e.gaxis.timestamp, e.gaxis.which, e.gaxis.axis, e.gaxis.value);
        break;

    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        astra::g.hermes->publish<sdl3::GamepadButtonEvent>(
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
        astra::g.hermes->publish<sdl3::GamepadDeviceEvent>(
                static_cast<sdl3::GamepadDeviceEventType>(e.type), e.gdevice.timestamp, e.gdevice.which);
        break;

    case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: {
        std::array<float, 3> data{};
        std::memcpy(data.data(), e.gsensor.data, sizeof(float) * 3);
        astra::g.hermes->publish<sdl3::GamepadSensorEvent>(
                e.gsensor.timestamp,
                e.gsensor.which,
                static_cast<sdl3::GamepadSensorType>(e.gsensor.sensor),
                data,
                e.gsensor.sensor_timestamp);
    } break;

    case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
        astra::g.hermes->publish<sdl3::GamepadTouchpadEvent>(
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
        astra::g.hermes->publish<sdl3::JoystickAxisEvent>(e.jaxis.timestamp, e.jaxis.which, e.jaxis.axis, e.jaxis.value);
        break;

    case SDL_EVENT_JOYSTICK_BALL_MOTION:
        astra::g.hermes->publish<sdl3::JoystickBallEvent>(
                e.jball.timestamp, e.jball.which, e.jball.ball, e.jball.xrel, e.jball.yrel);
        break;

    case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
        astra::g.hermes->publish<sdl3::JoystickBatteryEvent>(
                e.jbattery.timestamp,
                e.jbattery.which,
                static_cast<sdl3::JoystickBatteryState>(e.jbattery.state),
                e.jbattery.percent);
        break;

    case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
    case SDL_EVENT_JOYSTICK_BUTTON_UP:
        astra::g.hermes->publish<sdl3::JoystickButtonEvent>(
                static_cast<sdl3::JoystickButtonEventType>(e.type),
                e.jbutton.timestamp,
                e.jbutton.which,
                e.jbutton.button,
                e.jbutton.down);
        break;

    case SDL_EVENT_JOYSTICK_ADDED:
    case SDL_EVENT_JOYSTICK_REMOVED:
    case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
        astra::g.hermes->publish<sdl3::JoystickDeviceEvent>(
                static_cast<sdl3::JoystickDeviceEventType>(e.type), e.jdevice.timestamp, e.jdevice.which);
        break;

    case SDL_EVENT_JOYSTICK_HAT_MOTION:
        astra::g.hermes->publish<sdl3::JoystickHatEvent>(
                e.jhat.timestamp, e.jhat.which, e.jhat.hat, static_cast<sdl3::JoystickHatValue>(e.jhat.value));
        break;

    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
        astra::g.hermes->publish<sdl3::KeyboardDeviceEvent>(
                static_cast<sdl3::KeyboardDeviceEventType>(e.type), e.key.timestamp, e.key.which);
        break;

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        astra::g.hermes->publish<sdl3::KeyboardEvent>(
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
        astra::g.hermes->publish<sdl3::MouseButtonEvent>(
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
        astra::g.hermes->publish<sdl3::MouseDeviceEvent>(
                static_cast<sdl3::MouseDeviceEventType>(e.type), e.button.timestamp, e.button.which);
        break;

    case SDL_EVENT_MOUSE_MOTION:
        astra::g.hermes->publish<sdl3::MouseMotionEvent>(
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
        astra::g.hermes->publish<sdl3::MouseWheelEvent>(
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
        astra::g.hermes->publish<sdl3::PenAxisEvent>(
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
        astra::g.hermes->publish<sdl3::PenButtonEvent>(
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
        astra::g.hermes->publish<sdl3::PenMotionEvent>(
                e.pmotion.timestamp,
                e.pmotion.windowID,
                e.pmotion.which,
                static_cast<sdl3::PenInputFlags>(e.pmotion.pen_state),
                e.pmotion.x,
                e.pmotion.y);
        break;

    case SDL_EVENT_PEN_PROXIMITY_IN:
    case SDL_EVENT_PEN_PROXIMITY_OUT:
        astra::g.hermes->publish<sdl3::PenProximityEvent>(
                static_cast<sdl3::PenProximityEventType>(e.type),
                e.pproximity.timestamp,
                e.pproximity.windowID,
                e.pproximity.which);
        break;

    case SDL_EVENT_PEN_UP:
    case SDL_EVENT_PEN_DOWN:
        astra::g.hermes->publish<sdl3::PenTouchEvent>(
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

    case SDL_EVENT_QUIT: astra::g.hermes->publish<sdl3::QuitEvent>(e.quit.timestamp); break;

    case SDL_EVENT_RENDER_TARGETS_RESET:
    case SDL_EVENT_RENDER_DEVICE_RESET:
    case SDL_EVENT_RENDER_DEVICE_LOST:
        astra::g.hermes->publish<sdl3::RenderEvent>(
                static_cast<sdl3::RenderEventType>(e.type), e.render.timestamp, e.render.windowID);
        break;

    case SDL_EVENT_SENSOR_UPDATE: {
        std::array<float, 6> data{};
        std::memcpy(data.data(), e.sensor.data, sizeof(float) * 6);
        astra::g.hermes->publish<sdl3::SensorEvent>(e.sensor.timestamp, e.sensor.which, data, e.sensor.sensor_timestamp);
    } break;

    case SDL_EVENT_TEXT_EDITING_CANDIDATES: {
        std::vector<std::string> candidates;
        for (std::size_t i = 0; i < e.edit_candidates.num_candidates; ++i) candidates.emplace_back(e.edit.text);
        astra::g.hermes->publish<sdl3::TextEditingCandidatesEvent>(
                e.edit_candidates.timestamp,
                e.edit_candidates.windowID,
                candidates,
                e.edit_candidates.selected_candidate == -1
                        ? std::nullopt
                        : std::optional(static_cast<std::size_t>(e.edit_candidates.selected_candidate)),
                e.edit_candidates.horizontal);
    } break;

    case SDL_EVENT_TEXT_EDITING:
        astra::g.hermes->publish<sdl3::TextEditingEvent>(
                e.edit.timestamp,
                e.edit.windowID,
                e.edit.text,
                e.edit.start == -1 ? std::nullopt : std::optional(static_cast<std::size_t>(e.edit.start)),
                e.edit.length == -1 ? std::nullopt : std::optional(static_cast<std::size_t>(e.edit.length)));
        break;

    case SDL_EVENT_TEXT_INPUT:
        astra::g.hermes->publish<sdl3::TextInputEvent>(e.text.timestamp, e.text.windowID, e.text.text);
        break;

    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_MOTION:
    case SDL_EVENT_FINGER_CANCELED:
        astra::g.hermes->publish<sdl3::TouchFingerEvent>(
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
        astra::g.hermes->publish<sdl3::WindowEvent>(
                static_cast<sdl3::WindowEventType>(e.type),
                e.window.timestamp,
                e.window.windowID,
                e.window.data1,
                e.window.data2);
        break;

    default:
        if (e.type >= SDL_EVENT_USER && e.type < SDL_EVENT_LAST) {
            astra::g.hermes->publish<sdl3::UserEvent>(
                    e.type, e.user.timestamp, e.user.windowID, e.user.code, e.user.data1, e.user.data2);
        } else {
            ASTRA_LOG_DEBUG("Unhandled event: {} (0x{:x})", event_type_to_string(e.type), e.type);
        }
        break;
    }
}

const char *event_type_to_string(std::uint32_t type) {
    if (type >= SDL_EVENT_USER && type < SDL_EVENT_LAST) return "SDL_EVENT_USER";

    switch (type) {
    // case SDL_EVENT_FIRST: return "SDL_EVENT_FIRST";
    case SDL_EVENT_QUIT: return "SDL_EVENT_QUIT";
    case SDL_EVENT_TERMINATING: return "SDL_EVENT_TERMINATING";
    case SDL_EVENT_LOW_MEMORY: return "SDL_EVENT_LOW_MEMORY";
    case SDL_EVENT_WILL_ENTER_BACKGROUND: return "SDL_EVENT_WILL_ENTER_BACKGROUND";
    case SDL_EVENT_DID_ENTER_BACKGROUND: return "SDL_EVENT_DID_ENTER_BACKGROUND";
    case SDL_EVENT_WILL_ENTER_FOREGROUND: return "SDL_EVENT_WILL_ENTER_FOREGROUND";
    case SDL_EVENT_DID_ENTER_FOREGROUND: return "SDL_EVENT_DID_ENTER_FOREGROUND";
    case SDL_EVENT_LOCALE_CHANGED: return "SDL_EVENT_LOCALE_CHANGED";
    case SDL_EVENT_SYSTEM_THEME_CHANGED: return "SDL_EVENT_SYSTEM_THEME_CHANGED";
    case SDL_EVENT_DISPLAY_ORIENTATION: return "SDL_EVENT_DISPLAY_ORIENTATION";
    case SDL_EVENT_DISPLAY_ADDED: return "SDL_EVENT_DISPLAY_ADDED";
    case SDL_EVENT_DISPLAY_REMOVED: return "SDL_EVENT_DISPLAY_REMOVED";
    case SDL_EVENT_DISPLAY_MOVED: return "SDL_EVENT_DISPLAY_MOVED";
    case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED: return "SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED";
    case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED: return "SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED";
    case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: return "SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED";
    case SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED: return "SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED";
    case SDL_EVENT_WINDOW_SHOWN: return "SDL_EVENT_WINDOW_SHOWN";
    case SDL_EVENT_WINDOW_HIDDEN: return "SDL_EVENT_WINDOW_HIDDEN";
    case SDL_EVENT_WINDOW_EXPOSED: return "SDL_EVENT_WINDOW_EXPOSED";
    case SDL_EVENT_WINDOW_MOVED: return "SDL_EVENT_WINDOW_MOVED";
    case SDL_EVENT_WINDOW_RESIZED: return "SDL_EVENT_WINDOW_RESIZED";
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: return "SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED";
    case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED: return "SDL_EVENT_WINDOW_METAL_VIEW_RESIZED";
    case SDL_EVENT_WINDOW_MINIMIZED: return "SDL_EVENT_WINDOW_MINIMIZED";
    case SDL_EVENT_WINDOW_MAXIMIZED: return "SDL_EVENT_WINDOW_MAXIMIZED";
    case SDL_EVENT_WINDOW_RESTORED: return "SDL_EVENT_WINDOW_RESTORED";
    case SDL_EVENT_WINDOW_MOUSE_ENTER: return "SDL_EVENT_WINDOW_MOUSE_ENTER";
    case SDL_EVENT_WINDOW_MOUSE_LEAVE: return "SDL_EVENT_WINDOW_MOUSE_LEAVE";
    case SDL_EVENT_WINDOW_FOCUS_GAINED: return "SDL_EVENT_WINDOW_FOCUS_GAINED";
    case SDL_EVENT_WINDOW_FOCUS_LOST: return "SDL_EVENT_WINDOW_FOCUS_LOST";
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: return "SDL_EVENT_WINDOW_CLOSE_REQUESTED";
    case SDL_EVENT_WINDOW_HIT_TEST: return "SDL_EVENT_WINDOW_HIT_TEST";
    case SDL_EVENT_WINDOW_ICCPROF_CHANGED: return "SDL_EVENT_WINDOW_ICCPROF_CHANGED";
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED: return "SDL_EVENT_WINDOW_DISPLAY_CHANGED";
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: return "SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED";
    case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED: return "SDL_EVENT_WINDOW_SAFE_AREA_CHANGED";
    case SDL_EVENT_WINDOW_OCCLUDED: return "SDL_EVENT_WINDOW_OCCLUDED";
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: return "SDL_EVENT_WINDOW_ENTER_FULLSCREEN";
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: return "SDL_EVENT_WINDOW_LEAVE_FULLSCREEN";
    case SDL_EVENT_WINDOW_DESTROYED: return "SDL_EVENT_WINDOW_DESTROYED";
    case SDL_EVENT_WINDOW_HDR_STATE_CHANGED: return "SDL_EVENT_WINDOW_HDR_STATE_CHANGED";
    case SDL_EVENT_KEY_DOWN: return "SDL_EVENT_KEY_DOWN";
    case SDL_EVENT_KEY_UP: return "SDL_EVENT_KEY_UP";
    case SDL_EVENT_TEXT_EDITING: return "SDL_EVENT_TEXT_EDITING";
    case SDL_EVENT_TEXT_INPUT: return "SDL_EVENT_TEXT_INPUT";
    case SDL_EVENT_KEYMAP_CHANGED: return "SDL_EVENT_KEYMAP_CHANGED";
    case SDL_EVENT_KEYBOARD_ADDED: return "SDL_EVENT_KEYBOARD_ADDED";
    case SDL_EVENT_KEYBOARD_REMOVED: return "SDL_EVENT_KEYBOARD_REMOVED";
    case SDL_EVENT_TEXT_EDITING_CANDIDATES: return "SDL_EVENT_TEXT_EDITING_CANDIDATES";
    case SDL_EVENT_SCREEN_KEYBOARD_SHOWN: return "SDL_EVENT_SCREEN_KEYBOARD_SHOWN";
    case SDL_EVENT_SCREEN_KEYBOARD_HIDDEN: return "SDL_EVENT_SCREEN_KEYBOARD_HIDDEN";
    case SDL_EVENT_MOUSE_MOTION: return "SDL_EVENT_MOUSE_MOTION";
    case SDL_EVENT_MOUSE_BUTTON_DOWN: return "SDL_EVENT_MOUSE_BUTTON_DOWN";
    case SDL_EVENT_MOUSE_BUTTON_UP: return "SDL_EVENT_MOUSE_BUTTON_UP";
    case SDL_EVENT_MOUSE_WHEEL: return "SDL_EVENT_MOUSE_WHEEL";
    case SDL_EVENT_MOUSE_ADDED: return "SDL_EVENT_MOUSE_ADDED";
    case SDL_EVENT_MOUSE_REMOVED: return "SDL_EVENT_MOUSE_REMOVED";
    case SDL_EVENT_JOYSTICK_AXIS_MOTION: return "SDL_EVENT_JOYSTICK_AXIS_MOTION";
    case SDL_EVENT_JOYSTICK_BALL_MOTION: return "SDL_EVENT_JOYSTICK_BALL_MOTION";
    case SDL_EVENT_JOYSTICK_HAT_MOTION: return "SDL_EVENT_JOYSTICK_HAT_MOTION";
    case SDL_EVENT_JOYSTICK_BUTTON_DOWN: return "SDL_EVENT_JOYSTICK_BUTTON_DOWN";
    case SDL_EVENT_JOYSTICK_BUTTON_UP: return "SDL_EVENT_JOYSTICK_BUTTON_UP";
    case SDL_EVENT_JOYSTICK_ADDED: return "SDL_EVENT_JOYSTICK_ADDED";
    case SDL_EVENT_JOYSTICK_REMOVED: return "SDL_EVENT_JOYSTICK_REMOVED";
    case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: return "SDL_EVENT_JOYSTICK_BATTERY_UPDATED";
    case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: return "SDL_EVENT_JOYSTICK_UPDATE_COMPLETE";
    case SDL_EVENT_GAMEPAD_AXIS_MOTION: return "SDL_EVENT_GAMEPAD_AXIS_MOTION";
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN: return "SDL_EVENT_GAMEPAD_BUTTON_DOWN";
    case SDL_EVENT_GAMEPAD_BUTTON_UP: return "SDL_EVENT_GAMEPAD_BUTTON_UP";
    case SDL_EVENT_GAMEPAD_ADDED: return "SDL_EVENT_GAMEPAD_ADDED";
    case SDL_EVENT_GAMEPAD_REMOVED: return "SDL_EVENT_GAMEPAD_REMOVED";
    case SDL_EVENT_GAMEPAD_REMAPPED: return "SDL_EVENT_GAMEPAD_REMAPPED";
    case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: return "SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN";
    case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: return "SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION";
    case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: return "SDL_EVENT_GAMEPAD_TOUCHPAD_UP";
    case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: return "SDL_EVENT_GAMEPAD_SENSOR_UPDATE";
    case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: return "SDL_EVENT_GAMEPAD_UPDATE_COMPLETE";
    case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: return "SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED";
    case SDL_EVENT_FINGER_DOWN: return "SDL_EVENT_FINGER_DOWN";
    case SDL_EVENT_FINGER_UP: return "SDL_EVENT_FINGER_UP";
    case SDL_EVENT_FINGER_MOTION: return "SDL_EVENT_FINGER_MOTION";
    case SDL_EVENT_FINGER_CANCELED: return "SDL_EVENT_FINGER_CANCELED";
    case SDL_EVENT_PINCH_BEGIN: return "SDL_EVENT_PINCH_BEGIN";
    case SDL_EVENT_PINCH_UPDATE: return "SDL_EVENT_PINCH_UPDATE";
    case SDL_EVENT_PINCH_END: return "SDL_EVENT_PINCH_END";
    case SDL_EVENT_CLIPBOARD_UPDATE: return "SDL_EVENT_CLIPBOARD_UPDATE";
    case SDL_EVENT_DROP_FILE: return "SDL_EVENT_DROP_FILE";
    case SDL_EVENT_DROP_TEXT: return "SDL_EVENT_DROP_TEXT";
    case SDL_EVENT_DROP_BEGIN: return "SDL_EVENT_DROP_BEGIN";
    case SDL_EVENT_DROP_COMPLETE: return "SDL_EVENT_DROP_COMPLETE";
    case SDL_EVENT_DROP_POSITION: return "SDL_EVENT_DROP_POSITION";
    case SDL_EVENT_AUDIO_DEVICE_ADDED: return "SDL_EVENT_AUDIO_DEVICE_ADDED";
    case SDL_EVENT_AUDIO_DEVICE_REMOVED: return "SDL_EVENT_AUDIO_DEVICE_REMOVED";
    case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: return "SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED";
    case SDL_EVENT_SENSOR_UPDATE: return "SDL_EVENT_SENSOR_UPDATE";
    case SDL_EVENT_PEN_PROXIMITY_IN: return "SDL_EVENT_PEN_PROXIMITY_IN";
    case SDL_EVENT_PEN_PROXIMITY_OUT: return "SDL_EVENT_PEN_PROXIMITY_OUT";
    case SDL_EVENT_PEN_DOWN: return "SDL_EVENT_PEN_DOWN";
    case SDL_EVENT_PEN_UP: return "SDL_EVENT_PEN_UP";
    case SDL_EVENT_PEN_BUTTON_DOWN: return "SDL_EVENT_PEN_BUTTON_DOWN";
    case SDL_EVENT_PEN_BUTTON_UP: return "SDL_EVENT_PEN_BUTTON_UP";
    case SDL_EVENT_PEN_MOTION: return "SDL_EVENT_PEN_MOTION";
    case SDL_EVENT_PEN_AXIS: return "SDL_EVENT_PEN_AXIS";
    case SDL_EVENT_CAMERA_DEVICE_ADDED: return "SDL_EVENT_CAMERA_DEVICE_ADDED";
    case SDL_EVENT_CAMERA_DEVICE_REMOVED: return "SDL_EVENT_CAMERA_DEVICE_REMOVED";
    case SDL_EVENT_CAMERA_DEVICE_APPROVED: return "SDL_EVENT_CAMERA_DEVICE_APPROVED";
    case SDL_EVENT_CAMERA_DEVICE_DENIED: return "SDL_EVENT_CAMERA_DEVICE_DENIED";
    case SDL_EVENT_RENDER_TARGETS_RESET: return "SDL_EVENT_RENDER_TARGETS_RESET";
    case SDL_EVENT_RENDER_DEVICE_RESET: return "SDL_EVENT_RENDER_DEVICE_RESET";
    case SDL_EVENT_RENDER_DEVICE_LOST: return "SDL_EVENT_RENDER_DEVICE_LOST";
    case SDL_EVENT_POLL_SENTINEL: return "SDL_EVENT_POLL_SENTINEL";
    default: return "UNKNOWN";
    }
}
