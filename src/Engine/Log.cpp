/* =============================================================================
 * Log
 * 
 * 
 * ===========================================================================*/
#if defined(_DEBUG)
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
namespace SDG
{
    spdlog::logger *Log::s_clientLogger;
    spdlog::logger *Log::s_coreLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_coreLogger = spdlog::stdout_color_mt("SDG").get();

        s_coreLogger->set_level(spdlog::level::trace);
        s_clientLogger = spdlog::stdout_color_mt("APP").get();
        s_clientLogger->set_level(spdlog::level::trace);
    }
}
#endif
