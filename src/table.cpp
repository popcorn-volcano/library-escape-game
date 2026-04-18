// src/table.cpp
#include "../include/table.h"
#include "../include/tool.h"
#include "../include/constants.h"

Table::Table(SDL_Renderer* renderer, int x, int y, int w, int h, const char* texturePath)
    : Obstacle(renderer, x, y, w, h) {
    SDL_Log("Attempting to load table texture from: %s", texturePath);
    texture = loadTexture(texturePath, renderer);
    if (!texture) {
        SDL_Log("Failed to load table texture from: %s", texturePath);
        SDL_Log("SDL Error: %s", SDL_GetError());
        SDL_Log("Falling back to colored rectangle");
    } else {
        SDL_Log("Successfully loaded table texture");
    }
}

void Table::render() {
    SDL_Rect destRect = {
        gridX * GRID_SIZE,
        gridY * GRID_SIZE,
        width * GRID_SIZE,
        height * GRID_SIZE
    };
    
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        
        // Debug outline
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &destRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brown color
        SDL_RenderFillRect(renderer, &destRect);
    }
}

bool Table::canPlaceItem() const {
    // Add logic for checking if items can be placed on the table
    return true;  // Default implementation
}