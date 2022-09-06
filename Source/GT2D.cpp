/* TODO
 * - Make GT2D engine it's module? just for AddNote()
 * - Log errors
 */

/* ====== INCLUDES ====== */
#include "SDL.h"
#include "SDL_mixer.h"

#include "DebugLogManager.h"
#include "ClockManager.h"
#include "GTMath.h"
#include "InputModule.h"
#include "SoundModule.h"
#include "Game.h"

#include "GT2D.h"

/* ====== DEFINES ====== */
#define FPS 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

/* ====== VARIABLES ====== */
GT2D g_GT2D;

/* ====== METHODS ====== */
b32 GT2D::StartUp()
{
    // Start up log manager
    if (!g_debugLogMgr.StartUp())
        return false;

    { // Init all SDL stuff
        if (0 != SDL_Init(SDL_INIT_EVERYTHING))
        {
            // SDL_GetError()
            return false;
        }

        // Create window
        if ( nullptr == (m_pWindow = SDL_CreateWindow(
                            "GT2D",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS)) )
        {
            // SDL_GetError()
            return false;
        }

        // Create renderer
        if ( nullptr == (m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED)) )
        {
            // SDL_GetError()
            return false;
        }

        // Init SDL Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            // Mix_GetError()
            return false;
        }
    }

    { // Start up engine`s modules
        if (!g_clockMgr.StartUp(FPS))
            return false;
        if (!GTM::StartUp())
            return false;
        if (!g_inputModule.StartUp())
            return false;
        if (!g_soundModule.StartUp())
            return false;
        if (!g_game.StartUp())
            return false;
    }

    AddNote(PR_NOTE, "Engine started successfully");

    // Success
    return true;
}

void GT2D::ShutDown()
{
    { // Shut down SDL
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);

        Mix_Quit();
        SDL_Quit();
    }

    { // Shut down engine's modules
        g_game.ShutDown();
        g_soundModule.ShutDown();
        g_inputModule.ShutDown();
        GTM::ShutDown();
        g_clockMgr.ShutDown();
    }

    AddNote(PR_NOTE, "Engine shut down");

    // Shut down log manager
    g_debugLogMgr.ShutDown();
}

s32 GT2D::Run()
{
    while (g_game.Running())
    {
        if (!g_inputModule.HandleEvents())
            break;

        g_game.Update(g_clockMgr.GetDelta());
        g_game.Render();

        g_clockMgr.Sync();
    }

    return EC_OK;
}