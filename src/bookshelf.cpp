// src/bookshelf.cpp
#include "../include/bookshelf.h"
#include "../include/tool.h"
#include "../include/constants.h"

Bookshelf::Bookshelf(SDL_Renderer* renderer, int x, int y, int w, int h, const char* texturePath)
    : Obstacle(renderer, x, y, w, h) {
    texture = loadTexture(texturePath, renderer);
    if (!texture) {
        SDL_Log("Failed to load bookshelf texture: %s", texturePath);
        SDL_Log("SDL Error: %s", SDL_GetError());
    }
}

void Bookshelf::render() {
    SDL_Rect destRect = {
        gridX * GRID_SIZE,
        gridY * GRID_SIZE,
        width * GRID_SIZE,
        height * GRID_SIZE
    };
    
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    } else {
        // Fallback to colored rectangle if texture not available
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brown color
        SDL_RenderFillRect(renderer, &destRect);
    }
}