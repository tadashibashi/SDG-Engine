/* =============================================================================
 * Player
 * 
 * 
 * ===========================================================================*/
#pragma once

#include <Engine/Components/EntityComponent.h>
#include <Engine/GL.h>
#include <Engine/Math/Math.h>
#include <Engine/Math/Tween.h>
#include <Engine/Math/Rand.h>

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
        if (other->GetTag() == "Zombie")
        {
            SDG_LOG("You collided with a Zombie!");
            GetCurrentScene()->DestroyEntity(*other);
        }
    }

    void Update() override
    {
        if (GetInput()->GetKeyboard()->KeyPressed(Key::A))
        {
            tween->Start();
        }

        auto mousePos = GetInput()->GetMouse()->GetPosition();
        mousePos = GetCurrentScene()->GetCamera()->ScreenToWorld(mousePos);

        GetGraphicsDeviceMgr()->GetCurrentDevice().SetWindowTitle(
                (std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y)).c_str());

        tween->Update(GetTime()->DeltaTicks() * 0.001f);
        auto body = GetComponent<Body>();
        float speed = 5.f;
        auto time = GetTime();
        auto input = GetInput();
        auto keys = input->GetKeyboard();

        int xAxis = (keys->IsKeyDown(Key::Right)) - (keys->IsKeyDown(Key::Left));
        int yAxis = (keys->IsKeyDown(Key::Down)) - (keys->IsKeyDown(Key::Up));

        body->speed = 1.5f;
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
    }

    void Draw() override
    {
//        auto sb = GetSpriteBatch();
//
//        sb->DrawRectangle(FRectangle(0, 0, 100, 100),
//        Color(255, 255, 127, 255), 0);
    }

private:
    Transform *tf;
    SpriteRenderer *spr;
    Tween *tween;
};