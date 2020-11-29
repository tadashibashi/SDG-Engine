/* =============================================================================
 * Core
 * 
 * 
 * ===========================================================================*/
#pragma once

#ifdef SDG_PLATFORM_WINDOWS
    #ifdef SDG_BUILD_DLL
        #define SDG_API
    #else
        #define SDG_API
    #endif
#elif defined(__EMSCRIPTEN__)
    #define SDG_PLATFORM_HTML5
    #define SDG_API
#else
    #error SDG Engine: Platform not supported!
#endif