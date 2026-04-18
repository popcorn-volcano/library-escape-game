#ifndef VISION_H
#define VISION_H

#include <SDL.h>
#include "character.h"

class Vision
{
private:
    int range;
    float angle; // Angle in radians
    const Character *owner;
    float direction; // Direction in radians

public:
    Vision(const Character *owner, int range, float angle);
    bool isCharacterInVision(const Character *character) const;
    void render(SDL_Renderer *renderer) const;
    void setDirection(float direction); // Add this method
};

#endif
