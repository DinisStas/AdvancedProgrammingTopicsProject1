#pragma once
#include "Character.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer.h"
#include "PlayerMissile.h"


class Player : public Character {
public:
    Player(b2WorldId worldId, SDL_Renderer* sdlRenderer,glm::vec2 position = { 0.0f, 0.0f }, glm::vec2 scale = { 50.0f, 50.0f },float rotation = 0.0f,float damage = 1000.0f,float worldWidth = 1200.0f, float worldHeight = 800.0f)
        : Character(worldId, sdlRenderer, "PlayerCharacter", 1001, "Player", position, scale, rotation, 1000), width(worldWidth), height(worldHeight)
    {
        SetupHp(100.0f);
        b2Body_SetUserData(bodyId, this);
        LoadAnimation("XenonSprites/Ship1.bmp", 1, 7, true, 4, 1);
    }

    //Enity manager scans the missiles vector if there are missiles adds them to the characters vector
    std::vector<std::unique_ptr<PlayerMissile>> missiles;

    virtual void FireMissile()
    {
        if (fireTimer >= fireRate) {
            fireTimer = 0.0f;
            glm::vec2 offset = { 0.0f, -20.0f };
            auto missile = std::make_unique<PlayerMissile>(worldId, sdlRenderer,position + offset, glm::vec2(25.0f));
            missiles.push_back(std::move(missile));
            //std::cout << "Fired missile, total: " << missiles.size() << std::endl;
        }
    }

    //Hardcoded values for player limits horrible I know
    void UpdatePosition() override
    {
        Character::UpdatePosition();

        //Takes player size into account when clampin
        float halfW = scale.x * 0.4f;
        float halfH = scale.y * 0.4f;

        //Clamp position within the map bounds
        position.x = glm::clamp(position.x, halfW, width - halfW);
        position.y = glm::clamp(position.y, halfH, height - halfH);

        //Push the Box2D body to the clamped position so physics stays in sync
        b2Body_SetTransform(bodyId, { position.x, position.y }, b2Body_GetRotation(bodyId));
    }


    void Render(Renderer& renderer, float deltaTime) override
    {
        Character::Render(renderer, deltaTime);
        //Infiltrates the renderer to run the timer
        fireTimer += deltaTime;
        //Erase missiles if they are dead
        missiles.erase(std::remove_if(missiles.begin(), missiles.end(),[](const std::unique_ptr<PlayerMissile>& m) { return m->isDead; }),missiles.end());
    }

private:
    float width;
    float height;

    //Timer used to measure when to fire again
    float fireTimer = 0.0f;
    //Yes I know its not the best implementation to have the fireRate inside the player
    float fireRate = 0.5f;
};



