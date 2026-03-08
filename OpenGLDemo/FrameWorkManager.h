#pragma once
#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include "Renderer.h"
#include "EntityManager.h"
#include "PhysicsManager.h"

//Responsible for initializing all subsystems, running the game loop, updating physics, and rendering
class FrameworkManager
{
public:
    //Checks for erros initializes scripts
    bool Init(int width = 1200, int height = 800);

    //Runs Update and Render calculating and providing with deltaTime while capping fps and ending the game on player death
    void Run();
    void Shutdown();

private:
    //Window
    SDL_Window* m_window = nullptr;
    bool        m_running = false;
    int         m_width = 1200;
    int         m_height = 800;

    //Subsystems — init order matters, matches dependency order
    Renderer m_renderer;
    b2WorldId m_worldId;
    EntityManager* m_entities = nullptr;  // heap — needs renderer + worldId ready first
    PhysicsManager* m_physics = nullptr;  // heap — needs entityManager ready first

    //Timers
    float m_previousTime = 0.0f;
    const float  TARGET_FPS = 60.0f;
    const Uint64 TARGET_FRAMETIME = (Uint64)(1000.0f / 60.0f);

    //Updates physics
    void Update(float deltaTime);
    //Renders
    void Render(float deltaTime);
};