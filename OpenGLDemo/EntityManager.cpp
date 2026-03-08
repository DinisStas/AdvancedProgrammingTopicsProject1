#include "EntityManager.h"

void EntityManager::AwakeCharacters()
{
    auto playerPtr = std::make_unique<Player>(
        worldId, renderer->GetSDLRenderer(),
        glm::vec2(width * 0.5f, height * 0.5f),glm::vec2(50.0f, 50.0f), 0.0f,   //Position, scale and rotation
        1000.0f, //damage
        (float)width, (float)height); //wordl width and height

    //Setup player for input
    player = playerPtr.get();

    //If I change my mind and decide to kill player like other characters
    characters.push_back(std::move(playerPtr));

    map.Init(*renderer,
        "XenonSprites/Background1.bmp", 60.0f, //Background 1 and scroll speed
        "XenonSprites/Background1.bmp", 120.0f, //Background 2 and scroll speed
        (float)width, (float)height); //wordl width and height
}


void EntityManager::CharacterUpdate(float deltaTime) {

    map.Update(deltaTime);

    //Run timer to spawn enemies
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnRate)
    {
        spawnTimer = 0.0f;
        SpawnRandom();
    }

    for (auto& c : characters)
    {
        //Checks if character left the map bounds if they did they Die
        if (c->position.x <= -10|| c->position.x >= width+100  || c->position.y <= -10 || c->position.y >= width + 100)
        {
            //Forces any chararacter to die
            c->isDead = true;
        }
    }

    //Get missiles from player to manage
    if (player) {
        for (auto& missile : player->missiles) characters.push_back(std::move(missile));
        player-> missiles.clear();
    }


    //Adding the characters that spawn outside of the CharacterManager to avoid changing vector while iterating it
    //std::vector<std::unique_ptr<Character>> toAdd;


	//Remove all dead characters, except player
    characters.erase(std::remove_if(characters.begin(), characters.end(),[this](const std::unique_ptr<Character>& c) {return c->IsDead() && c.get() != player;}),characters.end());


    //Render everything last
    Render(deltaTime);
}

void EntityManager::Render(float deltaTime) {
    //Draw Map first behind everything else
    map.Render(*renderer);

    //Draw all characters
    for (auto& c : characters) c->Render(*renderer, deltaTime);

    //Not sure if making here a seperate draw for player
}