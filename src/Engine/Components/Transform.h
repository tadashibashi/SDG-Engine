/* =============================================================================
 * Transform
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Math/Vector2.h>
#include "Component.h"
#include <Engine/Log.h>

namespace SDG
{
    class SDG_API Transform: public Component
    {
    public:
        Transform(float x, float y, float scaleX, float scaleY):
            Component(true, false),
            position(x, y), scale(scaleX, scaleY) { }

        // Local position. Use GetPosition() to get the final projected position.
        Vector2 position;
        Vector2 scale;

        // The system at the top should call update only on components with no parents.
        void PostUpdate() override
        {
            setProjectedPos_ = false;
        }

        Vector2 GetPosition() const
        {
            if (!setProjectedPos_)
                SetProjectedPosition();

            return projectedPos_;
        }

        // Attach to another Transform to have its position transposed by it.
        void AttachTransform(Transform *tf)
        {
            if (tf->parent_ == this)
                return;
            else if (tf->parent_)
                tf->DetachFromParent();

            tf->parent_ = this;
        }

        // Remove the Transform's parent. It's position will be updated to its
        // current projected position.
        void DetachFromParent()
        {
            if (parent_)
            {
                parent_ = nullptr;

                // Smooth release, won't cause transform to jump positions suddenly.
                position = projectedPos_;
                projectedPos_ = Vector2();
            }
        }

    private:
        // A flag marking whether or not final projected position based on
        // parent position has been calculated or not this frame.
        mutable bool setProjectedPos_{false};
        // Final projected position.
        mutable Vector2 projectedPos_;

        Transform *parent_{};

        // Calculates the projected position.
        void SetProjectedPosition() const
        {
            if (parent_)
            {
                if (!parent_->setProjectedPos_)
                    parent_->SetProjectedPosition();

                projectedPos_ = position + parent_->projectedPos_;
            }
            else
            {
                projectedPos_ = position;
            }

            setProjectedPos_ = true;
        }
    };
}