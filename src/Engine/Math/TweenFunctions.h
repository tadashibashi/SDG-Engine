#pragma once
#ifndef SDGENGINE_TWEENFUNCTIONS_H
#define SDGENGINE_TWEENFUNCTIONS_H
#include <Engine/Core.h>
namespace SDG::TweenFunctions
    {
        float  Linear(float currentTime, float start, float relTarget, float duration);
        float  EaseInQuad(float currentTime, float start, float relTarget, float duration);
        float  EaseOutQuad(float currentTime, float start, float relTarget, float duration);
        float  EaseInOutQuad(float currentTime, float start, float relTarget, float duration);
        float  EaseInCubic(float currentTime, float start, float relTarget, float duration);
        float  EaseOutCubic(float currentTime, float start, float relTarget, float duration);
        float  EaseInOutCubic(float currentTime, float start, float relTarget, float duration);
    }

#endif
