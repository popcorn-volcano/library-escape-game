#ifndef engine
#define engine

#include <iostream>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include "tool.h"
#include "menu.h"
#include "playground.h"
#include "settings.h"

// Declare all global variables as extern
extern SDL_Window *mywindow;
extern SDL_Renderer *renderer;
extern SDL_Texture *background;
extern menu *MenuPage;
extern playground *PlayPage;
extern settings *SettingsPage;
extern int PAGE_ID;
extern int GameState;
extern int game_is_running;
extern int last_frame_time;
extern float delta_time;

int initialize_window();
void setup();
void process_input();
int update();
void render();
void destroy_window();

#endif
