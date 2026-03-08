#include "Map.h"
#include "stb_image.h"
#include <iostream>

static SDL_Texture* LoadTextureSTB(SDL_Renderer* renderer, const std::string& path)
{
    int w, h, channels;
    unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &channels, 4);
    if (!pixels) {
        std::cout << "stb_image: failed to load '" << path << "': "
            << stbi_failure_reason() << std::endl;
        return nullptr;
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(w, h,SDL_PIXELFORMAT_RGBA32,pixels, w * 4);

    if (!surface) {
        std::cout << "SDL_CreateSurfaceFrom failed: " << SDL_GetError() << std::endl;
        stbi_image_free(pixels);
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
        std::cout << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;

    SDL_DestroySurface(surface);
    stbi_image_free(pixels);
    return texture;
}

void Map::Init(Renderer& renderer,
    const std::string& bgTexPath, float bgScrollSpeed,
    const std::string& fgTexPath, float fgScrollSpeed,
    float windowW, float windowH)
{  
    //I don't like this at but texture and scroll speed did not allow me to do this much easier
    windowWidth = windowW;
    windowHeight = windowH;

    layers[0].texture = LoadTexture(renderer, bgTexPath);
    layers[0].scrollSpeed = bgScrollSpeed;
    layers[0].scrollOffset = 0.0f;
    layers[0].width = windowW;
    layers[0].height = windowH;

    layers[1].texture = LoadTexture(renderer, fgTexPath);
    layers[1].scrollSpeed = fgScrollSpeed;
    layers[1].scrollOffset = 0.0f;
    layers[1].width = windowW;
    layers[1].height = windowH;
}

void Map::Update(float deltaTime)
{
    for (int i = 0; i < 2; i++) {
        layers[i].scrollOffset += layers[i].scrollSpeed * deltaTime;
        if (layers[i].scrollOffset >= layers[i].height)
            layers[i].scrollOffset = 0.0f;
    }
}

void Map::Render(Renderer& renderer)
{
    //Has 2 scroll tiles so there won't be a moment where the background is offscreen done this way since in the project instead of having a long repeatable background we have reletively small images
    for (int i = 0; i < 2; i++) {
        const MapLayer& layer = layers[i];
        if (!layer.texture) continue;

        //Tile 1 scroll
        DrawCall dc1;
        dc1.texture = layer.texture;
        dc1.src = { 0, 0, layer.width, layer.height };
        dc1.dst = { 0, layer.scrollOffset, layer.width, layer.height };
        renderer.Submit(dc1);

        //Tile 2 scroll
        DrawCall dc2;
        dc2.texture = layer.texture;
        dc2.src = { 0, 0, layer.width, layer.height };
        dc2.dst = { 0, layer.scrollOffset - layer.height, layer.width, layer.height };
        renderer.Submit(dc2);
    }
}

void Map::Cleanup()
{
    for (int i = 0; i < 2; i++) {
        if (layers[i].texture) {
            SDL_DestroyTexture(layers[i].texture);
            layers[i].texture = nullptr;
        }
    }
}

SDL_Texture* Map::LoadTexture(Renderer& renderer, const std::string& path)
{
    return LoadTextureSTB(renderer.GetSDLRenderer(), path);
}