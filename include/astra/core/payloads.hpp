#pragma once

#include "astra/core/hermes.hpp"

#include <spdlog/details/log_msg.h>

namespace astra {
struct PreUpdate {
    HERMES_TAG_MEMBER(astra::PreUpdate);
    double dt;
};

struct Update {
    HERMES_TAG_MEMBER(astra::Update);
    double dt;
};

struct PostUpdate {
    HERMES_TAG_MEMBER(astra::PostUpdate);
    double dt;
};

struct PreDraw {
    HERMES_TAG_MEMBER(astra::PreDraw);
};

struct Draw {
    HERMES_TAG_MEMBER(astra::Draw);
};

struct PostDraw {
    HERMES_TAG_MEMBER(astra::PostDraw);
};

struct PreDrawOverlay {
    HERMES_TAG_MEMBER(astra::PreDrawOverlay);
};

struct DrawOverlay {
    HERMES_TAG_MEMBER(astra::DrawOverlay);
};

struct PostDrawOverlay {
    HERMES_TAG_MEMBER(astra::PostDrawOverlay);
};

struct LogMessage {
    HERMES_TAG_MEMBER(astra::LogMessage);
    spdlog::level::level_enum level;
    std::string text;
};
} // namespace astra
