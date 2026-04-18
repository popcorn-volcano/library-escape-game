// include/obstacle.h
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
protected:
    int gridX;
    int gridY;
    int width;
    int height;
    SDL_Texture* texture;
    SDL_Renderer* renderer;

public:
    Obstacle(SDL_Renderer* renderer, int x, int y, int w, int h);
    virtual ~Obstacle();
    
    // Common functionality
    virtual void render() = 0;
    bool isBlocking(int x, int y) const;
    
    // Getters
    int getX() const { return gridX; }
    int getY() const { return gridY; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif