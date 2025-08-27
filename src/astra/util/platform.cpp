#include "astra/util/platform.hpp"

#include "astra/core/log.hpp"

void astra::log_platform() {
    std::string platforms;

#define CHECK_ADD_PLATFORM(n) platforms += platforms.empty() ? #n : ", " #n
#if defined(ASTRA_PLATFORM_WINDOWS)
    CHECK_ADD_PLATFORM(Windows);
#endif
#if defined(ASTRA_PLATFORM_WINDOWS32)
    CHECK_ADD_PLATFORM(Windows x86);
#endif
#if defined(ASTRA_PLATFORM_WINDOWS64)
    CHECK_ADD_PLATFORM(Windows x64);
#endif
#if defined(ASTRA_PLATFORM_APPLE_IOS_SIMULATOR)
    CHECK_ADD_PLATFORM(iOS Simulator);
#endif
#if defined(ASTRA_PLATFORM_APPLE_CATALYST)
    CHECK_ADD_PLATFORM(Catalyst);
#endif
#if defined(ASTRA_PLATFORM_APPLE_IOS)
    CHECK_ADD_PLATFORM(iOS);
#endif
#if defined(ASTRA_PLATFORM_APPLE)
    CHECK_ADD_PLATFORM(Apple);
#endif
#if defined(ASTRA_PLATFORM_LINUX)
    CHECK_ADD_PLATFORM(Linux);
#endif
#if defined(ASTRA_PLATFORM_UNIX)
    CHECK_ADD_PLATFORM(Unix);
#endif
#if defined(ASTRA_PLATFORM_POSIX)
    CHECK_ADD_PLATFORM(POSIX);
#endif
#undef CHECK_ADD_PLATFORM

    ASTRA_LOG_DEBUG("Platform(s) detected: {}", platforms.empty() ? "none" : platforms);
}
