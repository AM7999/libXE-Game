#ifndef TILE_H
#define TILE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

namespace Xenia {
    class Tile {
        public:
            // FIX: SDL_Texture is an opaque struct and cannot be passed by value — must be a pointer
            Tile(std::string name, SDL_Texture* img, bool colidable);

            std::string getName() { return name; }
            SDL_Texture* getTexture() { return img; }
            bool isColidable() { return colidable; }
        private:
            std::string name;
            SDL_Texture* img;
            bool colidable;
    };
}

#endif
