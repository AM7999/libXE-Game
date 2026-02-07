#include "Player.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>

#include "../Filesystem/AssetManager.hpp"

SDL_FRect destR_p;
int ct = 0;

Player::Player(std::string pathToPlrTex, int x, int y, SDL_Renderer* ren, bool comp) {
    this->img = img;
    this->x = x;
    this->y = y;

    this->tmpRnd = ren;

    destR_p.h = 16*3;
    destR_p.w = 16*3;
    destR_p.x = this->x;
    destR_p.y = this->y;

    if(comp) {
        this->up1 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_up_1.png", ren);
        this->up2 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_up_2.png", ren);

        this->down1 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_down_1.png", ren);
        this->down2 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_down_2.png", ren);

        this->left1 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_left_1.png", ren);
        this->left2 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_left_2.png", ren);

        this->right1 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_right_1.png", ren);
        this->right2 = AssetManager::loadCompressedTexture(pathToPlrTex+"player_right_2.png", ren);
    }
    else if(!comp) {
        this->up1 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_up_1.png", ren);
        this->up2 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_up_2.png", ren);

        this->down1 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_down_1.png", ren);
        this->down2 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_down_2.png", ren);

        this->left1 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_left_1.png", ren);
        this->left2 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_left_2.png", ren);

        this->right1 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_right_1.png", ren);
        this->right2 = AssetManager::loadUncompressedTexture(pathToPlrTex+"player_right_2.png", ren);
    }


    this->img = up1;
}

void Player::update() {
    destR_p.x = this->x;
    destR_p.y = this->y;

    // Input handling moved to Game::handleEvents()
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

Player::~Player() {
}

void Player::draw() {
    if (!this->img || !this->tmpRnd) return;
    SDL_RenderTexture(this->tmpRnd, this->img, NULL, &destR_p);
}
