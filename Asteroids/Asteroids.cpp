/* =============================================================================
 * Asteroids
 * 
 * 
 * ===========================================================================*/
#include <Engine/Game.h>
#include "Scenes/GameScene.h"
#include <Engine/Scenes/SceneMgr.h>
#include <Engine/Events/Delegate.h>

using namespace SDG;

class Asteroids : public Game {
public:
    Asteroids() : Game("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       480, 480, 0, 0)
    {

    }

    ~Asteroids()
    {

    }
    void yo(int a)
    {
        SDG_LOG("Yo called with value: 10 * a = {0}", a * 10);
    }
private:
    Delegate<int> d;
    void PostInit() override
    {
        void (*hey)(int) = [](int a) { SDG_LOG("Delegate \"hey\" called with value: {0}", a); };
        d.AddListener(hey);
        d.AddListener(this, &Asteroids::yo);
        d(10);
        SDG_LOG("Delegate size: {0}", d.GetHandleSize());
        d.RemoveListener(hey);
        SDG_LOG("Delegate size: {0}", d.GetHandleSize());

        GetSceneMgr()->Register<GameScene>();
        GetSceneMgr()->Start<GameScene>();
    }
};

Game *SDG::CreateGame()
{
    return new Asteroids;
}



