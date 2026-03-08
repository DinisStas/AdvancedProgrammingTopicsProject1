#pragma once
#include "box2d/box2d.h"
#include "EntityManager.h"
#include "InputHandler.h"
#include <vector>
#include <memory>
#include <cstdlib>


//Update physics and check collisions
class PhysicsManager
{
private:
    //World Id since need physics
    b2WorldId worldId;
    //Need a reference to the world manager since I'll need the list of characters every frame to update theirs physics and would not be the cleanest solution to do it with the a middle man (FrameWorkManager)
    EntityManager& entityManager;

public:
    PhysicsManager(b2WorldId worldId, EntityManager& charManager)
        : worldId(worldId), entityManager(charManager) {}

    //Call this update in FrameWorkManager to update all physics
    void UpdatePhysics(float deltaTime)
    {
        //Input and player movement
        entityManager.input.processKeyboard(deltaTime);

        b2World_Step(worldId, 1.0f / 60.0f, 4); 

        UpdateCharecters();

        CheckContacts();
    }

private:
    //Records collisions verified in sensor events since it checks collisions in pairs in both orders A B and B A, and we want to trigger the collision events once per collision pair (not sure if it is worth it in terms of optimazation)
    std::vector<std::pair<Character*, Character*>> registeredCollisions;

    void CheckContacts();

    //Gets all characters and update theirs physics
    void UpdateCharecters();
};