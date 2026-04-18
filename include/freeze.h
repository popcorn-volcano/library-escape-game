#pragma once
#include "collectible.h"
#include "playground.h"
class Freeze : public Collectible {
private:
    float freezeDuration;
    bool m_active;  // Renamed from isActive to m_active
    float powerupTimer;

protected:
    void renderGlow() override;  // Only declare once

public:
    Freeze(SDL_Renderer* renderer, int x, int y);
    void collect() override;
    void update(float deltaTime) override;
    void render() override;  // Add render declaration
    bool isActive() const { return m_active; }  // Uses m_active
    float getFreezeTimeRemaining() const { return powerupTimer; }
};