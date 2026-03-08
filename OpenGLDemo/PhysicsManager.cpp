#include "PhysicsManager.h"


void PhysicsManager::CheckContacts()
{
    //Get all  SensorEvents tried with GetContactEvents but did not work
    b2SensorEvents events = b2World_GetSensorEvents(worldId);

    //Empties collisions each frame
    registeredCollisions.clear();

    //Check for all start of collisions
    for (int i = 0; i < events.beginCount; ++i)
    {
        //Provides both of the bodies that collide
        b2SensorBeginTouchEvent event = events.beginEvents[i];

        // Sensor events give you sensorShapeId and visitorShapeId
        void* sensorData = b2Body_GetUserData(b2Shape_GetBody(event.sensorShapeId));
        void* visitorData = b2Body_GetUserData(b2Shape_GetBody(event.visitorShapeId));


        //First check if both objects are Character since in this part only collision between characters matter
        Character* objectA = dynamic_cast<Character*>((Character*)sensorData);
        Character* objectB = dynamic_cast<Character*>((Character*)visitorData);

        //Registers collisions
        registeredCollisions.push_back({ objectA, objectB });


        //Verify first if both objects are Characters since both enemies and players are and we only need to check for them
        if (!objectA || !objectB) continue;

        //verify if collision isn't verified a second time in the same frame just reverse order
        bool alreadyRegistered = false;
        for (auto& [a, b] : registeredCollisions) {
            if ((a == objectA && b == objectB) || (a == objectB && b == objectA)) {
                alreadyRegistered = true;
                break; //Can't to continue here it will just break this for but not the next one
            }
        }
        if (alreadyRegistered) continue;

        //Confirm if the object isn't dead or dying
        if (objectA->isDying || objectA->isDead) continue;
        if (objectB->isDying || objectB->isDead) continue;

        std::string tagA = objectA->getTag();
        std::string tagB = objectB->getTag();

        //If tags are == there is no interaction since objects with the same tag don't interact with each other
        if (tagA == tagB) continue;

        //We verify if the enemy/enemyMissile enters in contact with player/playerMissile
        if ((tagA == "Player" || tagB == "Player") && (tagA == "Enemy" || tagB == "Enemy"))
        {
            //Yes both objects damage each other on contact the values are programmed in a way so missiles always get destroyed on contact, while player destroys anything they touch
            objectA->HpModify(-objectB->damage);
            objectB->HpModify(-objectA->damage);
            std::cout << "Object With name " << objectA->getName() << " with tag " << objectA->getTag() << " has " << objectA->getHp() << " Hp left" << std::endl;
            std::cout << "Object With name " << objectB->getName() << " with tag " << objectB->getTag() << " has " << objectB->getHp() << " Hp left" << std::endl;
        }
    }
}

void PhysicsManager::UpdateCharecters()
{
    //Get all characters from character manager and update their positions
    for (const std::unique_ptr<Character>& c : entityManager.getCharacters()) {
        Character* character = c.get();
        character->UpdatePosition();
    }
}