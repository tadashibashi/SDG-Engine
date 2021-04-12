/* =============================================================================
 * Graphics Library
 * Get the graphics library includes for native platforms
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>

#if   defined(SDG_PLATFORM_EMSCRIPTEN) || defined(SDG_PLATFORM_ANDROID)
    // HTML5
    #include <GLES2/GL2.h>
#elif defined(SDG_PLATFORM_APPLE)
    #include <GL/glew.h> // TODO: Metal includes

    #if   defined(SDG_PLATFORM_IPHONE)
    // iPhone/Simulator

    #elif defined(SDG_PLATFORM_MAC)
    // MacOS

    #endif
#elif defined(SDG_PLATFORM_WINDOWS)
    #include <GL/glew.h>
    #if SDG_PLATFORM_WINDOWS_ARCH == 64
    // Windows MSVC 64-bit

    #else
    // Windows MSVC 32-bit

    #endif
#endif
