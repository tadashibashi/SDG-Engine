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
#include "Zombie.h"
#include "Player.h"
#include <Engine/Math/Rand.h>
#include <Engine/Components/Collider.h>

using namespace SDG;

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

class GameScene : public Scene
{
public:
    GameScene(): spriteBatch(nullptr)
    {

    }
    ~GameScene() override
    {
        delete spriteBatch;
    }

    void OnStart() override
    {

    }

    void LoadContent() override
    {
        if (!spriteBatch)
        {
            spriteBatch = new SpriteBatch(&GetGraphicsDeviceMgr()->GetCurrentDevice());
        }
        GetContent()->LoadAtlas("assets/atlas/texturepacker_test.png");
        GetCamera()->SetScale(4.f);

        // tiled level parsing
        Tiled::TileMap map("assets/levels/level1.tmx");
        Texture2D tex = GetContent()->LoadTexture("assets/levels/" + map.tileSets[0].images[0].path, GL_NEAREST);

        spriteBatch->Begin(GetCamera()->GetMatrix());

        SDG_ASSERT(!map.tileLayers.empty());
        SDG_ASSERT(!map.tileSets.empty());
        SDG_ASSERT(!map.objectLayers.empty());

        int tileSetTileWidthCount = map.tileSets[0].columns;
        int tileSetTileHeightCount = map.tileSets[0].tileCount / map.tileSets[0].columns;
        int tileWidth = map.tileWidth;
        int tileHeight = map.tileHeight;

        // Visit each tile and draw it.
        for (int tile = 0, size = map.tileLayers[0].tiles.size(); tile < size; ++tile)
        {
            // Get the tile index
            int tileIndex = map.tileLayers[0].tiles[tile] - map.tileSets[0].firstgid;
            if (tileIndex < 0) // No tile on this coordinate, skip drawing.
                continue;

            spriteBatch->DrawTexture(
                    tex,
                    FRectangle(tileWidth * (tile % map.width), tileHeight * (int)(tile/map.width), tileWidth, tileHeight),
                    FRectangle((1.f/(float)tileSetTileWidthCount) * (float)(tileIndex % tileSetTileWidthCount), (1.f - (1.f/(float)tileSetTileHeightCount) * (float)(tileIndex / tileSetTileWidthCount + 1.f)), 1.f/(float)tileSetTileWidthCount, 1.f/(float)tileSetTileHeightCount),
                    Color(255, 255, 255, 255));
        }

        spriteBatch->End();

        Point roomSize = GetGraphicsDeviceMgr()->GetCurrentDevice().GetBackBufferSize();
        for (auto &obj: map.objectLayers[0].objects)
        {
            if (obj.type == "Player")
            {
                auto &e = CreateEntity(CreatePlayer);
                e.Components()->Get<Transform>()->position = Vector2(obj.x, obj.y);
                e.Components()->Get<Body>()->show = false;
            }
            else if (obj.type == "Zombie")
            {
                auto &z = CreateEntity("Zombie");
                auto &tf = z.Components()->Add<Transform>(obj.x, obj.y, 1.f, 1.f);
                z.Components()->Add<SpriteRenderer>();
                z.Components()->Add<Body>().show = false;
                z.Components()->Add<Collider2D>();
                z.Components()->Add<Zombie>();
            }
        }
    }

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

        Camera2D &cam = *GetCamera();
        Entity *player = GetSceneMgr()->CurrentScene()->GetFirstEntityWithTag("Player");
        if (player)
        {
            auto playerPos = player->Components()->Get<Transform>()->GetPosition();
            cam.SetPosition(Math::Lerp(cam.GetPosition(), playerPos, .1f));
        }

        auto worldBounds = cam.GetWorldBounds();

        // Clamp camera position
        auto camX = Math::Max<float>(cam.GetPosition().x, worldBounds.w / 2.f);
        auto camY = Math::Max<float>(cam.GetPosition().y, worldBounds.h / 2.f);
        cam.SetPosition(camX, camY);

        // Generate Zombie by pressing 'S'
        if (GetInput()->GetKeyboard()->KeyPressed(Key::S))
        {
            auto &zomb = GetCurrentScene()->CreateEntity("Zombie");
            double x = Rand::Next(worldBounds.w);
            double y = Rand::Next(worldBounds.h);
            zomb.Components()->Add<Transform>(x, y, 1.f, 1.f);
            zomb.Components()->Add<SpriteRenderer>();
            zomb.Components()->Add<Body>();
            zomb.Components()->Add<Collider2D>();
            zomb.Components()->Add<Zombie>();

            SDG_LOG("Zombie spawned at: ({0}, {1})", x, y);
        }
    }

    void Draw() override
    {
        spriteBatch->RenderBatches();
        Scene::Draw();
    }

private:
    SpriteBatch *spriteBatch;
};
