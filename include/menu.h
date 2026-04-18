//
// Created by Liu KT on 2024/11/20.
//

#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include "SDL2/SDL_ttf.h"
#include "tool.h"

class menu {
private:
    struct Button {
        SDL_Rect rect;      // Button position and size
        SDL_Texture* texture; // Button texture
        std::string text;    // Button text
        bool isHovered;     // Mouse hover state
    };
    
    std::vector<Button> buttons;    // Store all buttons
    SDL_Color buttonColor;          // Default button color
    SDL_Color buttonHoverColor;     // Button hover color
    TTF_Font* font;                 // Font for button text

    SDL_Renderer* renderer;         // Renderer

    Uint8 backgroundAlpha = 0;  // Start fully transparent
    float fadeSpeed = 255.0f;    // Fade in speed (alpha units per second)
    float currentAlpha = 0.0f;   // Current alpha as float for smooth transitions

public:
    SDL_Texture* background;
    
    // Button management methods
    void createButton(const char* text, int x, int y, int width, int height);
    void handleMouseMotion(int x, int y);
    int handleMouseClick(int x, int y);
    void renderButtons();
    
    // Core menu methods 
    int update();
    int process_input(SDL_Event* event);
    void render();  // Changed to not require renderer parameter since we store it as member
    
    // Constructor/Destructor
    menu(const std::string& path, SDL_Renderer* renderer);
    ~menu();
};

#endif //MENU_H
