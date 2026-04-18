#include "../include/audio_manager.h"

Mix_Music *AudioManager::bgm = nullptr;
std::map<std::string, Mix_Chunk*> AudioManager::soundEffects;

void AudioManager::initialize() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    } else {
        SDL_Log("SDL_mixer initialized successfully.");
    }
}

void AudioManager::loadSoundEffect(const std::string &filePath, const std::string &soundId) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (sound == nullptr) {
        SDL_Log("Failed to load sound effect from %s! SDL_mixer Error: %s\n", 
                filePath.c_str(), Mix_GetError());
    } else {
        // Free existing sound if any
        if (soundEffects.count(soundId) > 0) {
            Mix_FreeChunk(soundEffects[soundId]);
        }
        soundEffects[soundId] = sound;
    }
}

void AudioManager::loadMusic(const std::string &filePath)
{
    bgm = Mix_LoadMUS(filePath.c_str()); // Load the specified audio file
    if (bgm == nullptr)
    {
        SDL_Log("Failed to load background music from %s! SDL_mixer Error: %s\n", filePath.c_str(), Mix_GetError());
    }
    else
    {
        SDL_Log("Background music loaded from %s.", filePath.c_str());
    }
}

void AudioManager::playMusic()
{
    if (Mix_PlayingMusic() == 0)
    {
        if (Mix_PlayMusic(bgm, -1) == -1)
        {
            SDL_Log("Failed to play background music! SDL_mixer Error: %s\n", Mix_GetError());
        }
        else
        {
            SDL_Log("Background music is now playing.");
        }
    }
    else
    {
        SDL_Log("Background music is already playing.");
    }
}

void AudioManager::playSoundEffect(const std::string &soundId) {
    if (soundEffects.find(soundId) != soundEffects.end()) {
        Mix_PlayChannel(-1, soundEffects[soundId], 0);
    }
}

void AudioManager::cleanup()
{
    if (bgm != nullptr)
    {
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
    
    // Clean up all sound effects
    for (auto& [id, chunk] : soundEffects) {
        if (chunk != nullptr) {
            Mix_FreeChunk(chunk);
            chunk = nullptr;
        }
    }
    soundEffects.clear();
    Mix_CloseAudio();
}
