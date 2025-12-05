//
// Created by Liu KT on 2024/11/20.
//

#include "../include/menu.h"
#include "../include/constants.h" // For MENUID, PLAYGROUNDID, TRUE, FALSE

menu::menu(const std::string& path, SDL_Renderer* renderer) {
    SDL_Log("Creating menu...");
    this->renderer = renderer;
    this->background = loadTexture(path.c_str(), renderer);
    
    // Set initial alpha for background texture
    SDL_SetTextureAlphaMod(background, backgroundAlpha);
    
    // Initialize colors
    buttonColor = {100, 100, 100, 255};       // Gray
    buttonHoverColor = {150, 150, 150, 255};  // Light gray

    // Initialize font with larger size (increased from 24 to 36)
    font = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 36);  // Increased font size
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }

    // Create menu buttons
    // Original resolution is 675x386
    // New resolution will be 1350x772 (doubled)
    int scale = 2;  // Scaling factor
    int baseWidth = 150 * scale; 
    int baseHeight = 35 * scale; 
    int centerX = (675 * scale - baseWidth) / 2;  // Center position adjusted for new resolution
    int spacing = 50 * scale;  // Reduced vertical spacing from 75 to 50

    // Moved buttons down by increasing y values
    createButton("Play Game", centerX-220, 125 * scale, baseWidth, baseHeight);     // Top button (increased from 100)
    createButton("Settings", centerX-220, 175 * scale, baseWidth, baseHeight);      // Middle button (increased from 150)
    createButton("Exit", centerX-220, 225 * scale, baseWidth, baseHeight);          // Bottom button (increased from 200)
}

void menu::createButton(const char* text, int x, int y, int width, int height) {
    Button button;
    button.rect = {x, y, width, height};
    button.text = text;
    button.isHovered = false;
    
    SDL_Color textColor = {255, 255, 255, 255};  // White text
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if (surface) {
        button.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    
    buttons.push_back(button);
}

int menu::process_input(SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            handleMouseMotion(event->motion.x, event->motion.y);
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT) {
                return handleMouseClick(event->button.x, event->button.y);
            }
            break;
    }
    return MENUID;
}

void menu::handleMouseMotion(int x, int y) {
    for (auto& button : buttons) {
        button.isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                           y >= button.rect.y && y <= button.rect.y + button.rect.h);
    }
}

int menu::handleMouseClick(int x, int y) {
    for (size_t i = 0; i < buttons.size(); i++) {
        if (x >= buttons[i].rect.x && x <= buttons[i].rect.x + buttons[i].rect.w &&
            y >= buttons[i].rect.y && y <= buttons[i].rect.y + buttons[i].rect.h) {
            
            switch(i) {
                case 0: 
                    return PLAYGROUNDID;  // Play Game
                case 1: 
                    return SETTINGSID;    // Settings page
                case 2: 
                    return FALSE;         // Exit
            }
        }
    }
    return MENUID;
}

int menu::update() {
    // Update fade effect
    if (currentAlpha < 255.0f) {
        currentAlpha += fadeSpeed * (1.0f/60.0f);  // Assuming 60 FPS
        if (currentAlpha > 255.0f) currentAlpha = 255.0f;
        backgroundAlpha = (Uint8)currentAlpha;
        SDL_SetTextureAlphaMod(background, backgroundAlpha);
    }
    return TRUE;
}

void menu::render() {  // Changed to match the header declaration
    SDL_RenderClear(renderer);
    
    // Render background with current alpha
    if (background) {
        SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
    }
    
    // Render buttons with alpha based on background alpha
    for (const auto& button : buttons) {
        // Set button alpha proportional to background alpha
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 
            button.isHovered ? buttonHoverColor.r : buttonColor.r,
            button.isHovered ? buttonHoverColor.g : buttonColor.g,
            button.isHovered ? buttonHoverColor.b : buttonColor.b,
            (Uint8)(backgroundAlpha * 0.8f));  // Slightly more transparent than background
        SDL_RenderFillRect(renderer, &button.rect);
        
        if (button.texture) {
            SDL_SetTextureAlphaMod(button.texture, backgroundAlpha);
            int textWidth, textHeight;
            SDL_QueryTexture(button.texture, nullptr, nullptr, &textWidth, &textHeight);
            SDL_Rect textRect = {
                button.rect.x + (button.rect.w - textWidth) / 2,
                button.rect.y + (button.rect.h - textHeight) / 2,
                textWidth,
                textHeight
            };
            SDL_RenderCopy(renderer, button.texture, nullptr, &textRect);
        }
    }
    
    SDL_RenderPresent(renderer);
}

menu::~menu() {
    // Cleanup resources
    if (background) {
        SDL_DestroyTexture(background);
    }
    
    for (auto& button : buttons) {
        if (button.texture) {
            SDL_DestroyTexture(button.texture);
        }
    }
    
    if (font) {
        TTF_CloseFont(font);
    }
}
