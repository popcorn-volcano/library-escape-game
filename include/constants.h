#ifndef CONSTANTS_H
#define CONSTANTS_H

// Boolean constants
const int TRUE = 1;
const int FALSE = 0;
const int EXIT = -1;

// Window dimensions, frame rates, etc.
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1320
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define PLAYER_W 20
#define PLAYER_H 100
#define PLAYER_SPEED 3000
#define MENU_BACKGROUND "../imgs/background.jpg"
#define PLAYGROUND_BACKGROUND "../imgs/playground_background.jpg"
#define SETTINGS_BACKGROUND "../imgs/playground_background.jpg"

#define BOOKSHELF_TEXTURE_2X1 "../imgs/bookshelf-2x1.png"
#define BOOKSHELF_TEXTURE_3X1 "../imgs/bookshelf-3X1.png"
#define BOOKSHELF_TEXTURE_4X1 "../imgs/bookshelf-4X1.png"
#define BOOKSHELF_TEXTURE_4X2 "../imgs/bookshelf-4X2.png"
#define BOOKSHELF_TEXTURE_UP "../imgs/bookshelf-up-4X14.png"
#define BOOKSHELF_TEXTURE_LEFT "../imgs/bookshelf-left-1x14.png"
#define BOOKSHELF_TEXTURE_RIGHT "../imgs/bookshelf-right-1x14.png"
#define DEFAULT_BOOKSHELF_TEXTURE BOOKSHELF_TEXTURE_LEFT

#define TABLE_TEXTURE_1x1bag "../imgs/table-bag-1x1.png"
#define TABLE_TEXTURE_2x2bag "../imgs/table-bag-2x2.png"
#define TABLE_TEXTURE_1x1book "../imgs/table-book-1x1.png"
#define TABLE_TEXTURE_2x2book "../imgs/table-book-2x2.png"

// Grid size for game world
#define GRID_SIZE 40  // Size of each grid cell in pixels

#endif // STUDYSNATCHER_CONSTANTS_H
