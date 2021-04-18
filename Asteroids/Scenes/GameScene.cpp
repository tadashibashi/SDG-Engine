/* =============================================================================
 * GameScene
 * 
 * 
 * ===========================================================================*/
#include "GameScene.h"

void GameScene::LoadContent()
{
    GetContent()->LoadAtlas("assets/atlas/MainAtlas.xml");
    GetCamera()->SetScale(2.f);
    GetCamera()->SetPosition(GetCamera()->GetWorldBounds().w/2, GetCamera()->GetWorldBounds().h/2);

    if (!GetFirstEntityWithTag("Ship"))
    {
        SDG_LOG("Creating Ship");
        CreateEntity(Ship::MakeShip,
                     Vector2(GetCamera()->GetWorldBounds().w/2, GetCamera()->GetWorldBounds().h/2));
    }
    else
    {
        SDG_LOG("Ship already exists, skipping creation");
    }

    // Create asteroids inside room
    auto bounds = GetCamera()->GetWorldBounds();
    for (int i = 0; i < 3; ++i)
    {
        auto &ast = CreateEntity(Asteroid::MakeAsteroid);
        ast.Components()->Get<Transform>()->SetPositionLocal(
                Vector2(Rand::Next(bounds.w), Rand::Next(bounds.h)));
    }

    // Tiled Level Parsing

}
