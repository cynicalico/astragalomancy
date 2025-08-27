#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define ASTRA_PLATFORM_WINDOWS
#if defined(WIN64) || defined(_WIN64)
#define ASTRA_PLATFORM_WINDOWS64
#else
#define ASTRA_PLATFORM_WINDOWS32
#endif
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define ASTRA_PLATFORM_APPLE_IOS_SIMULATOR
#elif TARGET_OS_MACCATALYST
#define ASTRA_PLATFORM_APPLE_CATALYST
#elif TARGET_OS_IPHONE
#define ASTRA_PLATFORM_APPLE_IOS
#elif TARGET_OS_MAC
#define ASTRA_PLATFORM_APPLE
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
#define ASTRA_PLATFORM_LINUX
#elif __unix__
#define ASTRA_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
#define ASTRA_PLATFORM_POSIX
#else
#error "Unknown compiler"
#endif

namespace astra {
void log_platform();
} // namespace astra
