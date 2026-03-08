#pragma once
#include <glm/glm.hpp>
#include <iostream>

 class GameObject
{
protected:
	//Object Identifers Id, name and tag to which group they belong to

	const std::string name;
	//Id numbers 1000-1999 character spawned objects and upgrades, 2000-2999 enemies, 3000-3999  map objects.- Note this was supose to have more utility unfornetly it does not
	const uint16_t objectId;
	//Possible tags: Player, Enemy, Map, Projectiles, Upgrades
	const std::string objectTag;

public:
	//Constructor: childreen need to setup id, name and tag, when created adjust acordingly
	GameObject(std::string name = "Unamed", uint16_t objectId = 999, std::string objectTag = "Unset", glm::vec2 position = { 0.0f, 0.0f }, glm::vec2 scale = { 1.0f, 1.0f }, float rotation = 270.0f)
		: name(name), objectId(objectId), objectTag(objectTag), position(position), scale(scale), rotation(rotation) {};

	//Object global position and scale
	glm::vec2 position = { 0.0f, 0.0f };
	glm::vec2 scale = { 1.0f, 1.0f };
	float rotation = 0.0f;

	std::string getName() const { return name; };
	std::string getTag() const { return objectTag; };
	uint16_t getId() const { return objectId; };
};

