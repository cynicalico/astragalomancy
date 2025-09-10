#pragma once

#include <SDL3/SDL_scancode.h>

#include <fmt/format.h>

template<>
struct fmt::formatter<SDL_Scancode> : formatter<std::string_view> {
    auto format(const SDL_Scancode &e, format_context &ctx) const {
        switch (e) {
        case SDL_SCANCODE_UNKNOWN:
            return format_to(ctx.out(), "UNKNOWN");
        case SDL_SCANCODE_A:
            return format_to(ctx.out(), "A");
        case SDL_SCANCODE_B:
            return format_to(ctx.out(), "B");
        case SDL_SCANCODE_C:
            return format_to(ctx.out(), "C");
        case SDL_SCANCODE_D:
            return format_to(ctx.out(), "D");
        case SDL_SCANCODE_E:
            return format_to(ctx.out(), "E");
        case SDL_SCANCODE_F:
            return format_to(ctx.out(), "F");
        case SDL_SCANCODE_G:
            return format_to(ctx.out(), "G");
        case SDL_SCANCODE_H:
            return format_to(ctx.out(), "H");
        case SDL_SCANCODE_I:
            return format_to(ctx.out(), "I");
        case SDL_SCANCODE_J:
            return format_to(ctx.out(), "J");
        case SDL_SCANCODE_K:
            return format_to(ctx.out(), "K");
        case SDL_SCANCODE_L:
            return format_to(ctx.out(), "L");
        case SDL_SCANCODE_M:
            return format_to(ctx.out(), "M");
        case SDL_SCANCODE_N:
            return format_to(ctx.out(), "N");
        case SDL_SCANCODE_O:
            return format_to(ctx.out(), "O");
        case SDL_SCANCODE_P:
            return format_to(ctx.out(), "P");
        case SDL_SCANCODE_Q:
            return format_to(ctx.out(), "Q");
        case SDL_SCANCODE_R:
            return format_to(ctx.out(), "R");
        case SDL_SCANCODE_S:
            return format_to(ctx.out(), "S");
        case SDL_SCANCODE_T:
            return format_to(ctx.out(), "T");
        case SDL_SCANCODE_U:
            return format_to(ctx.out(), "U");
        case SDL_SCANCODE_V:
            return format_to(ctx.out(), "V");
        case SDL_SCANCODE_W:
            return format_to(ctx.out(), "W");
        case SDL_SCANCODE_X:
            return format_to(ctx.out(), "X");
        case SDL_SCANCODE_Y:
            return format_to(ctx.out(), "Y");
        case SDL_SCANCODE_Z:
            return format_to(ctx.out(), "Z");
        case SDL_SCANCODE_1:
            return format_to(ctx.out(), "1");
        case SDL_SCANCODE_2:
            return format_to(ctx.out(), "2");
        case SDL_SCANCODE_3:
            return format_to(ctx.out(), "3");
        case SDL_SCANCODE_4:
            return format_to(ctx.out(), "4");
        case SDL_SCANCODE_5:
            return format_to(ctx.out(), "5");
        case SDL_SCANCODE_6:
            return format_to(ctx.out(), "6");
        case SDL_SCANCODE_7:
            return format_to(ctx.out(), "7");
        case SDL_SCANCODE_8:
            return format_to(ctx.out(), "8");
        case SDL_SCANCODE_9:
            return format_to(ctx.out(), "9");
        case SDL_SCANCODE_0:
            return format_to(ctx.out(), "0");
        case SDL_SCANCODE_RETURN:
            return format_to(ctx.out(), "RETURN");
        case SDL_SCANCODE_ESCAPE:
            return format_to(ctx.out(), "ESCAPE");
        case SDL_SCANCODE_BACKSPACE:
            return format_to(ctx.out(), "BACKSPACE");
        case SDL_SCANCODE_TAB:
            return format_to(ctx.out(), "TAB");
        case SDL_SCANCODE_SPACE:
            return format_to(ctx.out(), "SPACE");
        case SDL_SCANCODE_MINUS:
            return format_to(ctx.out(), "MINUS");
        case SDL_SCANCODE_EQUALS:
            return format_to(ctx.out(), "EQUALS");
        case SDL_SCANCODE_LEFTBRACKET:
            return format_to(ctx.out(), "LEFTBRACKET");
        case SDL_SCANCODE_RIGHTBRACKET:
            return format_to(ctx.out(), "RIGHTBRACKET");
        case SDL_SCANCODE_BACKSLASH:
            return format_to(ctx.out(), "BACKSLASH");
        case SDL_SCANCODE_NONUSHASH:
            return format_to(ctx.out(), "NONUSHASH");
        case SDL_SCANCODE_SEMICOLON:
            return format_to(ctx.out(), "SEMICOLON");
        case SDL_SCANCODE_APOSTROPHE:
            return format_to(ctx.out(), "APOSTROPHE");
        case SDL_SCANCODE_GRAVE:
            return format_to(ctx.out(), "GRAVE");
        case SDL_SCANCODE_COMMA:
            return format_to(ctx.out(), "COMMA");
        case SDL_SCANCODE_PERIOD:
            return format_to(ctx.out(), "PERIOD");
        case SDL_SCANCODE_SLASH:
            return format_to(ctx.out(), "SLASH");
        case SDL_SCANCODE_CAPSLOCK:
            return format_to(ctx.out(), "CAPSLOCK");
        case SDL_SCANCODE_F1:
            return format_to(ctx.out(), "F1");
        case SDL_SCANCODE_F2:
            return format_to(ctx.out(), "F2");
        case SDL_SCANCODE_F3:
            return format_to(ctx.out(), "F3");
        case SDL_SCANCODE_F4:
            return format_to(ctx.out(), "F4");
        case SDL_SCANCODE_F5:
            return format_to(ctx.out(), "F5");
        case SDL_SCANCODE_F6:
            return format_to(ctx.out(), "F6");
        case SDL_SCANCODE_F7:
            return format_to(ctx.out(), "F7");
        case SDL_SCANCODE_F8:
            return format_to(ctx.out(), "F8");
        case SDL_SCANCODE_F9:
            return format_to(ctx.out(), "F9");
        case SDL_SCANCODE_F10:
            return format_to(ctx.out(), "F10");
        case SDL_SCANCODE_F11:
            return format_to(ctx.out(), "F11");
        case SDL_SCANCODE_F12:
            return format_to(ctx.out(), "F12");
        case SDL_SCANCODE_PRINTSCREEN:
            return format_to(ctx.out(), "PRINTSCREEN");
        case SDL_SCANCODE_SCROLLLOCK:
            return format_to(ctx.out(), "SCROLLLOCK");
        case SDL_SCANCODE_PAUSE:
            return format_to(ctx.out(), "PAUSE");
        case SDL_SCANCODE_INSERT:
            return format_to(ctx.out(), "INSERT");
        case SDL_SCANCODE_HOME:
            return format_to(ctx.out(), "HOME");
        case SDL_SCANCODE_PAGEUP:
            return format_to(ctx.out(), "PAGEUP");
        case SDL_SCANCODE_DELETE:
            return format_to(ctx.out(), "DELETE");
        case SDL_SCANCODE_END:
            return format_to(ctx.out(), "END");
        case SDL_SCANCODE_PAGEDOWN:
            return format_to(ctx.out(), "PAGEDOWN");
        case SDL_SCANCODE_RIGHT:
            return format_to(ctx.out(), "RIGHT");
        case SDL_SCANCODE_LEFT:
            return format_to(ctx.out(), "LEFT");
        case SDL_SCANCODE_DOWN:
            return format_to(ctx.out(), "DOWN");
        case SDL_SCANCODE_UP:
            return format_to(ctx.out(), "UP");
        case SDL_SCANCODE_NUMLOCKCLEAR:
            return format_to(ctx.out(), "NUMLOCKCLEAR");
        case SDL_SCANCODE_KP_DIVIDE:
            return format_to(ctx.out(), "KP_DIVIDE");
        case SDL_SCANCODE_KP_MULTIPLY:
            return format_to(ctx.out(), "KP_MULTIPLY");
        case SDL_SCANCODE_KP_MINUS:
            return format_to(ctx.out(), "KP_MINUS");
        case SDL_SCANCODE_KP_PLUS:
            return format_to(ctx.out(), "KP_PLUS");
        case SDL_SCANCODE_KP_ENTER:
            return format_to(ctx.out(), "KP_ENTER");
        case SDL_SCANCODE_KP_1:
            return format_to(ctx.out(), "KP_1");
        case SDL_SCANCODE_KP_2:
            return format_to(ctx.out(), "KP_2");
        case SDL_SCANCODE_KP_3:
            return format_to(ctx.out(), "KP_3");
        case SDL_SCANCODE_KP_4:
            return format_to(ctx.out(), "KP_4");
        case SDL_SCANCODE_KP_5:
            return format_to(ctx.out(), "KP_5");
        case SDL_SCANCODE_KP_6:
            return format_to(ctx.out(), "KP_6");
        case SDL_SCANCODE_KP_7:
            return format_to(ctx.out(), "KP_7");
        case SDL_SCANCODE_KP_8:
            return format_to(ctx.out(), "KP_8");
        case SDL_SCANCODE_KP_9:
            return format_to(ctx.out(), "KP_9");
        case SDL_SCANCODE_KP_0:
            return format_to(ctx.out(), "KP_0");
        case SDL_SCANCODE_KP_PERIOD:
            return format_to(ctx.out(), "KP_PERIOD");
        case SDL_SCANCODE_NONUSBACKSLASH:
            return format_to(ctx.out(), "NONUSBACKSLASH");
        case SDL_SCANCODE_APPLICATION:
            return format_to(ctx.out(), "APPLICATION");
        case SDL_SCANCODE_POWER:
            return format_to(ctx.out(), "POWER");
        case SDL_SCANCODE_KP_EQUALS:
            return format_to(ctx.out(), "KP_EQUALS");
        case SDL_SCANCODE_F13:
            return format_to(ctx.out(), "F13");
        case SDL_SCANCODE_F14:
            return format_to(ctx.out(), "F14");
        case SDL_SCANCODE_F15:
            return format_to(ctx.out(), "F15");
        case SDL_SCANCODE_F16:
            return format_to(ctx.out(), "F16");
        case SDL_SCANCODE_F17:
            return format_to(ctx.out(), "F17");
        case SDL_SCANCODE_F18:
            return format_to(ctx.out(), "F18");
        case SDL_SCANCODE_F19:
            return format_to(ctx.out(), "F19");
        case SDL_SCANCODE_F20:
            return format_to(ctx.out(), "F20");
        case SDL_SCANCODE_F21:
            return format_to(ctx.out(), "F21");
        case SDL_SCANCODE_F22:
            return format_to(ctx.out(), "F22");
        case SDL_SCANCODE_F23:
            return format_to(ctx.out(), "F23");
        case SDL_SCANCODE_F24:
            return format_to(ctx.out(), "F24");
        case SDL_SCANCODE_EXECUTE:
            return format_to(ctx.out(), "EXECUTE");
        case SDL_SCANCODE_HELP:
            return format_to(ctx.out(), "HELP");
        case SDL_SCANCODE_MENU:
            return format_to(ctx.out(), "MENU");
        case SDL_SCANCODE_SELECT:
            return format_to(ctx.out(), "SELECT");
        case SDL_SCANCODE_STOP:
            return format_to(ctx.out(), "STOP");
        case SDL_SCANCODE_AGAIN:
            return format_to(ctx.out(), "AGAIN");
        case SDL_SCANCODE_UNDO:
            return format_to(ctx.out(), "UNDO");
        case SDL_SCANCODE_CUT:
            return format_to(ctx.out(), "CUT");
        case SDL_SCANCODE_COPY:
            return format_to(ctx.out(), "COPY");
        case SDL_SCANCODE_PASTE:
            return format_to(ctx.out(), "PASTE");
        case SDL_SCANCODE_FIND:
            return format_to(ctx.out(), "FIND");
        case SDL_SCANCODE_MUTE:
            return format_to(ctx.out(), "MUTE");
        case SDL_SCANCODE_VOLUMEUP:
            return format_to(ctx.out(), "VOLUMEUP");
        case SDL_SCANCODE_VOLUMEDOWN:
            return format_to(ctx.out(), "VOLUMEDOWN");
        // case SDL_SCANCODE_LOCKINGCAPSLOCK:
        //     return format_to(ctx.out(), "LOCKINGCAPSLOCK");
        // case SDL_SCANCODE_LOCKINGNUMLOCK:
        //     return format_to(ctx.out(), "LOCKINGNUMLOCK");
        // case SDL_SCANCODE_LOCKINGSCROLLLOCK:
        //     return format_to(ctx.out(), "LOCKINGSCROLLLOCK");
        case SDL_SCANCODE_KP_COMMA:
            return format_to(ctx.out(), "KP_COMMA");
        case SDL_SCANCODE_KP_EQUALSAS400:
            return format_to(ctx.out(), "KP_EQUALSAS400");
        case SDL_SCANCODE_INTERNATIONAL1:
            return format_to(ctx.out(), "INTERNATIONAL1");
        case SDL_SCANCODE_INTERNATIONAL2:
            return format_to(ctx.out(), "INTERNATIONAL2");
        case SDL_SCANCODE_INTERNATIONAL3:
            return format_to(ctx.out(), "INTERNATIONAL3");
        case SDL_SCANCODE_INTERNATIONAL4:
            return format_to(ctx.out(), "INTERNATIONAL4");
        case SDL_SCANCODE_INTERNATIONAL5:
            return format_to(ctx.out(), "INTERNATIONAL5");
        case SDL_SCANCODE_INTERNATIONAL6:
            return format_to(ctx.out(), "INTERNATIONAL6");
        case SDL_SCANCODE_INTERNATIONAL7:
            return format_to(ctx.out(), "INTERNATIONAL7");
        case SDL_SCANCODE_INTERNATIONAL8:
            return format_to(ctx.out(), "INTERNATIONAL8");
        case SDL_SCANCODE_INTERNATIONAL9:
            return format_to(ctx.out(), "INTERNATIONAL9");
        case SDL_SCANCODE_LANG1:
            return format_to(ctx.out(), "LANG1");
        case SDL_SCANCODE_LANG2:
            return format_to(ctx.out(), "LANG2");
        case SDL_SCANCODE_LANG3:
            return format_to(ctx.out(), "LANG3");
        case SDL_SCANCODE_LANG4:
            return format_to(ctx.out(), "LANG4");
        case SDL_SCANCODE_LANG5:
            return format_to(ctx.out(), "LANG5");
        case SDL_SCANCODE_LANG6:
            return format_to(ctx.out(), "LANG6");
        case SDL_SCANCODE_LANG7:
            return format_to(ctx.out(), "LANG7");
        case SDL_SCANCODE_LANG8:
            return format_to(ctx.out(), "LANG8");
        case SDL_SCANCODE_LANG9:
            return format_to(ctx.out(), "LANG9");
        case SDL_SCANCODE_ALTERASE:
            return format_to(ctx.out(), "ALTERASE");
        case SDL_SCANCODE_SYSREQ:
            return format_to(ctx.out(), "SYSREQ");
        case SDL_SCANCODE_CANCEL:
            return format_to(ctx.out(), "CANCEL");
        case SDL_SCANCODE_CLEAR:
            return format_to(ctx.out(), "CLEAR");
        case SDL_SCANCODE_PRIOR:
            return format_to(ctx.out(), "PRIOR");
        case SDL_SCANCODE_RETURN2:
            return format_to(ctx.out(), "RETURN2");
        case SDL_SCANCODE_SEPARATOR:
            return format_to(ctx.out(), "SEPARATOR");
        case SDL_SCANCODE_OUT:
            return format_to(ctx.out(), "OUT");
        case SDL_SCANCODE_OPER:
            return format_to(ctx.out(), "OPER");
        case SDL_SCANCODE_CLEARAGAIN:
            return format_to(ctx.out(), "CLEARAGAIN");
        case SDL_SCANCODE_CRSEL:
            return format_to(ctx.out(), "CRSEL");
        case SDL_SCANCODE_EXSEL:
            return format_to(ctx.out(), "EXSEL");
        case SDL_SCANCODE_KP_00:
            return format_to(ctx.out(), "KP_00");
        case SDL_SCANCODE_KP_000:
            return format_to(ctx.out(), "KP_000");
        case SDL_SCANCODE_THOUSANDSSEPARATOR:
            return format_to(ctx.out(), "THOUSANDSSEPARATOR");
        case SDL_SCANCODE_DECIMALSEPARATOR:
            return format_to(ctx.out(), "DECIMALSEPARATOR");
        case SDL_SCANCODE_CURRENCYUNIT:
            return format_to(ctx.out(), "CURRENCYUNIT");
        case SDL_SCANCODE_CURRENCYSUBUNIT:
            return format_to(ctx.out(), "CURRENCYSUBUNIT");
        case SDL_SCANCODE_KP_LEFTPAREN:
            return format_to(ctx.out(), "KP_LEFTPAREN");
        case SDL_SCANCODE_KP_RIGHTPAREN:
            return format_to(ctx.out(), "KP_RIGHTPAREN");
        case SDL_SCANCODE_KP_LEFTBRACE:
            return format_to(ctx.out(), "KP_LEFTBRACE");
        case SDL_SCANCODE_KP_RIGHTBRACE:
            return format_to(ctx.out(), "KP_RIGHTBRACE");
        case SDL_SCANCODE_KP_TAB:
            return format_to(ctx.out(), "KP_TAB");
        case SDL_SCANCODE_KP_BACKSPACE:
            return format_to(ctx.out(), "KP_BACKSPACE");
        case SDL_SCANCODE_KP_A:
            return format_to(ctx.out(), "KP_A");
        case SDL_SCANCODE_KP_B:
            return format_to(ctx.out(), "KP_B");
        case SDL_SCANCODE_KP_C:
            return format_to(ctx.out(), "KP_C");
        case SDL_SCANCODE_KP_D:
            return format_to(ctx.out(), "KP_D");
        case SDL_SCANCODE_KP_E:
            return format_to(ctx.out(), "KP_E");
        case SDL_SCANCODE_KP_F:
            return format_to(ctx.out(), "KP_F");
        case SDL_SCANCODE_KP_XOR:
            return format_to(ctx.out(), "KP_XOR");
        case SDL_SCANCODE_KP_POWER:
            return format_to(ctx.out(), "KP_POWER");
        case SDL_SCANCODE_KP_PERCENT:
            return format_to(ctx.out(), "KP_PERCENT");
        case SDL_SCANCODE_KP_LESS:
            return format_to(ctx.out(), "KP_LESS");
        case SDL_SCANCODE_KP_GREATER:
            return format_to(ctx.out(), "KP_GREATER");
        case SDL_SCANCODE_KP_AMPERSAND:
            return format_to(ctx.out(), "KP_AMPERSAND");
        case SDL_SCANCODE_KP_DBLAMPERSAND:
            return format_to(ctx.out(), "KP_DBLAMPERSAND");
        case SDL_SCANCODE_KP_VERTICALBAR:
            return format_to(ctx.out(), "KP_VERTICALBAR");
        case SDL_SCANCODE_KP_DBLVERTICALBAR:
            return format_to(ctx.out(), "KP_DBLVERTICALBAR");
        case SDL_SCANCODE_KP_COLON:
            return format_to(ctx.out(), "KP_COLON");
        case SDL_SCANCODE_KP_HASH:
            return format_to(ctx.out(), "KP_HASH");
        case SDL_SCANCODE_KP_SPACE:
            return format_to(ctx.out(), "KP_SPACE");
        case SDL_SCANCODE_KP_AT:
            return format_to(ctx.out(), "KP_AT");
        case SDL_SCANCODE_KP_EXCLAM:
            return format_to(ctx.out(), "KP_EXCLAM");
        case SDL_SCANCODE_KP_MEMSTORE:
            return format_to(ctx.out(), "KP_MEMSTORE");
        case SDL_SCANCODE_KP_MEMRECALL:
            return format_to(ctx.out(), "KP_MEMRECALL");
        case SDL_SCANCODE_KP_MEMCLEAR:
            return format_to(ctx.out(), "KP_MEMCLEAR");
        case SDL_SCANCODE_KP_MEMADD:
            return format_to(ctx.out(), "KP_MEMADD");
        case SDL_SCANCODE_KP_MEMSUBTRACT:
            return format_to(ctx.out(), "KP_MEMSUBTRACT");
        case SDL_SCANCODE_KP_MEMMULTIPLY:
            return format_to(ctx.out(), "KP_MEMMULTIPLY");
        case SDL_SCANCODE_KP_MEMDIVIDE:
            return format_to(ctx.out(), "KP_MEMDIVIDE");
        case SDL_SCANCODE_KP_PLUSMINUS:
            return format_to(ctx.out(), "KP_PLUSMINUS");
        case SDL_SCANCODE_KP_CLEAR:
            return format_to(ctx.out(), "KP_CLEAR");
        case SDL_SCANCODE_KP_CLEARENTRY:
            return format_to(ctx.out(), "KP_CLEARENTRY");
        case SDL_SCANCODE_KP_BINARY:
            return format_to(ctx.out(), "KP_BINARY");
        case SDL_SCANCODE_KP_OCTAL:
            return format_to(ctx.out(), "KP_OCTAL");
        case SDL_SCANCODE_KP_DECIMAL:
            return format_to(ctx.out(), "KP_DECIMAL");
        case SDL_SCANCODE_KP_HEXADECIMAL:
            return format_to(ctx.out(), "KP_HEXADECIMAL");
        case SDL_SCANCODE_LCTRL:
            return format_to(ctx.out(), "LCTRL");
        case SDL_SCANCODE_LSHIFT:
            return format_to(ctx.out(), "LSHIFT");
        case SDL_SCANCODE_LALT:
            return format_to(ctx.out(), "LALT");
        case SDL_SCANCODE_LGUI:
            return format_to(ctx.out(), "LGUI");
        case SDL_SCANCODE_RCTRL:
            return format_to(ctx.out(), "RCTRL");
        case SDL_SCANCODE_RSHIFT:
            return format_to(ctx.out(), "RSHIFT");
        case SDL_SCANCODE_RALT:
            return format_to(ctx.out(), "RALT");
        case SDL_SCANCODE_RGUI:
            return format_to(ctx.out(), "RGUI");
        case SDL_SCANCODE_MODE:
            return format_to(ctx.out(), "MODE");
        case SDL_SCANCODE_SLEEP:
            return format_to(ctx.out(), "SLEEP");
        case SDL_SCANCODE_WAKE:
            return format_to(ctx.out(), "WAKE");
        case SDL_SCANCODE_CHANNEL_INCREMENT:
            return format_to(ctx.out(), "CHANNEL_INCREMENT");
        case SDL_SCANCODE_CHANNEL_DECREMENT:
            return format_to(ctx.out(), "CHANNEL_DECREMENT");
        case SDL_SCANCODE_MEDIA_PLAY:
            return format_to(ctx.out(), "MEDIA_PLAY");
        case SDL_SCANCODE_MEDIA_PAUSE:
            return format_to(ctx.out(), "MEDIA_PAUSE");
        case SDL_SCANCODE_MEDIA_RECORD:
            return format_to(ctx.out(), "MEDIA_RECORD");
        case SDL_SCANCODE_MEDIA_FAST_FORWARD:
            return format_to(ctx.out(), "MEDIA_FAST_FORWARD");
        case SDL_SCANCODE_MEDIA_REWIND:
            return format_to(ctx.out(), "MEDIA_REWIND");
        case SDL_SCANCODE_MEDIA_NEXT_TRACK:
            return format_to(ctx.out(), "MEDIA_NEXT_TRACK");
        case SDL_SCANCODE_MEDIA_PREVIOUS_TRACK:
            return format_to(ctx.out(), "MEDIA_PREVIOUS_TRACK");
        case SDL_SCANCODE_MEDIA_STOP:
            return format_to(ctx.out(), "MEDIA_STOP");
        case SDL_SCANCODE_MEDIA_EJECT:
            return format_to(ctx.out(), "MEDIA_EJECT");
        case SDL_SCANCODE_MEDIA_PLAY_PAUSE:
            return format_to(ctx.out(), "MEDIA_PLAY_PAUSE");
        case SDL_SCANCODE_MEDIA_SELECT:
            return format_to(ctx.out(), "MEDIA_SELECT");
        case SDL_SCANCODE_AC_NEW:
            return format_to(ctx.out(), "AC_NEW");
        case SDL_SCANCODE_AC_OPEN:
            return format_to(ctx.out(), "AC_OPEN");
        case SDL_SCANCODE_AC_CLOSE:
            return format_to(ctx.out(), "AC_CLOSE");
        case SDL_SCANCODE_AC_EXIT:
            return format_to(ctx.out(), "AC_EXIT");
        case SDL_SCANCODE_AC_SAVE:
            return format_to(ctx.out(), "AC_SAVE");
        case SDL_SCANCODE_AC_PRINT:
            return format_to(ctx.out(), "AC_PRINT");
        case SDL_SCANCODE_AC_PROPERTIES:
            return format_to(ctx.out(), "AC_PROPERTIES");
        case SDL_SCANCODE_AC_SEARCH:
            return format_to(ctx.out(), "AC_SEARCH");
        case SDL_SCANCODE_AC_HOME:
            return format_to(ctx.out(), "AC_HOME");
        case SDL_SCANCODE_AC_BACK:
            return format_to(ctx.out(), "AC_BACK");
        case SDL_SCANCODE_AC_FORWARD:
            return format_to(ctx.out(), "AC_FORWARD");
        case SDL_SCANCODE_AC_STOP:
            return format_to(ctx.out(), "AC_STOP");
        case SDL_SCANCODE_AC_REFRESH:
            return format_to(ctx.out(), "AC_REFRESH");
        case SDL_SCANCODE_AC_BOOKMARKS:
            return format_to(ctx.out(), "AC_BOOKMARKS");
        case SDL_SCANCODE_SOFTLEFT:
            return format_to(ctx.out(), "SOFTLEFT");
        case SDL_SCANCODE_SOFTRIGHT:
            return format_to(ctx.out(), "SOFTRIGHT");
        case SDL_SCANCODE_CALL:
            return format_to(ctx.out(), "CALL");
        case SDL_SCANCODE_ENDCALL:
            return format_to(ctx.out(), "ENDCALL");
        case SDL_SCANCODE_RESERVED:
            return format_to(ctx.out(), "RESERVED");
        default:
            return format_to(ctx.out(), "???");
        }
    }
};
