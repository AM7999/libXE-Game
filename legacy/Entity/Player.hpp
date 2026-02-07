#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"
#include <SDL3/SDL_render.h>

class Player : public Entity {
    public:
        Player(std::string pathToPlrTex, int x, int y, SDL_Renderer* ren, bool comp);
        ~Player();
        void draw();
        void update();
        void move(int dx, int dy);
    private:
        int sprCtr = 0;

        SDL_Texture* up1;
        SDL_Texture* up2;

        SDL_Texture* down1;
        SDL_Texture* down2;

        SDL_Texture* left1;
        SDL_Texture* left2;

        SDL_Texture* right1;
        SDL_Texture* right2;
};

#endif
