/**
 * Author: Aaron Ishibashi
 * Date: 3/18/2020
 *
 */
#include "Tween.h"

#include <utility>

namespace SDG
{
    
    Tween::Tween(std::function<void(float)> setter, float start, float relVal,
                 float duration, TweenFunction func) :
                 currentTime_(0),
                 func_(std::move(func)),
                 startVal_(start),
                 relVal_(relVal),
                 duration_(duration),
                 setter_(std::move(setter)),
                 isActive_(false),
                 speed_(1.f),
                 isReversing_(false),
                 isYoyo_(false)
    {}

    // ===== Setters ==========================================================

    Tween&
    Tween::OnFinish(std::function<void()> cb)
    {
        onFinish_ = std::move(cb);
        return *this;
    }
    
    Tween&
    Tween::OnStep(std::function<void(float)> cb)
    {
        onStep_ = std::move(cb);
        return *this;
    }
    
    Tween&
    Tween::SetYoyo(bool setYoyo) {
        isYoyo_ = setYoyo;
        return *this;
    }

    void
    Tween::ChangeTarget(const std::function<void(float)> &setter)
    {
        setter_ = setter;
    }

    // ===== Playback Control =================================================

    void
    Tween::Restart()
    {
        isActive_ = true;
        currentTime_ = 0;
        isReversing_ = false;
    }
    
    void
    Tween::Restart(float startVal, float endVal, float duration, TweenFunction func)
    {
        startVal_ = startVal;
        relVal_ = endVal - startVal_;
        duration_ = duration;
        func_ = std::move(func);

        Restart();
    }
    
    void
    Tween::Stop(bool resetTween)
    {
        isActive_ = false;
        if (resetTween) {
            Reset();
        }
    }
    
    void
    Tween::Reset()
    {
        isActive_ = false;
        currentTime_ = 0;
        isReversing_ = false;
    }
    
    void
    Tween::Reset(float startVal, float endVal, float duration, TweenFunction func)
    {
        startVal_ = startVal;
        relVal_ = endVal - startVal_;
        duration_ = duration;
        func_ = std::move(func);

        Reset();
    }

    // ===== Runtime Impl ==========================================================

    void
    Tween::Update(const float deltaTime)
    {
        if (!isActive_)
            return;
    
        // calculate the current value
        float val = func_(currentTime_, startVal_, relVal_,
                          duration_);
        val = Math::Clamp<float>(val, startVal_, startVal_ + relVal_);
    
        // apply value to the target (if there is one)
        if (setter_) setter_(val);
    
        // fire step callback
        if (onStep_) onStep_(val);
    
        // update time counter
        currentTime_ += deltaTime * speed_ *
                        ((isReversing_) ? -1.f : 1.f);
    
        // time-based checks
        if (currentTime_ >= duration_)
        {
            currentTime_ = duration_;
            if (isYoyo_ && !isReversing_)
            {
                isReversing_ = true;
            }
            else
            {
                isActive_ = false;
                if (onFinish_) onFinish_();
            }
        }
        else if (currentTime_ <= 0)
        {
            currentTime_ = 0;
            if (isYoyo_ && isReversing_)
            {
                isReversing_ = false;
                isActive_ = false;
                if (onFinish_) onFinish_();
            }
        }
    }
}
