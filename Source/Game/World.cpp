/* ====== INCLUDES ====== */
#include "GraphicsModule.h"
#include "ScriptModule.h"
#include "DamageManager.h"
#include "Actor.h"
#include "Weapon.h"
#include "Game.h"

#include "World.h"

/* ====== DEFINES ====== */
#define CAMERA_DEFAULT_X 0
#define CAMERA_DEFAULT_Y 0
#define CAMERA_BOUNDS_DEFAULT_X1 0
#define CAMERA_BOUNDS_DEFAULT_Y1 0
#define CAMERA_BOUNDS_DEFAULT_X2 ( g_graphicsModule.GetScreenWidth() - 1 )
#define CAMERA_BOUNDS_DEFAULT_Y2 ( g_graphicsModule.GetScreenHeight() - 1 )

#define GROUND_BOUNDS_DEFAULT_X1 0
#define GROUND_BOUNDS_DEFAULT_Y1 0
#define GROUND_BOUNDS_DEFAULT_X2 ( g_graphicsModule.GetScreenWidth() - 1 )
#define GROUND_BOUNDS_DEFAULT_Y2 ( g_graphicsModule.GetScreenHeight() - 1 )

/* ====== METHODS ====== */
void World::StartUp()
{
    // Defaults
    m_groundBounds = { GROUND_BOUNDS_DEFAULT_X1, GROUND_BOUNDS_DEFAULT_Y1,
                       GROUND_BOUNDS_DEFAULT_X2, GROUND_BOUNDS_DEFAULT_Y2 };
    m_switchLocation = -1;

    g_graphicsModule.GetCamera().SetBounds({ CAMERA_BOUNDS_DEFAULT_X1, CAMERA_BOUNDS_DEFAULT_Y1,
                                             CAMERA_BOUNDS_DEFAULT_X2, CAMERA_BOUNDS_DEFAULT_Y2 });
    g_graphicsModule.GetCamera().SetPosition(CAMERA_DEFAULT_X, CAMERA_DEFAULT_Y);

    AddNote(PR_NOTE, "World started");
}

void World::ShutDown()
{
    CleanEntities();
    CleanWeapons();

    AddNote(PR_NOTE, "World shut down");
}

void World::Update(f32 dtTime)
{
    HandleSwitchLocation();
    UpdateEntities(dtTime);
    RemoveEntities();
}

void World::Render()
{
    // Draw entities
    m_lstEntity.Mapcar([](auto pEntity) { pEntity->Draw(); });
}

void World::HandleSwitchLocation()
{
    // Check if we don't need to switch location
    if (m_switchLocation == -1)
        return;

    // Stop sounds and music
    g_soundModule.StopSoundsAndMusic();

    // Clean current location stuff
    CleanEntities();

    // Save switchLocation value
    s32 location = m_switchLocation;
    // Call switch location function
    g_scriptModule.SwitchLocation(g_game.GetScript(), m_switchLocation);
    // If there're no another switch location request then set to -1
    if (m_switchLocation == location)
        m_switchLocation = -1;
}

void World::UpdateEntities(f32 dtTime)
{
    auto end = m_lstEntity.End();
    for (auto it = m_lstEntity.Begin(); it != end; ++it)
        it->data->Update(dtTime);
}

void World::RemoveEntities()
{
    auto end = m_lstRemove.End();
    for (auto it = m_lstRemove.Begin(); it != end; ++it)
    {
        // Remove from entity list
        m_lstEntity.Remove(it->data);

        // Free memory
        it->data->Clean();
        delete it->data;
    }

    // Clean remove list
    m_lstRemove.Clean();
}

void World::CleanEntities()
{
    m_lstEntity.Mapcar([](auto pEntity) {
        pEntity->Clean();
        delete pEntity;
    });
    m_lstEntity.Clean();
    m_lstRemove.Clean();
}

void World::CleanWeapons()
{
    m_lstWeapon.Mapcar([](auto pWeapon) { if (pWeapon) delete pWeapon; });
    m_lstWeapon.Clean();
}

