#pragma once
#include "../Core.h"
#include <Engine/Math/Vector2.h>
#include <Engine/Log.h>
#include <Engine/Math/Rectangle.h>
#include "GraphicsDeviceMgr.h"
#include <glm/gtc/matrix_transform.hpp>

namespace SDG
{
    // An object that represents the player's view into the world.
    // Make sure to set altered flag to true when modifying SetPosition or Scale.
    class  Camera2D {
    public:
        explicit Camera2D(GraphicsDeviceMgr &device);

        // ====================================================================
        // Set / Get
        // ====================================================================

        void SetDimensions(int screen_width, int screen_height);

        Vector2 GetDimensions() const
        {
            return Vector2((float)screenWidth_, (float)screenHeight_);
        }

        FRectangle GetWorldBounds() const
        {
            updateMatrix();
            return worldBounds_;
        }

        void SetPosition(const Vector2& position)
        {
            position_ = Vector2(position.x, -position.y);
            altered_ = true;
        }

        void SetPosition(float x, float y)
        {
            position_.x = x;
            position_.y = -y;
            altered_ = true;
        }

        Vector2 GetPosition() const
        {
            return Vector2(position_.x, -position_.y);
        }

        void SetScale(const Vector2 &scale)
        {
            scale_ = scale;
            altered_ = true;
        }

        void SetScale(float new_scale)
        {
            scale_ = Vector2(new_scale, new_scale);
            altered_ = true;
        }

        Vector2 GetScale() const
        {
            return scale_;
        }

        glm::mat4 GetMatrix() const
        {
            updateMatrix();
            return camMatrix_;
        }

        Vector2 ScreenToWorld(Vector2 screen) const;

    private:
        // Updates the internal matrix based on altered Scale and SetPosition.
        void updateMatrix() const;

        int screenWidth_, screenHeight_;
        Vector2 scale_;
        Vector2 position_;
        
        glm::mat4 orthoMatrix_;
        mutable bool altered_;
        mutable glm::mat4 camMatrix_;
        mutable FRectangle worldBounds_;

        GraphicsDeviceMgr &graphicsMgr_;
    };
}


