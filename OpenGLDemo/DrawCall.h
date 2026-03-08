#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

//Used both in animation and renderer
struct DrawCall {
    SDL_Texture* texture = nullptr;
    SDL_FRect    src = { 0, 0, 0, 0 };  // UV source rect in pixels
    SDL_FRect    dst = { 0, 0, 0, 0 };  // Screen destination rect
    double       angle = 0.0;           // Rotation in degrees
    SDL_FPoint   center = { 0, 0 };        // Rotation center
    SDL_FlipMode flip = SDL_FLIP_NONE;
    SDL_Color    solidColor = { 0, 0, 0, 255 }; // Fallback color if no texture
};
