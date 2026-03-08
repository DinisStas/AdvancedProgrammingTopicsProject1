#pragma once
#include <SDL3/SDL.h>
#include "DrawCall.h"

class Renderer {
public:
    //Creates the SDL_Renderer from the given window
    void Init(SDL_Window* window);

    //If there is no texture provided than provides solid color rectangle
    void Submit(const DrawCall& call);

    void Clear(SDL_Color color = { 30, 30, 30, 255 });
    //Necessary since its sdl
    void Present();
    void Cleanup();

    SDL_Renderer* GetSDLRenderer() const { return renderer; }

private:
    SDL_Renderer* renderer = nullptr;
};