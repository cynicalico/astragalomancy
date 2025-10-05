#include "astra/util/module/dear.hpp"

#include "astra/core/log.hpp"
#include "astra/core/payloads.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "sdl3_raii/events/keyboard.hpp"
#include "sdl3_raii/events/mouse.hpp"
#include "sdl3_raii/events/raw.hpp"

astra::Dear::Dear(Messenger *messenger, sdl3::Window *window)
    : messenger_(messenger) {
    IMGUI_CHECKVERSION();

    imgui_ctx_ = ImGui::CreateContext();
    ASTRA_LOG_DEBUG("ImGui v{}", IMGUI_VERSION);

    implot_ctx_ = ImPlot::CreateContext();
    ASTRA_LOG_DEBUG("ImPlot v{}", IMPLOT_VERSION);

    (void)ImGui::GetIO();

    if (!ImGui_ImplSDL3_InitForOpenGL(window->handle(), window->gl_context()))
        ASTRA_LOG_ERROR("Failed to initialize ImGui/SDL3 backend");

    if (!ImGui_ImplOpenGL3_Init("#version 130"))
        ASTRA_LOG_ERROR("Failed to initialize ImGui/OpenGL backend");

    register_callbacks_();
}

astra::Dear::~Dear() {
    if (implot_ctx_)
        ImPlot::DestroyContext(implot_ctx_);

    if (imgui_ctx_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext(imgui_ctx_);
    }

    if (callback_id_)
        unregister_callbacks_();
}

astra::Dear::Dear(Dear &&other) noexcept
    : imgui_ctx_(other.imgui_ctx_),
      implot_ctx_(other.implot_ctx_) {
    other.imgui_ctx_ = nullptr;
    other.implot_ctx_ = nullptr;
    other.unregister_callbacks_();
    messenger_ = other.messenger_;
    register_callbacks_();
}

astra::Dear &astra::Dear::operator=(Dear &&other) noexcept {
    if (this != &other) {
        imgui_ctx_ = other.imgui_ctx_;
        implot_ctx_ = other.implot_ctx_;
        // This will get fixed at the next event if needed
        keyboard_captured_ = false;
        mouse_captured_ = false;

        other.imgui_ctx_ = nullptr;
        other.implot_ctx_ = nullptr;
        unregister_callbacks_();
        other.unregister_callbacks_();
        messenger_ = other.messenger_;
        register_callbacks_();
    }
    return *this;
}

void astra::Dear::register_callbacks_() {
    callback_id_ = messenger_->get_id();

    messenger_->subscribe<sdl3::RawEvent>(*callback_id_, [&](const auto *p) {
        ImGui_ImplSDL3_ProcessEvent(&p->e);

        const ImGuiIO &io = ImGui::GetIO();

        if (!keyboard_captured_ && io.WantCaptureKeyboard) {
            messenger_->capture<sdl3::KeyboardEvent>(*callback_id_);
            keyboard_captured_ = true;
        } else if (keyboard_captured_ && !io.WantCaptureKeyboard) {
            messenger_->uncapture<sdl3::KeyboardEvent>(*callback_id_);
            keyboard_captured_ = false;
        }

        if (!mouse_captured_ && io.WantCaptureMouse) {
            messenger_->capture<sdl3::MouseButtonEvent>(*callback_id_);
            messenger_->capture<sdl3::MouseMotionEvent>(*callback_id_);
            messenger_->capture<sdl3::MouseWheelEvent>(*callback_id_);
            mouse_captured_ = true;
        } else if (mouse_captured_ && !io.WantCaptureMouse) {
            messenger_->uncapture<sdl3::MouseButtonEvent>(*callback_id_);
            messenger_->uncapture<sdl3::MouseMotionEvent>(*callback_id_);
            messenger_->uncapture<sdl3::MouseWheelEvent>(*callback_id_);
            mouse_captured_ = false;
        }
    });

    messenger_->subscribe<PreDraw>(*callback_id_, [&](const auto *) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    });

    messenger_->subscribe<PostDraw>(*callback_id_, [&](const auto *) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    });
}

void astra::Dear::unregister_callbacks_() {
    messenger_->release_id(*callback_id_);
    callback_id_ = std::nullopt;
}
