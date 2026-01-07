#pragma once

#include "astra/core/messenger.hpp"

#include <spdlog/details/log_msg.h>

namespace astra {
struct PreUpdate {
    ASTRA_TAG_MEMBER(astra::PreUpdate);
    double dt;
};

struct Update {
    ASTRA_TAG_MEMBER(astra::Update);
    double dt;
};

struct PostUpdate {
    ASTRA_TAG_MEMBER(astra::PostUpdate);
    double dt;
};

struct PreDraw {
    ASTRA_TAG_MEMBER(astra::PreDraw);
};

struct Draw {
    ASTRA_TAG_MEMBER(astra::Draw);
};

struct PostDraw {
    ASTRA_TAG_MEMBER(astra::PostDraw);
};

struct PreDrawOverlay {
    ASTRA_TAG_MEMBER(astra::PreDrawOverlay);
};

struct DrawOverlay {
    ASTRA_TAG_MEMBER(astra::DrawOverlay);
};

struct PostDrawOverlay {
    ASTRA_TAG_MEMBER(astra::PostDrawOverlay);
};

struct LogMessage {
    ASTRA_TAG_MEMBER(astra::LogMessage);
    spdlog::level::level_enum level;
    std::string text;
};
} // namespace astra
