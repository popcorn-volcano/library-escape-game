// include/table.h
#ifndef TABLE_H
#define TABLE_H

#include "obstacle.h"

class Table : public Obstacle {
public:
    Table(SDL_Renderer* renderer, int x, int y, int w, int h);
    void render() override;
    // Table specific functionality
    bool canPlaceItem() const;
};

#endif