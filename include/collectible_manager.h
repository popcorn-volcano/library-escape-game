#pragma once
#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <set>  // Add this
#include "constants.h"
#include "powerup_icon.h"
#include "playground.h"

// Add SDL_Point comparison struct
struct SDLPointCompare {
    bool operator()(const SDL_Point& a, const SDL_Point& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

class CollectibleManager {
private:
    static std::vector<SDL_Point> spawnPoints;
    static Mix_Chunk* collectSound;
    static Mix_Chunk* powerupSound;
    static SDL_Texture* particleTexture;
    static bool initialized;
    static playground* currentPlayground;  // Add static pointer to current playground
    static void updateIcon(PowerupIcon& icon, bool active, float duration);
    static std::vector<bool> queuedNotes;  // Add this to track queued notes

public:
    static void initialize();
    static SDL_Point getRandomSpawnPoint();
    static void playCollectSound();
    static void playPowerupSound();
    static void createParticles(SDL_Renderer* renderer, int x, int y);
    static void cleanup();
    static void setCoffeeIconActive(bool active, float duration);
    static void setFreezeIconActive(bool active, float duration);
    
    // Add these methods
    static void setCurrentPlayground(playground* pg) { currentPlayground = pg; }
    static playground* getCurrentPlayground() { return currentPlayground; }
    static void queueNoteSpawn();  // Add this declaration
};