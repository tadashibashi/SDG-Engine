/* =============================================================================
 * GameScene
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Scenes/Scene.h>
#include <Engine/Components/SpriteRenderer.h>
#include <Engine/Components/Transform.h>
#include <Engine/Components/Body.h>
#include <Engine/Content/Tiled/TileMap.h>

#include <Engine/Math/Rand.h>
#include <Engine/Components/Collider.h>

#include "../Ship.h"
#include "../Asteroid.h"

class Ship;

using namespace SDG;

class GameScene : public Scene
{
public:
    GameScene()
    {

    }

    ~GameScene() override
    {
    }

    void OnStart() override
    {

    }

    void LoadContent() override;

    void OnEnd() override
    {

    }

    void OnPause() override
    {

    }

    void OnResume() override
    {

    }

    void Update() override
    {
        Scene::Update();
    }

    void Draw() override
    {
        Scene::Draw();
    }

private:
    Point mapSize_;
};
