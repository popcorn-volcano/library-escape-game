#pragma once
#include "collectible.h"
#include "collectible_manager.h"
#include "playground.h"
class Coffee : public Collectible {
private:
    float speedBoostDuration;
    float speedBoostMultiplier;
    bool isActive;
    float powerupTimer;
    float glowTime;  // Add this line

protected:
    void renderGlow() override;

public:
    Coffee(SDL_Renderer* renderer, int x, int y);
    void collect() override;
    void update(float deltaTime) override;
    void render() override;
    bool isBoostActive() const { return isActive; }
    float getBoostMultiplier() const { return speedBoostMultiplier; }
    float getBoostDuration() const { return speedBoostDuration; }
};