#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_gamepad.h"

#include <string>

class Player : public Xenia::Entity {
    public:
        Player(std::string pathToPlrTex, int x, int y, SDL_Renderer* ren, bool comp);
        ~Player();
        void update();
        void draw();
    private:
        int worldX;
        int worldY;
        SDL_Gamepad* gamepad = nullptr;
        int gamepadDeadzone = 8000; // axis deadzone
};

#endif
