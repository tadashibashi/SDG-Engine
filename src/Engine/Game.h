/* =============================================================================
 * Game
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>

namespace SDG
{
    class GraphicsDeviceMgr;
    class GameTime;
    class InputMgr;
    class ContentMgr;
    class SpriteBatch;
    class SceneMgr;

    class SDG_API Game {
    public:
        explicit Game(const std::string &configPath);
        Game(const std::string &title, int x, int y, int width, int height, unsigned int windowFlags = 0, unsigned long startingTime = 0);
        ~Game();
        void Run();

        void RunOneFrame()
        {
            ProcessInput();
            Update();
            Draw();
        }

        SceneMgr *GetSceneMgr() { return scenes; }
        [[nodiscard]] bool IsRunning() const { return isRunning_; }
    private:
        // Happens once at the start of the game. Use for any necessary setup.
        bool Init();
        virtual void PostInit() {}
        // Called once every frame to handle user input.
        void ProcessInput();
        // Called once every frame to handle update logic.
        void Update();

        virtual void PostUpdate();
        // Called once every frame to handle drawing.
        void Draw();
        virtual void PostDraw();

        void Quit();

    private:
        bool isRunning_;
        GraphicsDeviceMgr *graphics;
        SpriteBatch *spriteBatch;
        GameTime *time;
        InputMgr *input;
        ContentMgr *content;

        SceneMgr *scenes;
    };

    SDG::Game *CreateGame();
}



