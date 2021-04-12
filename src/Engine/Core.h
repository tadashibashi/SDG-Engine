/* =============================================================================
 * Core
 *     Detects the platform based on compiler defines
 * ===========================================================================*/
#pragma once

// Find the platform via compiler defines
#if   defined(_MSC_VER)
    #define SDG_PLATFORM_WINDOWS
    #if _WIN64
    // Windows MSVC 64-bit
    #define SDG_PLATFORM_WINDOWS_ARCH 64
    #else
    // Windows MSVC 32-bit
    #define SDG_PLATFORM_WINDOWS_ARCH 32
    #endif

#elif defined(__APPLE__) // Apple always 64-bit since 32-bit has been deprecated
    #define SDG_PLATFORM_APPLE

    #if   defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
        // iPhone/Simulator
        #define SDG_PLATFORM_IPHONE
    #elif defined(TARGET_OS_MAC)
        // MacOS
        #define SDG_PLATFORM_MAC
    #endif
#elif defined(__linux__)
    #define SDG_PLATFORM_LINUX
#elif defined(__ANDROID__)
    #define SDG_PLATFORM_ANDROID
#elif defined(__EMSCRIPTEN__)
    #include <emscripten.h>
    #define SDG_PLATFORM_EMSCRIPTEN
#else
    #error SDG Engine: Platform not supported!
#endif

namespace SDG
{
    typedef unsigned char      ubyte;
    typedef unsigned short     ushort;
    typedef unsigned int       uint;
    typedef unsigned long      ulong;
    typedef unsigned long long ullong;
    typedef char               byte;
}

