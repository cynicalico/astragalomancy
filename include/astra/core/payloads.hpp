#pragma once

#include "astra/util/constexpr_hash.hpp"

#include <spdlog/details/log_msg.h>

#define ASTRA_TAG_MEMBER(name) constexpr static std::uint32_t ASTRA_MESSENGER_TAG{astra::murmur_x86_32(#name, 0)};

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

struct LogMessage {
    ASTRA_TAG_MEMBER(astra::LogMessage);
    spdlog::level::level_enum level;
    std::string text;
};
} // namespace astra
