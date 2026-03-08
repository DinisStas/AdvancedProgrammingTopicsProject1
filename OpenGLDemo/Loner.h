#pragma once
#include "Enemy.h"

class Loner : public Enemy
{
public:
    Loner(b2WorldId worldId, SDL_Renderer* sdlRenderer, glm::vec2 position = { 0.0f, 0.0f },
        glm::vec2 scale = { 50.0f, 50.0f }, float rotation = 270.0f, float damage = 10.0f)
        : Enemy(worldId, sdlRenderer,  "Rusher", 2003, "Enemy", position, scale, rotation, damage)
    {
        SetupHp(20.0f);
        LoadAnimation("XenonSprites/LonerA.bmp", 4, 4);
    }
    b2Vec2 enemyVelocity = {100.0f, 0.0f };

    //Adds here enemy movement
    virtual void UpdatePosition()
    {
        //Makes the previous codes runs
        Enemy::UpdatePosition();
        b2Body_SetLinearVelocity(bodyId, enemyVelocity);
    }
};


