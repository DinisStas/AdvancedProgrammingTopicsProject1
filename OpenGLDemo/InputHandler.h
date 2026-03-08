#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Player.h" 



class InputHandler {
private:

	//Pointer to the player so I can change its position
	Player* playerRef = nullptr;

	//Speed at which the player will move
	float speed = 250.f;

	void quitGame() { std::cout << "Quit trough input handler" << std::endl; exit(0); void SDL_Quit(void); };

	//Called once to get controller
	void openController();
	//GamePadSetup
	SDL_Gamepad* gamepad = nullptr;
	bool gotController = false;

public:
	//Get player for to get than the position
	void ActivateInput(Player* playerReference) { playerRef = playerReference; };

	//Handles
	void processKeyboard(float deltaTime);

};

