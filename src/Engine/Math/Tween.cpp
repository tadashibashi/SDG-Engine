/**
 * Author: Aaron Ishibashi
 * Date: 3/18/2020
 *
 */
#include "Tween.h"

namespace SDG
{
    
    Tween::Tween(std::function<void(float)> setter, float start, float relVal,
                 float duration, const TweenFunction &func)
    {
        currentTime_ = 0;
        func_ = func;
        startVal_ = start;
        relVal_ = relVal;
        duration_ = duration;
        setter_ = std::move(setter);
        isActive_ = false;
        speed_ = 1.f;
        isReversing_ = false;
        isYoyo_ = false;
    }
    
    Tween& Tween::OnFinish(std::function<void()> cb)
    {
        onFinish_ = std::move(cb);
        return *this;
    }
    
    Tween& Tween::OnStep(std::function<void(float)> cb)
    {
        onStep_ = std::move(cb);
        return *this;
    }
    
    Tween& Tween::SetYoyo(bool setYoyo) {
        isYoyo_ = setYoyo;
        return *this;
    }
    
    void Tween::Restart() {
        isActive_ = true;
        currentTime_ = 0;
        isReversing_ = false;
    }
    
    void Tween::Restart(float startVal, float endVal, float duration,
                        TweenFunction func) {
        startVal_ = startVal;
        relVal_ = endVal - startVal_;
        duration_ = duration;
        func_ = std::move(func);

        Restart();
    }
    
    void Tween::Stop(bool resetTween) {
        isActive_ = false;
        if (resetTween) {
            Reset();
        }
    }
    
    void Tween::ChangeTargetSetter(const std::function<void(float)> &setter)
    {
        setter_ = setter;
    }
    
    void Tween::Reset()
    {
        isActive_ = false;
        currentTime_ = 0;
        isReversing_ = false;
    }
    
    void Tween::Reset(float startVal, float endVal, float duration,
                      TweenFunction &func)
    {
        startVal_ = startVal;
        relVal_ = endVal - startVal_;
        duration_ = duration;
        func_ = func;

        Reset();
    }
    
    void Tween::Update(const float deltaTime)
    {
        if (!isActive_)
            return;
    
        // get the current value
        float val = func_(currentTime_, startVal_, relVal_,
                          duration_);
    
        // clamp val
        val = Math::Clamp<float>(val, startVal_, startVal_ + relVal_);
    
        // apply the values to the target (if there is one)
        if (setter_) {
            setter_(val);
        }
    
        // send step callback (if there is one)
        if (onStep_) {
            onStep_(val);
        }
    
        // add time
        currentTime_ += deltaTime * speed_ *
                        ((isReversing_) ? -1.f : 1.f);
    
        // clamp time
        currentTime_ = Math::Clamp<float>(currentTime_, 0.f, duration_);
    
        // checks
        if (currentTime_ >= duration_) {
            if (isYoyo_ && !isReversing_) {
                isReversing_ = true;
            } else {
                isActive_ = false;
                if (onFinish_) {
                    onFinish_();
                }
            }
        }
    
        if (currentTime_ <= 0) {
            if (isYoyo_ && isReversing_) {
                isReversing_ = false;
                isActive_ = false;
                if (onFinish_) {
                    onFinish_();
                }
            }
        }
    
    }
}
