#ifndef CONSTANTS_H
#define CONSTANTS_H

// Game states/IDs
const int MENUID = 0;
const int PLAYGROUNDID = 1;
// Add new state ID
const int SETTINGSID = 2;

// Boolean constants
const int TRUE = 1;
const int FALSE = 0;

// Add any other constants here
// Window dimensions, frame rates, etc.
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 1200
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define PLAYER_W 20
#define PLAYER_H 100
#define PLAYER_SPEED 3000
#define MENU_BACKGROUND "../imgs/background.jpg"
#define PLAYGROUND_BACKGROUND "../imgs/playground_background.jpg"
#define SETTINGS_BACKGROUND "../imgs/settings_background.jpg"
#define BOOKSHELF_TEXTURE "../imgs/bookshelf_image.jpg"
#define TABLE_TEXTURE "../imgs/table1x2.png"

// Grid size for game world
#define GRID_SIZE 40  // Size of each grid cell in pixels

#endif // STUDYSNATCHER_CONSTANTS_H

