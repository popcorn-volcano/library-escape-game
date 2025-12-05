//
// Created by Liu KT on 2024/11/20.
//
#include "../include/tool.h"
#include <SDL2/SDL_image.h>
#include <iostream>

SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load image %s: %s", filePath.c_str(), IMG_GetError());
        return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create texture from %s: %s", filePath.c_str(), SDL_GetError());
        return nullptr;
    }
    return texture;
}



