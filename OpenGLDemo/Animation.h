#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "DrawCall.h"

class Animation {
public:
    SDL_Texture* textureID = nullptr; // kept as textureID to match Character.h usage
    int          frameWidth = 0;
    int          frameHeight = 0;
    int          numFrames = 0;
    int          startFrame = 0;
    int          numRows = 0;
    int          numColumns = 0;
    int          currentFrame = 0;
    float        frameTimer = 0.0f;
    float        frameRate = 12.0f;
    bool         loop = true;
    bool         bAnimationFinished = false;


    void SetRenderer(SDL_Renderer* renderer) { sdlRenderer = renderer; }

    //Calculates frames and their size based on the rows and columns provided
    void Load(const char* filepath,
        int rows, int columns,
        bool bLoop = true, int startingFrame = 0, int frames = -1);

    void Update(float deltaTime);

    //Returns a DrawCall built from screen-space position and size
    DrawCall GetDrawCall(float x, float y, float w, float h,
        double angle = 0.0,
        SDL_FlipMode flip = SDL_FLIP_NONE) const;

    void Cleanup();

private:
    SDL_Renderer* sdlRenderer = nullptr;
};