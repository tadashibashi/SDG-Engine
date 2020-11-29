#pragma once
#ifndef SDGENGINE_TWEENFUNCTIONS_H
#define SDGENGINE_TWEENFUNCTIONS_H
#include <Engine/Core.h>
namespace SDG::TweenFunctions
    {
        float SDG_API Linear(float currentTime, float start, float relTarget, float duration);
        float SDG_API EaseInQuad(float currentTime, float start, float relTarget, float duration);
        float SDG_API EaseOutQuad(float currentTime, float start, float relTarget, float duration);
        float SDG_API EaseInOutQuad(float currentTime, float start, float relTarget, float duration);
        float SDG_API EaseInCubic(float currentTime, float start, float relTarget, float duration);
        float SDG_API EaseOutCubic(float currentTime, float start, float relTarget, float duration);
        float SDG_API EaseInOutCubic(float currentTime, float start, float relTarget, float duration);
    }

#endif
