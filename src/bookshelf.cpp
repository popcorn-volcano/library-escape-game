// src/bookshelf.cpp
#include "../include/bookshelf.h"
#include "../include/tool.h"
#include "../include/constants.h"

Bookshelf::Bookshelf(SDL_Renderer* renderer, int x, int y, int w, int h)
    : Obstacle(renderer, x, y, w, h), bookCount(0) {
    // Try to load texture, but don't require it
    texture = loadTexture(BOOKSHELF_TEXTURE, renderer);  // Use the constant
    if (!texture) {
        SDL_Log("Using fallback rendering for bookshelf");
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
        // Use the loaded texture if available
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    } else {
        // Fallback to colored rectangle only if texture is null
        SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}