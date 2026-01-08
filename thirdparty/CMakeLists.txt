include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.2.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        sdl3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-3.4.0
        GIT_SHALLOW TRUE
)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
FetchContent_MakeAvailable(sdl3)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.3
        GIT_SHALLOW TRUE
)
set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        boost_multiprecision
        GIT_REPOSITORY https://github.com/boostorg/multiprecision
        GIT_TAG Boost_1_89_0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(boost_multiprecision)

FetchContent_Declare(
        glad2
        GIT_REPOSITORY https://github.com/Dav1dde/glad
        GIT_TAG v2.0.8
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glad2)
add_subdirectory(${glad2_SOURCE_DIR}/cmake ${glad2_BINARY_DIR})
if (WIN32)
    glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE API gl:core=4.6 wgl=1.0)
else ()
    glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE API gl:core=4.6 egl=1.5)
endif ()

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        implot
        GIT_REPOSITORY https://github.com/epezent/implot
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(implot)

FetchContent_Declare(
        pcg
        GIT_REPOSITORY https://github.com/imneme/pcg-cpp
        GIT_TAG 428802d1a5634f96bcd0705fab379ff0113bcf13
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(pcg)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions
        GIT_TAG v3.10.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ctre)

add_library(astra_thirdparty)
add_library(astra::astra_thirdparty ALIAS astra_thirdparty)

target_sources(
        astra_thirdparty

        PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${implot_SOURCE_DIR}/implot.cpp
        ${implot_SOURCE_DIR}/implot_demo.cpp
        ${implot_SOURCE_DIR}/implot_items.cpp

        PUBLIC
        FILE_SET imgui_headers
        TYPE HEADERS
        BASE_DIRS
        ${imgui_SOURCE_DIR}
        FILES
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h

        PUBLIC
        FILE_SET imgui_backend_headers
        TYPE HEADERS
        BASE_DIRS
        ${imgui_SOURCE_DIR}/backends
        FILES
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h

        PUBLIC
        FILE_SET implot_headers
        TYPE HEADERS
        BASE_DIRS
        ${implot_SOURCE_DIR}
        FILES
        ${implot_SOURCE_DIR}/implot.h
        ${implot_SOURCE_DIR}/implot_internal.h

        PUBLIC FILE_SET pcg_headers
        TYPE HEADERS
        BASE_DIRS
        ${pcg_SOURCE_DIR}/include
        FILES
        ${pcg_SOURCE_DIR}/include/pcg_extras.hpp
        ${pcg_SOURCE_DIR}/include/pcg_random.hpp
        ${pcg_SOURCE_DIR}/include/pcg_uint128.hpp

        PUBLIC FILE_SET stb_headers
        TYPE HEADERS
        BASE_DIRS
        ${stb_SOURCE_DIR}
        FILES
        ${stb_SOURCE_DIR}/stb_image.h
)

target_compile_definitions(
        astra_thirdparty

        PUBLIC
        IMGUI_USER_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/imconfig.hpp"
)


target_link_libraries(
        astra_thirdparty

        PUBLIC
        fmt::fmt
        glm::glm
        glad_gl_core_46
        SDL3::SDL3
        spdlog::spdlog
        ctre::ctre
        Boost::multiprecision
)
