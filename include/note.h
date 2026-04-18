#pragma once
#include "collectible.h"

class Note : public Collectible {
private:
    int scoreValue;

protected:
    void renderGlow() override;

public:
    Note(SDL_Renderer* renderer, int x, int y);
    void collect() override;
    void render() override;  // Add render override
    void update(float deltaTime) override;  // Add update for glow animation
};