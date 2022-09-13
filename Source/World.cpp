#include "GraphicsModule.h"

#include "World.h"

void World::StartUp()
{
    // Player
    GT_Texture* pTemp = g_graphicsModule.DefineTexture(TFN_PLAYER, TW_ACTOR, TH_ACTOR);

    Player* pPlayer = new Player();
    pPlayer->Init(Vec2(0.0f * g_unitX, 0.0f * g_unitY),
                  (s32)(TW_ACTOR * g_unitX), (s32)(TH_ACTOR * g_unitY),
                  pTemp);

    m_lstEntity.Push(pPlayer);

    pPlayer = new Player();
    pPlayer->Init(Vec2(20.0f * g_unitX, 65.0f * g_unitY),
                  (s32)(TW_ACTOR * g_unitX), (s32)(TH_ACTOR * g_unitY),
                  pTemp);

    m_lstEntity.Push(pPlayer);

    // Set camera
    g_graphicsModule.GetCamera().SetBoundary({ 0, 0, (s32)(TW_LOCATION * g_unitX * 2) - 1,
                                                     (s32)(TH_LOCATION * g_unitY) - 1 });
    g_graphicsModule.GetCamera().Attach(pPlayer);

    AddNote(PR_NOTE, "World start up");
}

void World::ShutDown()
{
    m_lstEntity.Mapcar([](auto pEntity, auto _) {
        pEntity->Clean();
        delete pEntity;
    }, nullptr);

    m_lstEntity.Clean();

    AddNote(PR_NOTE, "World shut down");
}

void World::Update(f32 dtTime)
{
    m_lstEntity.Mapcar([](auto pEntity, auto dtTime) {
        pEntity->Update(*(f32*)dtTime);
    }, &dtTime);
}

void World::Render()
{
    // Draw parallax
    SDL_Rect rect = { 0, 0, (s32)(TW_PARALLAX * g_unitX), (s32)(TH_PARALLAX * g_unitY) };
    g_graphicsModule.Draw(m_pParallax, 0, 0, &rect);

    // Draw background
    rect = { 0, 0, (s32)(TW_LOCATION * g_unitX), (s32)(TH_LOCATION * g_unitY) };
    g_graphicsModule.Draw(m_pBackground, 0, 0, &rect);
    rect.x = rect.w;
    g_graphicsModule.Draw(m_pBackground, 0, 1, &rect);

    // Draw player
    m_lstEntity.Mapcar([](auto pEntity, auto _) {
        pEntity->Draw();
    }, nullptr);
}