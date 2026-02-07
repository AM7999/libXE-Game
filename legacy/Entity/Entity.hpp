#ifndef ENTITY_H
#define ENTITY_H

#include <SDL3_image/SDL_image.h>
#include <string>

class Entity {
    protected:
        char direction;
        int x, y;
        SDL_Texture* img;
        SDL_Renderer* tmpRnd;
};

#endif
