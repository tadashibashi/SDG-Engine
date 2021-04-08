/* =============================================================================
 * EventListener
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>

namespace SDG
{
    template <typename... Args>
    class EventListener {
    public:
        virtual ~EventListener() = default;
        virtual void Callback(Args... args) = 0;
    };

}



