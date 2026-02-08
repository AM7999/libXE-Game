#ifndef ENTITY_H
#define ENTITY_H

#include "../AssetManager/AssetManager.hpp"

#include <SDL3_image/SDL_image.h>
#include <string>

namespace Xenia {
    class Entity {
        public:
            int getX() {return this->x;}
            int getY() {return this->y;}
        protected:
            bool loadTextures(std::string pathToTextures, std::string thing, bool compression);

            char direction;
            int x,y;

            SDL_Texture* img;

            SDL_Texture* up1;
            SDL_Texture* up2;

            SDL_Texture* down1;
            SDL_Texture* down2;

            SDL_Texture* left1;
            SDL_Texture* left2;

            SDL_Texture* right1;
            SDL_Texture* right2;

            int sprCtr = 0;

            SDL_FRect rect;

            SDL_Renderer* tmpRnd;
    };
}

#endif
