#pragma once
#include <SDL.h>

struct PowerupIcon {
    SDL_Texture* texture;
    SDL_Rect position;
    bool isActive;
    float timeLeft;  // For displaying duration
};