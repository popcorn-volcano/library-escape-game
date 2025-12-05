// include/bookshelf.h
#ifndef BOOKSHELF_H
#define BOOKSHELF_H

#include "obstacle.h"

class Bookshelf : public Obstacle {
private:
    int bookCount;
    
public:
    Bookshelf(SDL_Renderer* renderer, int x, int y, int w, int h);
    void render() override;
    // Bookshelf specific functionality
    bool addBook();
    bool removeBook();
    int getBookCount() const;
};

#endif