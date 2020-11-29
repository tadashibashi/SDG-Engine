/* =============================================================================
 * Audio
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>

namespace SDG
{
    class SDG_API Audio {
    public:
        Audio();
        ~Audio();
        virtual void PlayEvent(const std::string &id) = 0;
        virtual void StopAllSounds() = 0;
    private:

    };
}
