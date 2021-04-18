/* =============================================================================
 * Ship
 * 
 * 
 * ===========================================================================*/
#include "Ship.h"
#include "Bullet.h"

void Ship::DeadStateEnter()
{
    SDG_LOG("You lost a life!");
    info.LoseLife();
    if (!info.HasLives())
    {
        SDG_LOG("GAME OVER!!!");
        states.StartState(State::GameOver);
    }

    spr->color = Color(0, 0, 0, 0);

    GetComponent<Body>()->show = false;
    GetComponent<Body>()->velocity = Vector2();
    velocity = Vector2();
    SDG_LOG("Lives left: {0}", info.GetLives());
}

void Ship::CreateBullet()
{
    if (bulletTimer_ == 0)
    {
        auto bulletPos =  GetComponent<Transform>()->GetPosition();
        bulletPos.y -= 1;
        auto &e = GetScene()->CreateEntity(Bullet::MakeBullet, bulletPos);
        e.Components()->Get<Body>()->velocity = Math::Trajectory(spr->rotation, 1.f) * 8.f;

        e.Components()->Get<Transform>()->position = bulletPos;

        bulletTimer_ = bulletTimerMax_;
    }
}

void Ship::Init()
{
    if (tween) return;

    //GetEntity()->SetPersistent(true);

    tween = new Tween([this](float val) { tf->scale = Vector2(val * Math::Sign(tf->scale.x), val); },
                      1.f, 5.f, 1.f,TweenFunctions::EaseInCubic);
    tween->SetYoyo(true);

    // Setup Position and image
    tf = GetComponent<Transform>();
    spr = GetComponent<SpriteRenderer>();
    auto size = GetScene()->GetCamera()->GetWorldBounds();
    tf->position = Vector2(size.w/2, size.h/2);

    // Setup Collider
    GetComponent<Collider2D>()->SetCallback(OnCollide);

    // Setup State Machine
    states.AddState(State::Alive)
            .OnStep(this, &Ship::AliveState);
    states.AddState(State::Dead)
            .OnEnter(this, &Ship::DeadStateEnter)
            .OnStep(this, &Ship::DeadStateStep);
    states.AddState(State::Recover)
            .OnStep(this, &Ship::RecoverStateStep)
            .OnExit(this, &Ship::RecoverEventExit);
    states.AddState(State::GameOver)
            .OnStep(this, &Ship::GameOverStep);

    states.StartState(State::Alive);
}

void Ship::AliveState(float delta, float totalTime)
{
    Move();
    Fire();
}

void Ship::GameOverStep(float delta, float time)
{
    if (time > 2.f)
    {
        GetSceneMgr()->Start<GameScene>();
        info.ResetLevel();
        info.SetScore(0);
        info.SetLives(INIT_LIVES);
        auto worldBounds = GetScene()->GetCamera()->GetWorldBounds();
        this->tf->SetPositionLocal(Vector2(worldBounds.w/2, worldBounds.h/2));
        states.StartState(State::Alive);
        spr->color = Color(255, 255, 255, 255);
        GetComponent<Body>()->show = true;
    }
}


