//
// Created by Liu KT on 2024/11/20.
//
#include "../include/tool.h"
#include <SDL2/SDL_image.h>
#include <iostream>

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        SDL_Log("Failed to load image %s: %s", path.c_str(), IMG_GetError());
    }
    return texture;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        SDL_Log("Failed to load image %s: %s", path, IMG_GetError());
    }
    return texture;
}



