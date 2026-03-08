#pragma once
#include "Character.h"
#include "Enemy.h"

class PlayerMissile : public Character
{
public:
    PlayerMissile(b2WorldId worldId, SDL_Renderer* sdlRenderer, glm::vec2 position = { 0.0f, 0.0f },
        glm::vec2 scale = { 25.0f, 25.0f }, float rotation = 0.0f, float damage = 20.0f)
        : Character(worldId, sdlRenderer, "Missile", 1002, "Player", position, scale, rotation, damage)
    {
        //Using events since using version 3.0 need here to set pointer data for later on get a reference
        b2Body_SetUserData(bodyId, this);
        LoadAnimation("XenonSprites/missile.bmp", 3, 2, true, 0, 2);
        SetupHp(1.0f);
    }
private:

    //Speed
    b2Vec2 missileVelocity = { 0.0f, -200.0f };

    void UpdatePosition()
    {
        //Makes sure the physic boxes follow the enemy
        Character::UpdatePosition();
        b2Body_SetLinearVelocity(bodyId, missileVelocity);
    }
    void Die() override
    {
        Character::Die();

        LoadAnimation("XenonSprites/explode64.bmp", 2, 5, false);
    }
};

