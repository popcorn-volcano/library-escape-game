// include/table.h
#ifndef TABLE_H
#define TABLE_H

#include <SDL.h>
#include "obstacle.h"
#include "constants.h" // Add this include before using constants

class Table : public Obstacle {
public:
    Table(SDL_Renderer* renderer, int x, int y, int w, int h, const char* texturePath = TABLE_TEXTURE_2x2book);
    void render() override;
    bool canPlaceItem() const;
};

#endif