#include "Player.hpp"
#include "../Debug/Logger.hpp"
#include <string>
#include <SDL3/SDL_gamepad.h>

int ct = 0;

Player::Player(std::string pathToPlrTex, int x, int y, SDL_Renderer* ren, bool comp){
    this->x = x;
    this->y = y;
    this->img = nullptr;
    this->tmpRnd = ren;
    this->sprCtr = 0;

    this->rect = { (float)x, (float)y, 48.0f, 48.0f };

    // lowkey kinda redundant bc like, have you seen the function for this??
    if(!loadTextures("res/img/plr/", "player", true)) {
      Xenia::Logger::logError("!!! COULD NOT LOAD TEXTURES !!!", 1);
    }

    this->img = down1; // default sprite
}

Player::~Player() {}


void Player::update() {
    rect.x = this->x;
    rect.y = this->y;

    const bool* keystate = SDL_GetKeyboardState(NULL);

    bool moving =
        keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP] ||
        keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN] ||
        keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT] ||
        keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT];

    if (moving) {
        sprCtr++;
        if (sprCtr > 20) sprCtr = 1;
    }

    if(keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        if(sprCtr >= 10)
            this->img = this->up1;
        else
            this->img = this->up2;
        this->y -= 4;
    }

    if(keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        if(sprCtr >= 10)
            this->img = this->down1;
        else
            this->img = this->down2;
        this->y += 4;
    }
    if(keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
        if(sprCtr >= 10)
            this->img = this->left1;
        else
            this->img = this->left2;
        this->x -= 4;
    }
    if(keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
        if(sprCtr >= 10)
            this->img = this->right1;
        else
            this->img = this->right2;
        this->x += 4;
    }
}


void Player::draw() {
    if (!this->img || !this->tmpRnd) return;
    SDL_RenderTexture(this->tmpRnd, this->img, NULL, &rect);
}
