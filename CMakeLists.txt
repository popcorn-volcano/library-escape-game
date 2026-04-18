cmake_minimum_required(VERSION 3.29)
project(my_Final_project_game)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 20)

set(SDL2_PATH "SDL_lib/SDL2-2.30.7/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL_lib/SDL2_image-2.8.2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL_lib/SDL2_ttf-2.22.0/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "SDL_lib/SDL2_mixer-2.8.0/x86_64-w64-mingw32")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_PATH}/include ${SDL2_IMAGE_PATH}/include ${SDL2_TTF_PATH}/include ${SDL2_MIXER_PATH}/include)
#link_directories(${SDL2_IMAGE_PATH}/lib/x64)

add_executable(my_Final_project_game
    main.cpp
    src/engine.cpp
    src/menu.cpp
    src/playground.cpp
    src/settings.cpp
    src/character.cpp
    src/tool.cpp
    src/obstacle.cpp    
    src/table.cpp      
    src/bookshelf.cpp
    src/collectible.cpp        
    src/collectible_manager.cpp
    src/note.cpp              
    src/coffee.cpp            
    src/freeze.cpp            
    src/pastexam.cpp
    src/game_management.cpp    # Add this line
    src/score_page.cpp         # Add this line
    src/enemy.cpp              # Add this line
    src/vision.cpp             # Add this line
    src/audio_manager.cpp      # Add this line
)

target_sources(${PROJECT_NAME} PRIVATE
    src/freeze.cpp
    src/collectible_manager.cpp
    # ... other source files
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})

set(SDL2_DLLS
        "${CMAKE_SOURCE_DIR}/bin/SDL2.dll"
        "${CMAKE_SOURCE_DIR}/bin/SDL2_image.dll"
        "${CMAKE_SOURCE_DIR}/bin/SDL2_mixer.dll"
        "${CMAKE_SOURCE_DIR}/bin/SDL2_ttf.dll"
)

# Copy the DLLs to the build directory
add_custom_command(
        TARGET my_Final_project_game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_DLLS}
        $<TARGET_FILE_DIR:my_Final_project_game>
        COMMENT "Copying SDL2 DLLs to the build directory"
)