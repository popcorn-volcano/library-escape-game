#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "constants.h"

class settings {
private:
    struct Button {
        SDL_Rect rect;
        const char* text;
        bool isHovered;
        SDL_Texture* textTexture;  // Add texture for button text
    };

    SDL_Texture* background;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color buttonColor;
    SDL_Color buttonHoverColor;
    std::vector<Button> buttons;

    // Private helper methods
    void createButton(const char* text, int x, int y, int width, int height);
    void handleMouseMotion(int x, int y);
    int handleMouseClick(int x, int y);

public:
    settings(const std::string& path, SDL_Renderer* renderer);
    ~settings();
    void render();
    int process_input(SDL_Event* event);
    int update();
};

#endif