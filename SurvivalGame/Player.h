/* =============================================================================
 * Player
 * 
 * 
 * ===========================================================================*/
#pragma once

#include <Engine/Components/EntityComponent.h>
#include <Engine/GraphicsLibrary.h>
#include <Engine/Math/Math.h>
#include <Engine/Math/Tween.h>
#include <Engine/Math/Rand.h>
#include "Bullet.h"

const int BULLET_TIMER_INIT_MAX = 250;

using namespace SDG;

class Player: public EntityComponent
{
public:
    Player(): EntityComponent(true, true),
              tf(nullptr), spr(nullptr), tween(nullptr)
    {

    }

    ~Player() override
    {
        delete tween;
    }

    void Init() override
    {
        tween = new Tween([this](float val) { tf->scale = Vector2(val * Math::Sign(tf->scale.x), val); }, 1.f, 5.f, 1.f,
                          TweenFunctions::Linear);
        tween->SetYoyo(true);
        tf = GetComponent<Transform>();
        spr = GetComponent<SpriteRenderer>();
        auto size = GetGraphicsDeviceMgr()->GetCurrentDevice().GetBackBufferSize();
        tf->position = Vector2(size.w/2, size.h/2);

        GetComponent<Collider2D>()->SetCallback(OnCollide);
    }

    static void OnCollide(Entity *thiz, Entity *other)
    {
        if (thiz->GetTag() == "Player" && other->GetTag() == "Zombie")
        {
            SDG_LOG("You collided with a Zombie!");
            GetScene()->DestroyEntity(*other);
        }
    }

    void Update() override
    {
        if (GetInput()->GetKeyboard()->KeyPressed(Key::A))
        {
            tween->Start();
        }

        if (GetInput()->GetKeyboard()->IsKeyDown(Key::E))
        {
            if (bulletTimer_ == 0)
            {
                auto &e = GetScene()->CreateEntity(MakeBullet);
                e.Components()->Get<Body>()->velocity = axis.Normalize() * 8.f;
                e.Components()->Get<Transform>()->position = GetComponent<Transform>()->GetPosition() + Vector2(-2, -10.f);

                bulletTimer_ = bulletTimerMax_;
            }
        }

        if (bulletTimer_ > 0)
        {
            bulletTimer_ -= (int)GetTime()->DeltaTicks();
        }
        else
        {
            bulletTimer_ = 0;
        }

        // Set window title to mouse position
        auto mousePos = GetInput()->GetMouse()->GetPosition();
        mousePos = GetScene()->GetCamera()->ScreenToWorld(mousePos);

        GetGraphicsDeviceMgr()->GetCurrentDevice().SetWindowTitle(
                (std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y)).c_str());

        tween->Update((float)GetTime()->DeltaTicks() * 0.001f);
        auto body = GetComponent<Body>();
        float speed = 5.f;
        auto input = GetInput();
        auto keys = input->GetKeyboard();

        int xAxis = (keys->IsKeyDown(Key::Right)) - (keys->IsKeyDown(Key::Left));
        int yAxis = (keys->IsKeyDown(Key::Down)) - (keys->IsKeyDown(Key::Up));


        body->velocity = Math::Lerp(body->velocity, Vector2(xAxis, yAxis).Normalize() * speed, .2f);
        body->velocity = Math::Lerp(body->velocity, Vector2(), .025f);

        float scaleMag = Math::Abs(tf->scale.x);
        if (xAxis < 0)
            tf->scale.x = -scaleMag;
        else if (xAxis > 0)
            tf->scale.x = scaleMag;
        else if (yAxis == 0)
        {
            spr->imageIndex = 0;
        }

        spr->imageSpeed = (std::abs(xAxis) > 0 || std::abs(yAxis) > 0) ? body->velocity.Length() : 0;
        //spr->rotation = std::fmod(spr->rotation + 1, 360);
        Vector2 tempAxis = Vector2(xAxis, yAxis);
        if (tempAxis.Length() > 0)
            axis = tempAxis;
    }

    void Draw() override
    {
//        auto sb = GetSpriteBatch();
//
//        sb->DrawRectangle(FRectangle(0, 0, 100, 100),
//        Color(255, 255, 127, 255), 0);
    }

private:
    int bulletTimer_ = 0;
    int bulletTimerMax_ = BULLET_TIMER_INIT_MAX;
    Transform *tf;
    SpriteRenderer *spr;
    Tween *tween;
    Vector2 axis = Vector2(1.f, 0);
};

// Takes a pre-existing Entity and adds the component makeup of the Player
void CreatePlayer(Entity &e)
{
    e.Components()->Add<SDG::Transform>(0, 0, 1.f, 1.f);
    auto &sprRenderer = e.Components()->Add<SDG::SpriteRenderer>();
    sprRenderer.SetSpriteByKey("guy-idle");
    auto &body = e.Components()->Add<Body>();
    body.show = true;
    e.Components()->Add<Collider2D>();
    e.Components()->Add<Player>();

    e.SetTag("Player");
}
