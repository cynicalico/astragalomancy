#pragma once

#include "astra/core/messenger.hpp"
#include "sdl3_raii/window.hpp"

#include "imgui.h"
#include "implot.h"

#include <optional>

namespace astra {
class Dear {
public:
    explicit Dear(Messenger *messenger, sdl3::Window *window);
    ~Dear();

    Dear(const Dear &other) = delete;
    Dear &operator=(const Dear &other) = delete;

    Dear(Dear &&other) noexcept;
    Dear &operator=(Dear &&other) noexcept;

private:
    ImGuiContext *imgui_ctx_;
    ImPlotContext *implot_ctx_;
    bool keyboard_captured_{false};
    bool mouse_captured_{false};

    Messenger *messenger_;
    std::optional<Messenger::ID> callback_id_;
    void register_callbacks_();
    void unregister_callbacks_();
};
} // namespace astra
