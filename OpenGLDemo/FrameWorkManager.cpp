#include "FrameworkManager.h"
#include <iostream>
#include <ctime>

bool FrameworkManager::Init(int width, int height)
{
    m_width = width;
    m_height = height;

    // ── SDL ────────────────────────────────────────────────────────────────
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_window = SDL_CreateWindow("XenonGame", width, height, 0);
    if (!m_window) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // ───────────────────────────────────────────Create Renderer ───────────────────────────────────────────────────────────
    m_renderer.Init(m_window);

    // ─────────────────────────────────────────── Box2D ──────────────────────────────────────────────────────────────
    std::srand((unsigned)std::time(nullptr));
    b2WorldDef worldDef = b2DefaultWorldDef();
    //By default it should alredy be 0 if I am not wrong 
    worldDef.gravity = { 0.0f, 0.0f };
    m_worldId = b2CreateWorld(&worldDef);

    //─────────────────────────────────────────── Subsystems — order matters or it might break ─────────────────────────────────────────
    //EntityManager needs renderer + worldId ready
    m_entities = new EntityManager(m_worldId, &m_renderer, width, height);

    // PhysicsManager needs entityManager ready
    m_physics = new PhysicsManager(m_worldId, *m_entities);

    m_running = true;
    return true;
}

void FrameworkManager::Run()
{
    m_previousTime = (float)SDL_GetTicks() / 1000.0f;

    while (m_running)
    {
        Uint64 ticks = SDL_GetTicks();
        float  time = ticks / 1000.0f;
        float  deltaTime = time - m_previousTime;

        //Prevents issues with really low fps
        if (deltaTime > 0.25f) deltaTime = 0.25f;
        m_previousTime = time;

        //Checks if tgere uinst a quit event
        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT) m_running = false;

        //Check for player death ends program on player dying
        if (m_entities->getPlayer() && m_entities->getPlayer()->IsDead()) {
            std::cout << "Player has died " << std::endl;
            m_running = false;
        }

        Update(deltaTime);
        Render(deltaTime);

        //Fps cap
        Uint64 frameTime = SDL_GetTicks() - ticks;
        if (frameTime < TARGET_FRAMETIME)
            SDL_Delay((Uint32)(TARGET_FRAMETIME - frameTime));
    }
}

void FrameworkManager::Update(float deltaTime)
{
    //PlayerInput → physics step → position sync → checks contacts
    m_physics->UpdatePhysics(deltaTime);

    //Checks Spawn → out-of-bounds KILL → add missiles from player  →despawn 
    m_entities->CharacterUpdate(deltaTime);

}

void FrameworkManager::Render(float deltaTime)
{
    m_renderer.Clear({ 10, 10, 20, 255 });
    //Map, characters and player rendereded inside the m_entities
    m_entities->Render(deltaTime);  
    //Swaps the back buffer to the screen
    m_renderer.Present();
}


void FrameworkManager::Shutdown()
{
    //Deletion order matters if I am not wrong

    //Entities first — destroys b2Bodies while world still exists
    delete m_physics;
    delete m_entities;

    //World after bodies are gone
    b2DestroyWorld(m_worldId);

    m_renderer.Cleanup();
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}