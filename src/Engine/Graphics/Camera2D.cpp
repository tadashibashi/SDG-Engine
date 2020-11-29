#include <Engine/Math/Vector2.h>
#include "Camera2D.h"
#include <cmath>
namespace SDG
{
    void Camera2D::updateMatrix() const
    {
        if (altered_)
        {
            Vector2 tpos = position_;
            tpos *= scale_;
            tpos.x = std::round(tpos.x);
            tpos.y = std::round(tpos.y);
            tpos /= scale_;

            // Translation
            glm::vec3 translate(-tpos.x + (float)screenWidth_ / 2,
                -tpos.y + (float)screenHeight_ / 2, 0.f);
            camMatrix_ = glm::translate(orthoMatrix_, translate);

            // Scale
            glm::vec3 scale(scale_.x, scale_.y, 0.0f);
            camMatrix_ = glm::scale(glm::mat4(1.0f), scale) * camMatrix_;

            auto pos = ScreenToWorld(position_);
            worldBounds_ = FRectangle(pos.x, pos.y,
                              (float)screenWidth_/scale_.x, (float)screenHeight_/scale_.y);

            altered_ = false;
        }
    }

    void Camera2D::SetDimensions(int screen_width, int screen_height)
    {
        screenWidth_ = screen_width;
        screenHeight_ = screen_height;
        orthoMatrix_ = glm::ortho(0.f, (float)screenWidth_, 0.f,
                                  (float)screenHeight_);
    }

    Vector2 Camera2D::ScreenToWorld(Vector2 screen) const
    {
        screen -= Vector2((float)screenWidth_ * .5f, (float)screenHeight_ * .5f);
        screen /= GetScale();
        screen += Vector2(GetPosition().x, GetPosition().y);

        return Vector2(screen);
    }

    Camera2D::Camera2D(GraphicsDeviceMgr &device) :
            position_{}, scale_(1.f, 1.f), camMatrix_(1.f),
            altered_(true), screenWidth_(0), screenHeight_(0),
            orthoMatrix_(1.f), graphicsMgr_(device)
    {
        Point windowSize = device.GetCurrentDevice().GetBackBufferSize();
        SDG_CORE_LOG("Camera setting dimensions: {0}, {1}", windowSize.x, windowSize.y);
        SetDimensions(windowSize.w, windowSize.h);
        SetPosition((windowSize.w/2.f), (windowSize.h/2.f));
    }


}
