#pragma once
#include "collectible.h"

class PastExam : public Collectible {
private:
    int scoreMultiplier;
    float duration;
    SDL_Color glowColor;
    float glowIntensity;
    float glowTime;

protected:
    void renderGlow();  // Add protected render glow method

public:
    PastExam(SDL_Renderer* renderer, int x, int y);
    void collect() override;
    void update(float deltaTime) override;
    void render() override;  // Add override for render
};