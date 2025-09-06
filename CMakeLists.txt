cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)
project(
        astra
        VERSION 0.0.1
        LANGUAGES C CXX
)

option(ASTRA_BUILD_EXAMPLES "Build astra examples" OFF)
option(ASTRA_BUILD_DEPENDENCIES "Build astra dependencies (à la FetchContent)" OFF)

add_library(astra)
add_library(astra::astra ALIAS astra)

target_sources(
        astra

        PRIVATE
        src/astra/core/log.cpp
        src/astra/util/platform.cpp
        src/astra/util/rng.cpp
        src/astra/util/time.cpp
        src/gloo/init.cpp
        src/gloo/gl.cpp
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
        include/astra/core/log.hpp
        include/astra/core/messenger.hpp
        include/astra/core/types.hpp
        include/astra/util/constexpr_hash.hpp
        include/astra/util/enum_class_helpers.hpp
        include/astra/util/is_any_of.hpp
        include/astra/util/platform.hpp
        include/astra/util/rng.hpp
        include/astra/util/time.hpp
        include/astra/astra.hpp
        include/gloo/init.hpp
        include/gloo/gl.hpp
        include/sdl3_raii/gl_attr.hpp
        include/sdl3_raii/gl_context_flags.hpp
        include/sdl3_raii/init.hpp
        include/sdl3_raii/window.hpp
)

target_compile_features(astra PRIVATE cxx_std_23)

add_subdirectory(thirdparty)
target_link_libraries(astra PUBLIC astra::astra_thirdparty)

if (ASTRA_BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()