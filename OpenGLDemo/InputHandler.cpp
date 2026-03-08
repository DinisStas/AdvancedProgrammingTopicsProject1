#include "InputHandler.h"


void InputHandler::processKeyboard(float deltaTime)
{
	if (!gotController)
	{
		openController();
		gotController = true;
	}
	if (!playerRef) return;

	b2Vec2 playerVelocity = { 0.0f, 0.0f };
	const bool* keyState = SDL_GetKeyboardState(NULL);

	//Only Dpad movement for this part of the project
	if (keyState[SDL_SCANCODE_W] || SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))playerVelocity.y = -speed;
	if (keyState[SDL_SCANCODE_S] || SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))playerVelocity.y = speed;
	if (keyState[SDL_SCANCODE_A] || SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))playerVelocity.x = -speed;
	if (keyState[SDL_SCANCODE_D] || SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))playerVelocity.x = speed;

	//Adding player movement trough box 2D since we will have to account phisics into our movement calcs
	b2Body_SetLinearVelocity(playerRef->bodyId, playerVelocity);

	//Keys for special action
	if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_LSHIFT] || keyState[SDL_SCANCODE_RSHIFT]
		|| SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) || SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)
		|| SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) || SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER))
	{
		playerRef->FireMissile();
	}

	//Quit game
	if (keyState[SDL_SCANCODE_ESCAPE]) { std::cout << "Quit Game" << std::endl; quitGame(); }
}

void InputHandler::openController()
{
	//Get the first controller
	int count = 0;
	SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
	for (int i = 0; i < count; i++)
	{
		if (SDL_IsGamepad(joysticks[i]))
		{
			gamepad = SDL_OpenGamepad(joysticks[i]);
			if (gamepad)
			{
				std::cout << "Controller connected: "
					<< SDL_GetGamepadName(gamepad) << std::endl;
				break;
			}
		}
	}
	SDL_free(joysticks);
}
