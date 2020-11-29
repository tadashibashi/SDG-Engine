/* =============================================================================
 * Game
 * 
 * 
 * ===========================================================================*/
#include "Game.h"
#include "Log.h"
#include <Engine/Graphics/GraphicsDeviceMgr.h>
#include <Engine/Time/GameTime.h>
#include <Engine/Input/InputMgr.h>

#include <sol/sol.hpp>
#include <Engine/Components/EntityComponent.h>
#include <Engine/Scenes/SceneMgr.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

namespace SDG
{
    class QuitListener : public SDG::EventListener<const SDL_QuitEvent &>
    {
    public:
        explicit QuitListener(bool &isRunning) : isRunning(isRunning) { }

        void Callback(const SDL_QuitEvent &ev) override
        {
            SDG_LOG("Received Quit Event at timestamp:", ev.timestamp);
            isRunning = false;
        }

        bool &isRunning;
    };

    QuitListener *quitListener = nullptr;

    Game::Game(const std::string &title, int x, int y, int width, int height, unsigned int windowFlags, unsigned long startingTime):
        graphics(new GraphicsDeviceMgr), time(), input(),
        spriteBatch(), content(), scenes(new SceneMgr),
        isRunning_(false)
    {
        graphics->Init(title.c_str(), x, y, width, height, windowFlags);
        Init();
    }

    Game::Game(const std::string &configPath):
        graphics(new GraphicsDeviceMgr), time(), input(),
        spriteBatch(), content(), scenes(new SceneMgr),
        isRunning_(false)
    {
        // Initialize lua/sol.
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        // Open script file.
        auto result = lua.safe_script_file(configPath);
        if (!result.valid())
        {
            throw std::runtime_error("Failed to parse game config file: "
                                     + configPath);
        }

        try
        {
            // Parse config file.
            std::string title = lua["title"];
            int windowPosX    = lua["position"]["x"];
            int windowPosY    = lua["position"]["y"];
            int windowWidth   = lua["resolution"]["width"];
            int windowHeight  = lua["resolution"]["height"];
            sol::table flags  = lua["window"];

            // Interpret window flags list
            unsigned winFlags = 0;
            for (auto &[k, v]: flags)
            {
                std::string flag = v.as<std::string>();
                if (flag == "fullscreen")
                {
                    winFlags |= SDL_WINDOW_FULLSCREEN;
                }
                else
                if (flag == "borderless")
                {
                    winFlags |= SDL_WINDOW_BORDERLESS;
                }
                else
                if (flag == "resizable")
                {
                    winFlags |= SDL_WINDOW_RESIZABLE;
                }
            }

            // Commit parsed information into graphics manager's initial window
            graphics->Init(title.c_str(), windowPosX, windowPosY, windowWidth,
                                    windowHeight, winFlags);
        }
        catch (const std::exception &e)
        {
            SDG_ERR("Problem parsing Game configuration file: ", configPath,
                    ": ", e.what());
            throw e;
        }
    }

    Game::~Game()
    {
        delete graphics;
        delete time;
        delete input;
        delete spriteBatch;
        delete content;
        delete scenes;
    }

    void Game::Run()
    {
        Init();
        PostInit();
        isRunning_ = true;
    }

    void Game::ProcessInput()
    {
        input->ProcessInput();
    }

    bool Game::Init()
    {
        content = new ContentMgr;
        time = new GameTime;
        input = new InputMgr;
        spriteBatch = new SpriteBatch(&graphics->GetCurrentDevice());

        if (!quitListener) quitListener = new QuitListener(isRunning_);
        input->QuitEvent += quitListener;

        Component::Provide(content);
        Component::Provide(time);
        Component::Provide(input);
        Component::Provide(spriteBatch);
        Component::Provide(graphics);
        EntityComponent::Provide(scenes);

        return true;
    }

    void Game::Update()
    {
        time->Update();
        scenes->Update();
        PostUpdate();
    }

    void Game::Draw()
    {
        auto &g = graphics->GetCurrentDevice();
        g.Clear();
        scenes->Draw();
        PostDraw();
        g.SwapBuffers();
    }

    void Game::Quit()
    {
        isRunning_ = false;
    }

    void Game::PostDraw()
    {

    }

    void Game::PostUpdate()
    {

    }
}