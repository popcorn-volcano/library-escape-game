#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL_mixer.h>
#include <SDL.h>
#include <string>
#include <map>

class AudioManager
{
public:
    static void initialize();
    static void loadMusic(const std::string &filePath);
    static void loadSoundEffect(const std::string &filePath, const std::string &soundId);
    static void playMusic();
    static void playSoundEffect(const std::string &soundId);
    static void cleanup();

private:
    static Mix_Music *bgm;
    static std::map<std::string, Mix_Chunk*> soundEffects;  // All sound effects stored in map
};

#endif
