// Author: Aaron Ishibashi
// 
//
#pragma once
#ifndef SDGENGINE_TWEEN_H
#define SDGENGINE_TWEEN_H
#include <Engine/Core.h>
#include <Engine/Math/TweenFunctions.h>
#include <Engine/Math/Math.h>
#include <functional>
#include <cmath>
#include <utility>

namespace SDG
{
    // Definition for TweenFunctions in namespace Tweens::functions
    typedef std::function<float(float, float, float, float)> TweenFunction;

    class SDG_API Tween
    {
    public:
        Tween(std::function<void(float)> setter, float start, float relVal,
                float duration, const TweenFunction& func);
        ~Tween() = default;

        Tween& OnFinish(std::function<void()> cb);
        Tween& OnStep(std::function<void(float)> cb);
        Tween& SetYoyo(bool setYoyo);

        [[nodiscard]] bool isYoyo() const {return isYoyo_;}

        /**
         * Set the currentTime to zero, and activate Tween
         */
        void Restart();
        void Restart(float startVal, float endVal, float duration,
                     TweenFunction func);
        void Start() { isActive_ = true;}
        void Stop(bool resetTween = true);
        void ChangeTargetSetter(const std::function<void(float)>& setter);

        /**
         * Stops the Tween and sets its currentTime to 0
         */
        void Reset();

        /**
         * Stops the Tween, sets its currentTime to 0, and modifies essential Tween parameters
         */
        void Reset(float startVal, float endVal, float duration,
                   TweenFunction& func);

        /**
         * Set the speed multiplier of the Tween (default: 1.f)
         * @param speed The value of speed to set
         */
        void SetSpeed(float speed) { speed_ = speed;}

        [[nodiscard]] float GetSpeed() const {return speed_;}

        /**
         * Update should be called every frame for accurate timing
         */
        void Update(float deltaTime); // end void evUpdate

    private:
        float currentTime_;
        float startVal_;
        float relVal_;
        float duration_;
        bool isActive_;
        bool isYoyo_;
        bool isReversing_;
        float speed_;

        // Callbacks

        // The Tween function to perform every call to evUpdate
        TweenFunction func_;

        // The function that alters the desired values
        std::function<void(float)> setter_;

        // The callback that sends when the Tween has reached the
        // destination
        std::function<void()> onFinish_;
        std::function<void(float)> onStep_;
    }; // end class Tween
}

#endif
