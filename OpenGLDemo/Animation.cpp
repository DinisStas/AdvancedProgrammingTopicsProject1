#include "Animation.h"
#include <SDL3/SDL.h>
#include "stb_image.h"
#include <iostream>

SDL_Texture* LoadTextureFromFile(SDL_Renderer* renderer, const char* filepath)
{
    int w, h, channels;
    unsigned char* pixels = stbi_load(filepath, &w, &h, &channels, 4);
    if (!pixels) {
        std::cout << "stb_image failed: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }

    //Remove magenta (255, 0, 255) and replacing it with transperency
    for (int i = 0; i < w * h * 4; i += 4) {
        if (pixels[i] == 255 && pixels[i + 1] == 0 && pixels[i + 2] == 255)
        {
            pixels[i] = 0;   // R
            pixels[i + 1] = 0;   // G
            pixels[i + 2] = 0;   // B
            pixels[i + 3] = 0;   // A — fully transparent
        }
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, pixels, w * 4);

    if (!surface) {
        std::cout << "SDL_CreateSurfaceFrom failed: " << SDL_GetError() << std::endl;
        stbi_image_free(pixels);
        return nullptr;
    }

    //This shoudn't happen unless texture is missing or path provided is wrong
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)  std::cout << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;

    SDL_DestroySurface(surface);
    stbi_image_free(pixels);
    return texture;
}

void Animation::Load(const char* filepath, int rows, int columns, bool bLoop, int startingFrame, int frames)
{
    if (!sdlRenderer) {
        std::cout << "Animation::Load — sdlRenderer is null! Call SetRenderer() first." << std::endl;
        return;
    }

    //Resets on every load since we use the same Animation.h when when iddle and than when dying
    numRows = rows;
    numColumns = columns;
    loop = bLoop;
    startFrame = startingFrame;
    currentFrame = startingFrame;
    bAnimationFinished = false;
    frameTimer = 0.0f;

    //Destroy old texture if replacing animation (swapping to death animation)
    if (textureID) {
        SDL_DestroyTexture(textureID);
        textureID = nullptr;
    }

    textureID = LoadTextureFromFile(sdlRenderer, filepath);
    if (!textureID) return;

    float texW = 0, texH = 0;
    SDL_GetTextureSize(textureID, &texW, &texH);
    frameWidth = (int)(texW / columns);
    frameHeight = (int)(texH / rows);
    numFrames = (frames == -1) ? (columns * rows) : frames;
}

void Animation::Update(float deltaTime)
{
    //Dont run animation on finish
    if (bAnimationFinished) return;

    //Update
    frameTimer += deltaTime;
    if (frameTimer >= 1.0f / frameRate) {
        frameTimer = 0.0f;
        ++currentFrame;
        //Loop logic
        if (currentFrame >= startFrame + numFrames) {
            if (loop)
                currentFrame = startFrame;
            else {
                currentFrame = startFrame + numFrames - 1;
                bAnimationFinished = true;
            }
        }
    }
}

DrawCall Animation::GetDrawCall(float x, float y, float w, float h,
    double angle, SDL_FlipMode flip) const
{
    int col = currentFrame % numColumns;
    int row = currentFrame / numColumns;

    DrawCall dc;
    dc.texture = textureID;
    dc.src = { (float)(col * frameWidth), (float)(row * frameHeight),
                   (float)frameWidth,         (float)frameHeight };
    dc.dst = { x, y, w, h };
    dc.angle = angle;
    dc.center = { w * 0.5f, h * 0.5f };
    dc.flip = flip;
    return dc;
}

void Animation::Cleanup()
{
    if (textureID) {
        SDL_DestroyTexture(textureID);
        textureID = nullptr;
    }
}