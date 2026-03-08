#pragma once
#include "Character.h"
#include <iostream>
#include "Renderer.h"

class Enemy : public Character
{
public:
    //───────────────────────────────────────────────────────────Start setup(in this case is start this inside the enemy manager)───────────────────────────────────────────────────────────
    Enemy(b2WorldId worldId, SDL_Renderer* sdlRenderer, std::string name, uint16_t id, std::string tag , glm::vec2 position = { 0.0f, 0.0f },
        glm::vec2 scale = { 1.0f, 1.0f }, float rotation = 90.0f, float damage = 10.0f)
        :Character(worldId, sdlRenderer, "Enemy", 2001, "Enemy", position, scale, rotation, damage)
    {
        SetupHp(25.0f);
        //Using events since using version 3.0 need here to set pointer data for later on get a reference
        b2Body_SetUserData(bodyId, this);
        LoadAnimation("XenonSprites/LonerA.bmp",4, 4);
        /* The child enemy will load the animation inside their constructor this is just for Debug
         LoadAnimation()
         */
    }
};


