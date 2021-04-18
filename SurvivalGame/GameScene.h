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
#include <Engine/Platform.h>
#include <Engine/Graphics/OpenGL/SpriteBatch_GL.h>

using namespace SDG;

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
            spriteBatch = CreateNativeSpriteBatch(&GetGraphicsDeviceMgr()->GetCurrentDevice());
        }

        GetContent()->LoadAtlas("assets/atlas/texturepacker_test.xml");
        GetCamera()->SetScale(4.f);
        GetCamera()->SetPosition(GetCamera()->GetWorldBounds().w/2.f, GetCamera()->GetWorldBounds().h/2.f);

        // Tiled Level Parsing
        Tiled::TileMap map("assets/levels/level1.tmx");
        Texture2D tex = GetContent()->LoadTexture("assets/levels/" + map.tileSets[0].images[0].path, GL_NEAREST);

        for (auto &obj: map.objectLayers[0].objects)
        {
            if (obj.type == "Player")
            {
                auto &e = CreateEntity(CreatePlayer);
                e.Components()->Get<Transform>()->SetPositionLocal(Vector2(obj.x, obj.y));
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

        UpdateCameraPosition(false);

        spriteBatch->Begin(GetCamera()->GetMatrix(), SortOrder::Texture);

        SDG_ASSERT(!map.tileLayers.empty());
        SDG_ASSERT(!map.tileSets.empty());
        SDG_ASSERT(!map.objectLayers.empty());

        int tileSetTileWidthCount = map.tileSets[0].columns;
        int tileSetTileHeightCount = map.tileSets[0].tileCount / map.tileSets[0].columns;
        int tileWidth = map.tileWidth;
        int tileHeight = map.tileHeight;
        mapSize_ = Point(map.tileWidth * map.width, map.tileHeight * map.height);

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
                    Color(255, 255, 255, 255), 0);
        }

        spriteBatch->End();
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

    void UpdateCameraPosition(bool lerp = true)
    {
        Camera2D &cam = *GetCamera();
        Entity *player = GetSceneMgr()->CurrentScene()->GetFirstEntityWithTag("Player");
        Vector2 position = cam.GetPosition();
        if (player)
        {
            auto playerPos = player->Components()->Get<Transform>()->GetPosition();
            if (lerp)
                position = Math::Lerp(position, playerPos, .1f);
            else
                position = playerPos;
        }

        cam.SetPosition(position);
        auto worldBounds = cam.GetWorldBounds();

        // Clamp camera position
        auto camX = Math::Max<float>(position.x, worldBounds.w / 2.f);
        auto camY = Math::Max<float>(position.y, worldBounds.h / 2.f);
        cam.SetPosition(camX, camY);
    }

    void Update() override
    {
        Scene::Update();

        UpdateCameraPosition();

        // Generate Zombie by pressing 'S'
        if (GetInput()->GetKeyboard()->IsKeyDown(Key::S))
        {
            auto &zomb = GetScene()->CreateEntity("Zombie");
            double x = Rand::Next((float)mapSize_.w);
            double y = Rand::Next((float)mapSize_.h);
            zomb.Components()->Add<Transform>((float)x, (float)y, 1.f, 1.f);
            zomb.Components()->Add<SpriteRenderer>();
            zomb.Components()->Add<Body>();
            zomb.Components()->Add<Collider2D>();
            zomb.Components()->Add<Zombie>();

            size_t zombieSize = GetScene()->GetEntityTagList("Zombie").size();
            SDG_LOG("Zombie spawned at: ({0}, {1}). Total Zombies = {2}", x, y, zombieSize);

        }
    }

    void Draw() override
    {
        spriteBatch->RenderBatches();
        Scene::Draw();
    }

private:
    SpriteBatch *spriteBatch;
    Point mapSize_;
};
