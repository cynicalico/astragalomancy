cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)
project(
        astra
        VERSION 0.0.1
        LANGUAGES C CXX
)

option(ASTRA_BUILD_EXAMPLES "Build astra examples" OFF)
option(ASTRA_BUILD_DEPENDENCIES "Build astra dependencies (à la FetchContent)" OFF)
option(ASTRA_SPDLOG_LOG_LEVEL "Log level for spdlog, see" NONE)

add_library(astra
        src/astra/core/engine.cpp)
add_library(astra::astra ALIAS astra)

target_sources(
        astra

        PRIVATE
        src/astra/core/color.cpp
        src/astra/core/engine.cpp
        src/astra/core/log.cpp
        src/astra/util/module/dear.cpp
        src/astra/util/module/timer_mgr.cpp
        src/astra/util/averagers.cpp
        src/astra/util/io.cpp
        src/astra/util/math.cpp
        src/astra/util/platform.cpp
        src/astra/util/rng.cpp
        src/astra/util/time.cpp
        src/gloo/init.cpp
        src/gloo/gl.cpp
        src/sdl3_raii/event_pump.cpp
        src/sdl3_raii/gl_attr.cpp
        src/sdl3_raii/gl_context_flags.cpp
        src/sdl3_raii/init.cpp
        src/sdl3_raii/window.cpp

        PUBLIC
        FILE_SET astra_headers
        TYPE HEADERS
        BASE_DIRS
        include
        FILES
        include/astra/core/color.hpp
        include/astra/core/engine.hpp
        include/astra/core/log.hpp
        include/astra/core/messenger.hpp
        include/astra/core/payloads.hpp
        include/astra/core/types.hpp
        include/astra/util/module/dear.hpp
        include/astra/util/module/timer_mgr.hpp
        include/astra/util/averagers.hpp
        include/astra/util/constexpr_hash.hpp
        include/astra/util/enum_class_helpers.hpp
        include/astra/util/io.hpp
        include/astra/util/is_any_of.hpp
        include/astra/util/math.hpp
        include/astra/util/platform.hpp
        include/astra/util/rng.hpp
        include/astra/util/time.hpp
        include/astra/astra.hpp
        include/gloo/init.hpp
        include/gloo/gloo.hpp
        include/gloo/gl.hpp
        include/sdl3_raii/events/audio.hpp
        include/sdl3_raii/events/camera.hpp
        include/sdl3_raii/events/clipboard.hpp
        include/sdl3_raii/events/display.hpp
        include/sdl3_raii/events/drop.hpp
        include/sdl3_raii/events/gamepad.hpp
        include/sdl3_raii/events/joystick.hpp
        include/sdl3_raii/events/keyboard.hpp
        include/sdl3_raii/events/mouse.hpp
        include/sdl3_raii/events/pen.hpp
        include/sdl3_raii/events/quit.hpp
        include/sdl3_raii/events/raw.hpp
        include/sdl3_raii/events/render.hpp
        include/sdl3_raii/events/sensor.hpp
        include/sdl3_raii/events/text.hpp
        include/sdl3_raii/events/touch.hpp
        include/sdl3_raii/events/user.hpp
        include/sdl3_raii/events/window.hpp
        include/sdl3_raii/event_pump.hpp
        include/sdl3_raii/gl_attr.hpp
        include/sdl3_raii/gl_context_flags.hpp
        include/sdl3_raii/init.hpp
        include/sdl3_raii/keycode.hpp
        include/sdl3_raii/scancode.hpp
        include/sdl3_raii/sdl3_raii.hpp
        include/sdl3_raii/window.hpp
)

target_compile_features(astra PRIVATE cxx_std_23)

if (NOT ${ASTRA_SPDLOG_LOG_LEVEL} STREQUAL "NONE")
    target_compile_definitions(astra PUBLIC SPDLOG_ACTIVE_LEVEL=${ASTRA_SPDLOG_LOG_LEVEL})
endif ()

if (MSVC)
    target_compile_definitions(astra PUBLIC /utf-8)
endif ()

add_subdirectory(thirdparty)
target_link_libraries(astra PUBLIC astra::astra_thirdparty)

if (ASTRA_BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()