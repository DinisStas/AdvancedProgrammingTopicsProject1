#include "Renderer.h"
#include <iostream>

void Renderer::Init(SDL_Window* window)
{
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::Submit(const DrawCall& call)
{
    if (call.texture) {
        SDL_RenderTextureRotated(
            renderer,
            call.texture,
            &call.src,
            &call.dst,
            call.angle,
            &call.center,
            call.flip
        );
    }
    else {
        //Solid color and draw as a rectangle
        SDL_SetRenderDrawColor(renderer,
            call.solidColor.r, call.solidColor.g,
            call.solidColor.b, call.solidColor.a);
        SDL_RenderFillRect(renderer, &call.dst);
    }
}

void Renderer::Clear(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Renderer::Present()
{
    SDL_RenderPresent(renderer);
}

void Renderer::Cleanup()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}