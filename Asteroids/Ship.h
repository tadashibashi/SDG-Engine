/* =============================================================================
 * Player
 *
 *
 * ===========================================================================*/
#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <Engine/Components/EntityComponent.h>
#include <Engine/GraphicsLibrary.h>
#include <Engine/Math/Math.h>
#include <Engine/Math/Tween.h>
#include <Engine/Math/Rand.h>
#include "Scenes/GameScene.h"
#include <Engine/Events/Delegate.h>
#include <Engine/States/StateMachine.h>
#include <iostream>
#include <Engine/Scenes/SceneMgr.h>

const int BULLET_TIMER_INIT_MAX = 250;
const int INIT_LIVES = 3;

using namespace SDG;

class GameScene;

class GameInfo
{
public:
    GameInfo() : lives(INIT_LIVES), score(0) {}

    // Subtracts lives by one, and returns the decremented number of lives
    int LoseLife()
    {
       return --lives;
    }

    [[nodiscard]]
    bool HasLives() const { return this->lives > 0; }

    [[nodiscard]]
    int GetLives() const { return this->lives; }
    void SetLives(int lives) { this->lives = lives; }

    [[nodiscard]]
    int GetScore() const { return this->score; }
    void SetScore(int score) { this->score = score; }

    void ResetGameInfo()
    {
        lives = INIT_LIVES;
        score = 0;
        level = 0;
    }

    void ResetLevel() { level = 0; }
    int IncreaseLevel() { return ++level; }
    [[nodiscard]]
    int GetLevel() const { return level; }
private:
    int lives, score, level;
};


class Ship: public EntityComponent
{
    enum class State {
        Alive, Dead, Recover, GameOver
    };

public:
    Ship(): EntityComponent(true, true),
            tf{}, spr{}, tween{}, info{}
    {}

    ~Ship() override
    {
        delete tween;
    }

    void Init() override;

    void RecoverEventExit(float time)
    {
        spr->color = Color(255, 255, 255, 255);
        GetComponent<Body>()->show = true;
    }

    // Collision event handler
    static void OnCollide(Entity *thiz, Entity *other)
    {
        if (other->GetTag() == "Asteroid")
        {
            thiz->Components()->Get<Ship>()->Die();
            //GetCurrentScene()->DestroyEntity(*other);
        }
    }

    void Die()
    {
        if (states.GetCurrentState()->GetKey() == State::Alive)
            states.StartState(State::Dead);
    }

    void Update() override
    {
        states.Update(*GetTime());
    }

    void AliveState(float delta, float totalTime);
    void Win()
    {
        info.IncreaseLevel();

    }

    void GameOverStep(float delta, float time);
    void DeadStateEnter();
    void DeadStateStep(float delta, float time)
    {
        if (time > 1.f) states.StartState(State::Recover);
    }

    void RecoverStateStep(float delta, float time)
    {
        if (time > 4.f) { states.StartState(State::Alive); }

        Move();
        Fire();
        spr->color = Color(Rand::INext(256), Rand::INext(256), Rand::INext(256), Rand::INext(256));
    }

    void Fire()
    {
        // Press E to fire bullets
        if (GetInput()->GetKeyboard()->IsKeyDown(Key::E))
        {
            CreateBullet();
        }

        // Fire with mouse
        auto mousePos = GetInput()->GetMouse()->GetPosition();
        mousePos = GetScene()->GetCamera()->ScreenToWorld(mousePos);

        if (GetInput()->GetMouse()->ButtonDown(Button::Left))
        {
            auto angle = Math::PointDirection(tf->GetPositionLocal().x, tf->GetPositionLocal().y, mousePos.x, mousePos.y);
            if (spr->rotation != angle)
            {
                if (spr->rotation - angle > 180) angle += 360;
                else if (spr->rotation - angle < -180) angle -= 360;
                spr->rotation = Math::Mod(Math::Lerp(spr->rotation, angle, .1f), 360.f);
            }
            CreateBullet();
        }
    }

    void Move()
    {
        // Press A to rotate the ship left
        if (GetInput()->GetKeyboard()->IsKeyDown(Key::A))
        {
            spr->rotation += rotationSpeed_;
        }

        // Press D to rotate the ship right
        if (GetInput()->GetKeyboard()->IsKeyDown(Key::D))
        {
            spr->rotation -= rotationSpeed_;
        }

        // Calculate bullet timer
        if (bulletTimer_ > 0)
        {
            bulletTimer_ -= (int)GetTime()->DeltaTicks();
        }
        else
        {
            bulletTimer_ = 0;
        }

        // Press W to fire thrusters
        if (GetInput()->GetKeyboard()->IsKeyDown(Key::W))
        {
            speed_ = maxspeed_;
        }
        else
        {
            speed_ = 0;
        }

        // Calculate angled movement
        auto body = GetComponent<Body>();

        // Adjust velocity when player fires boosters
        if (speed_ > 0)
        {
            float axisTargetX = Math::TrajectoryX(spr->rotation, speed_);
            float axisTargetY = Math::TrajectoryY(spr->rotation, speed_);
            velocity = Math::Lerp(velocity, Vector2(axisTargetX, axisTargetY), .075f);
            if (velocity.Length() > speed_) velocity.Normalize() * speed_;
        }
        velocity = Math::Lerp(velocity, Vector2(), .00125f);
        body->velocity = velocity;

        auto roomSz = GetScene()->GetCamera()->GetWorldBounds();

        //std::cout << "Ship position " << tf->position.ToString() << '\n';

        // Wrap ship within room bounds
        tf->SetPositionLocal(Math::Wrap(tf->GetPositionLocal(),
                                        Vector2(-4, -4),
                                        Vector2(roomSz.w+8, roomSz.h+8)));

    }

    void CreateBullet();

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
    Vector2 axis = Vector2();
    Vector2 velocity = Vector2();
    float rotationSpeed_ = 3.f;
    float speed_ = 0;
    float maxspeed_ = 1.5f;

    GameInfo info;
    StateMachine<State> states;

public:
    // Takes a pre-existing Entity and adds the component makeup of the Player Ship
    static void MakeShip(Entity &e)
    {
        e.Components()->Add<SDG::Transform>(0.f, 0.f, 1.f, 1.f);

        // Set sprite
        auto &sprRenderer = e.Components()->Add<SDG::SpriteRenderer>();
        sprRenderer.SetSpriteByKey("ship");
        auto &body = e.Components()->Add<Body>();
        body.show = true;

        e.Components()->Add<Collider2D>();
        e.Components()->Add<Ship>();

        e.SetTag("Ship");
    }
};

#endif
