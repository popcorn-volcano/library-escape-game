//
// Created by Liu KT on 2024/9/9.
//

#include "../include/engine.h"
#include "../include/settings.h"

// Define global variables
SDL_Window* mywindow = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* background = nullptr;
menu* MenuPage = nullptr;
playground* PlayPage = nullptr;
settings* SettingsPage = nullptr;
int PAGE_ID = MENUID;
int GameState = MENUID;
int game_is_running = FALSE;
int last_frame_time = 0;
float delta_time = 0;

int initialize_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Init Error: %s\n", SDL_GetError());
        return FALSE;
    }

    // Create a fullscreen window
    mywindow = SDL_CreateWindow("Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
         675 * 2, 386 * 2,  // Doubled resolution while maintaining ratio
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP  // Add fullscreen flag
    );

    if (!mywindow) {
        fprintf(stderr, "Window Error: %s\n", SDL_GetError());
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return FALSE;
    }

    // Create renderer with proper scaling
    renderer = SDL_CreateRenderer(mywindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer Error: %s\n", SDL_GetError());
        SDL_Log("Renderer Error: %s\n", SDL_GetError());
        return FALSE;
    }

    // Set logical size to maintain aspect ratio
    SDL_RenderSetLogicalSize(renderer, 675 * 2, 386 * 2);

    // Rest of your initialization code...
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF Error: %s\n", SDL_GetError());
        SDL_Log("TTF Error: %s\n", SDL_GetError());
        return FALSE;
    }
    TTF_Font* font = TTF_OpenFont("../myFont/arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Font Error: %s\n", TTF_GetError());
        SDL_Log("Font Error: %s\n", TTF_GetError());
        return FALSE;
    }
    SDL_Log("Engine Fine!");
    return TRUE;
}

void setup() {
    SDL_Log("Setting up the game...");

    MenuPage = new menu(MENU_BACKGROUND, renderer);
    if (!MenuPage) {
        SDL_Log("Failed to create MenuPage!");
        return;
    }
    SDL_Log("MenuPage created.");

    PlayPage = new playground(PLAYGROUND_BACKGROUND, renderer, "Player1");
    if (!PlayPage) {
        SDL_Log("Failed to create PlayPage!");
        return;
    }
    SDL_Log("PlayPage created.");

    SettingsPage = new settings(SETTINGS_BACKGROUND, renderer);
    if (!SettingsPage) {
        SDL_Log("Failed to create SettingsPage!");
        return;
    }
    SDL_Log("SettingsPage created.");

    background = loadTexture("../imgs/background2.jpg", renderer);
    if (!background) {
        SDL_Log("Failed to load background image!");
    } else {
        SDL_Log("Background image loaded.");
    }
}

// 在迴圈裡
void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game_is_running = FALSE;
        } else if (PAGE_ID == MENUID) {
            int result = MenuPage->process_input(&event);
            if (result == PLAYGROUNDID) {
                // Start new game when moving from menu to playground
                if (PlayPage) {
                    delete PlayPage;
                }
                PlayPage = new playground(PLAYGROUND_BACKGROUND, renderer, "Player1");
            }
            PAGE_ID = result;
            GameState = result;
        } else if (PAGE_ID == PLAYGROUNDID) {
            PAGE_ID = PlayPage->process_input(&event);
            GameState = PAGE_ID;
        }
    }
}

int update() {
    if(!game_is_running) return FALSE;
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();
    if (PAGE_ID == MENUID) {
        return MenuPage->update();
    }else if (PAGE_ID == PLAYGROUNDID) {
        return PlayPage->update();
    }
    return game_is_running;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch(PAGE_ID) {
        case MENUID:
            MenuPage->render();
            break;
        case PLAYGROUNDID:
            PlayPage->render();  // Now works without parameter
            break;
        case SETTINGSID:
            SettingsPage->render();
            break;
    }
    
    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_Log("Engine TURN OFF!");
    
    // Clean up pages
    if (MenuPage) delete MenuPage;
    if (PlayPage) delete PlayPage;
    if (SettingsPage) delete SettingsPage;
    if (background) SDL_DestroyTexture(background);
    
    // Clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(mywindow);
    SDL_Quit();
}