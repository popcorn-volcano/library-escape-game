#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "constants.h"

// Define the Button struct before the settings class
struct Button {
    SDL_Rect rect;
    const char* text;
    SDL_Texture* textTexture;
    bool isHovered;
    
    Button() : text(nullptr), textTexture(nullptr), isHovered(false) {}
};

class settings {
private:
    SDL_Renderer* renderer;
    SDL_Texture* background;
    TTF_Font* font;
    std::vector<Button> buttons;
    SDL_Color buttonColor;
    SDL_Color buttonHoverColor;

    struct CharacterOption {
        SDL_Texture* texture;
        SDL_Rect rect;
        bool isSelected;
        //new add
        SDL_Texture* usernameTexture;  // 用戶名材質
        SDL_Rect usernameRect;         // 用戶名位置
        //new add ends here
    };
    std::vector<CharacterOption> characterOptions;
    int selectedCharacter = 0; // Track currently selected character

//new add
    SDL_Texture* titleTexture = nullptr;
    SDL_Rect titleRect;
    SDL_Texture* rulesTexture = nullptr;  // 規則文字材質
    SDL_Rect rulesRect;                   // 規則文字位置和大小
//new add ends here

public:
    settings(const std::string& path, SDL_Renderer* renderer);
    ~settings();
    
    int process_input(SDL_Event* event);
    void render();
    int update();

    SDL_Texture* getSelectedCharacterTexture() const {
        if (!characterOptions.empty() && selectedCharacter >= 0 && 
            selectedCharacter < characterOptions.size()) {
            return characterOptions[selectedCharacter].texture;
        }
        return nullptr;
    }
    SDL_Rect getSelectedCharacterRect() const {
        if (!characterOptions.empty() && selectedCharacter >= 0 && 
            selectedCharacter < characterOptions.size()) {
            return characterOptions[selectedCharacter].rect;
        }
        return SDL_Rect{0, 0, 0, 0};
    }
//new function
    void initializeRules();
//new function ends here

    int getSelectedCharacterIndex() const {
        return selectedCharacter;
    }

private:
    void createButton(const char* text, int x, int y, int width, int height);
    void handleMouseMotion(int x, int y);
    int handleMouseClick(int x, int y);
};

#endif