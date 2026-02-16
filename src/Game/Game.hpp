#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL3/SDL.h>

#include "../Engine/Debug/Logger.hpp"
#include "../Engine/Objects/GameObject.hpp"
#include "../Engine/Save/Config.hpp"
#include "../Engine/Map/GameObjectManager.hpp"
#include "../Engine/Entity/Player.hpp"
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

        // FIX: store a const reference to avoid copying, and match the actual return type
        const std::vector<Xenia::GameObject*>& gameObjects = Xenia::GameObjectManager::getGameObjects();

        Player* player;

        bool isRunning;

        bool debug;
};

#endif
