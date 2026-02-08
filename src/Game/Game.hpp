#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL3/SDL.h>

#include "../Engine/Debug/Logger.hpp"
#include "../Engine/Objects/GameObject.hpp"
#include "../Engine/Save/Config.hpp"
#include "Player.hpp"
#include "SDL3/SDL_video.h"

class Game {
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, bool debug);
        void update();
        void draw();

        void handleEvents();

        bool running() {return isRunning;}
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        SDL_Texture* grass;

        std::vector<Xenia::GameObject*> gameObjects;

        Player* player;

        Xenia::GameObject* gObj;

        bool isRunning;

        Xenia::GameObject* obj;

        bool debug;
};

#endif