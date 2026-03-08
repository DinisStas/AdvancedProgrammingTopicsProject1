#pragma once
#include "Renderer.h"
#include <SDL3/SDL.h>
#include <string>

struct MapLayer {
    SDL_Texture* texture = nullptr;
    float        scrollSpeed = 0.0f;   
    float        scrollOffset = 0.0f;
    float        width = 0.0f;
    float        height = 0.0f;
};

//Beware that this doesnt use the animation importer so it still has magenta pixels rendered
class Map {
public:
    //Renderer is required upfront to load textures
    void Init(Renderer& renderer,
        const std::string& bgTexPath, float bgScrollSpeed,
        const std::string& fgTexPath, float fgScrollSpeed,
        float windowW, float windowH);

    void Update(float deltaTime);
    void Render(Renderer& renderer);
    void Cleanup();

private:
    MapLayer layers[2];
    float windowWidth = 0.0f;
    float windowHeight = 0.0f;

    //Reuses load texture
    SDL_Texture* LoadTexture(Renderer& renderer, const std::string& path);
};