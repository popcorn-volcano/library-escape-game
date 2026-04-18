// include/bookshelf.h
#ifndef BOOKSHELF_H
#define BOOKSHELF_H

#include <SDL.h>
#include "obstacle.h"
#include "constants.h" // Add this include before using constants

class Bookshelf : public Obstacle {
public:
    Bookshelf(SDL_Renderer* renderer, int x, int y, int w, int h, const char* texturePath = BOOKSHELF_TEXTURE_2X1);
    void render() override;
};

#endif