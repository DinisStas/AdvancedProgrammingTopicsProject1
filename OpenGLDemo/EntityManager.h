#pragma once
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "InputHandler.h"
#include "Map.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include "Rusher.h"
#include "Loner.h"

//Spawns and stores all characters including enemy, player, missiles; Also serves as a general initializer: Setups up map and Input and renders the objects
class EntityManager
{
private:
    //Variables necessary to provide for the characters on spawn
    b2WorldId worldId;
    Renderer* renderer;

    //For enemy spawns
    float spawnTimer = 0.0f;
    float spawnRate = 1.0f;

    //world height and width for spawning in the correct position
    int width;
    int height;

    //Map initialized here would have to alter in cpp to alter spriteSheet
    Map map;

    //Player Necessary for the input handler as other miscelanous things (Loner to fire missiles towards the player)
    Player* player = nullptr;

public:

    //Initialize input handler on awake but update it inside the PhysicsManager
    InputHandler input;

    EntityManager(b2WorldId worldId, Renderer* renderer, int width = 800, int height = 600) :worldId(worldId), renderer(renderer), width(width), height(height)
    { AwakeCharacters();
    input.ActivateInput(player); }

    //Stores all currently alive charecters, (dead characters are removed in update)
	std::vector<std::unique_ptr<Character>> characters;
    
    //Have a default player and map ready to initialize the game
    void AwakeCharacters();

	//Removes any dead charecters runs other scripts inside the CharacterManager
	void CharacterUpdate(float deltaTime);

    //Draw everything in the following order Map, Characters and Player
    void Render(float deltaTime);

    //Can overwrite the Map in the case I want another one (Will have to insert new map inside the FrameWorkManager)
    void LoadMap(const std::string& bgTexPath, float bgScrollSpeed, const std::string& fgTexPath, float fgScrollSpeed)
    {
        //Destroy previous map and iniate new one based of characteristics desired
        map.Cleanup();
        map.Init(*renderer, bgTexPath, bgScrollSpeed, fgTexPath, fgScrollSpeed,(float)width, (float)height);
    }

	std::vector<std::unique_ptr<Character>>& getCharacters() { return characters; } 

    Player* getPlayer() { return player; }
    Map& GetMap() { return map; }

private:
    void SpawnRandom()
    {
        //Enemies spawn from the edges of the map be it from the top(height) or right(width)
        int spawnX = width;
        int spawnY = height;

        int randomSpawnX = 50.0f + (std::rand() % (int)width);
        int randomSpawnY =(std::rand() % (int)height) - 50.0f;

        int roll = std::rand() % 2;
        std::cout << "SpawnRandom roll: " << roll << std::endl;
    
        if (roll == 0) characters.push_back(std::make_unique<Rusher>(worldId, renderer->GetSDLRenderer(), glm::vec2(randomSpawnX, 0)));
        if (roll == 1) characters.push_back(std::make_unique<Loner>(worldId, renderer->GetSDLRenderer(),glm::vec2(0, randomSpawnY)));
    }
};

