#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Tile.hpp"

class Map {
    public:
        Map(SDL_Renderer* renderer);
        ~Map();

        void getTileImages();

        void loadMap(const char* filename);

        void draw(SDL_Renderer* renderer);
    private:
        SDL_Renderer* mapRend;
        Tile tSet[3];
};

#endif
