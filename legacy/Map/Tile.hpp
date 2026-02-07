#ifndef TILE_H
#define TILE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Tile {
    public:
        Tile(SDL_Texture* tex) { this->texture = tex; }
        SDL_Texture* getTex() { return this->texture; }
        void setCol(bool col) { this->collision = col; }
        bool getCol() { return this->collision; }
    private:
        SDL_Texture* texture;
        bool collision = false;
};

#endif
