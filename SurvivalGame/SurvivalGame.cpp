#include <Engine/Scenes/SceneMgr.h>

#include "GameScene.h"
#include <Engine/Game.h>

using namespace SDG;

class SurvivalGame : public SDG::Game
{
public:
    SurvivalGame() : Game("Survival Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720)
    {

    }

    void PostInit() override
    {
        GetSceneMgr()->Register<GameScene>();
        GetSceneMgr()->Start<GameScene>();
    }

    ~SurvivalGame()
    {

    }
};

SDG::Game *SDG::CreateGame()
{
    return new SurvivalGame;
}
