#  Library Escape Game

A 2D arcade-style game built from scratch using SDL2 in C++. Navigate through a library environment, dodge obstacles, and complete objectives before time runs out!

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL2-2.30.7-green.svg)
![CMake](https://img.shields.io/badge/CMake-Build-red.svg)

##  Features

- **Custom Game Engine** - Built from scratch with proper game loop architecture
- **Multiple Game States** - Menu, Settings, and Gameplay screens with smooth transitions
- **Collision Detection** - Physics-based collision system for obstacles (tables, bookshelves)
- **Character Controller** - Smooth 2D character movement with sprite animations
- **Audio System** - Background music and sound effects using SDL2_mixer
- **UI System** - Custom rendered menus and HUD using SDL2_ttf

##  Tech Stack

| Technology | Purpose |
|------------|---------|
| C++17 | Core programming language |
| SDL2 | Window management & rendering |
| SDL2_image | Texture loading (PNG, JPG) |
| SDL2_ttf | Font rendering |
| SDL2_mixer | Audio playback |
| CMake | Cross-platform build system |

##  Project Structure

```
 main.cpp           # Entry point with game loop
 src/
    engine.cpp     # Core engine (init, update, render, cleanup)
    character.cpp  # Player controller and animations
    playground.cpp # Main gameplay logic
    menu.cpp       # Main menu implementation
    settings.cpp   # Settings page
    enemy.cpp      # Enemy AI behavior
    collectible.cpp# Collectible items logic
    obstacle.cpp   # Obstacle collision handling
 include/           # Header files
 imgs/              # Game textures and sprites
 fonts/             # TTF font files
 audio/             # Sound effects and music
 SDL_lib/           # SDL2 library files
```

##  Building & Running

### Prerequisites
- CMake 3.10+
- SDL2, SDL2_image, SDL2_ttf, SDL2_mixer

### Build
```bash
mkdir build && cd build
cmake ..
make
./my_Final_project_game
```

##  Controls

| Key | Action |
|-----|--------|
| W/ | Move Up |
| S/ | Move Down |
| A/ | Move Left |
| D/ | Move Right |
| ESC | Pause/Menu |

##  Screenshots

*Screenshots coming soon*

##  What I Learned

- Game loop architecture with delta time for frame-rate independent movement
- 2D rendering pipeline and texture management
- Object-oriented design patterns for game entities
- Memory management and resource cleanup in C++
- Cross-platform build configuration with CMake

##  License

MIT License - Feel free to use this code for learning purposes!
