// src/obstacle.cpp
#include "../include/obstacle.h"
#include "../include/constants.h"

Obstacle::Obstacle(SDL_Renderer* renderer, int x, int y, int w, int h)
    : renderer(renderer), gridX(x), gridY(y), width(w), height(h) {
    texture = nullptr;
}

Obstacle::~Obstacle() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

bool Obstacle::isBlocking(int x, int y) const {
    return (x >= gridX && x < gridX + width &&
            y >= gridY && y < gridY + height);
}