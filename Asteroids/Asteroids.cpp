/* =============================================================================
 * Asteroids
 * 
 * 
 * ===========================================================================*/
#include <Engine/Game.h>
#include "Scenes/GameScene.h"

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

private:

    void PostInit() override
    {
        GetSceneMgr()->Register<GameScene>();
        GetSceneMgr()->Start<GameScene>();
    }
};

Game *SDG::CreateGame()
{
    return new Asteroids;
}



