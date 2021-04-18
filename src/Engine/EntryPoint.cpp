/* =============================================================================
 * EntryPoint
 * 
 * 
 * ===========================================================================*/
#include <Engine/Game.h>

#if defined (SDG_PLATFORM_WINDOWS) || defined(SDG_PLATFORM_APPLE) || defined(SDG_PLATFORM_ANDROID)
    int main(int argc, char *argv[])
    {
#if defined(_DEBUG)
        SDG::Log::Init();
#endif
        auto game = SDG::CreateGame();
        game->Run();
        while(game->IsRunning())
        {
            game->RunOneFrame();
        }
        delete game;

        return 0;
    }
#endif

#if defined (SDG_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>

    void RunGame(void *arg)
    {
        SDG::Game *game = static_cast<SDG::Game *>(arg);
        game->RunOneFrame();

        if (!game->IsRunning())
        {
            emscripten_cancel_main_loop();
        }
    }

    int main(int argc, char *argv[])
    {
        auto game = SDG::CreateGame();
        game->Run();
        emscripten_set_main_loop_arg(RunGame, (void *)game, -1, 1);

        delete game;

        return 0;
    }
#endif
