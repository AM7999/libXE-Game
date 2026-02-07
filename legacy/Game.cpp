#include "Game.hpp"
#include "Debug/Logger.hpp"

#include "Entity/Player.hpp"
#include "Filesystem/AssetManager.hpp"

#include "Map/Map.hpp"

#include "Save/Config.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

SDL_Texture *plrTex;
SDL_FRect srcR, destR;

bool debug = false;

// lowkey, should i just move the init to the constructor..?
Game::Game() {
    Xenia::Logger::logMessage("Constructed");
}
Game::~Game() {
    Xenia::Logger::logMessage("Deconstructed");
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, bool debug) {
    SDL_WindowFlags flags = 0;
    if(fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    this->debug = debug;

    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        Xenia::Logger::logMessage("Subsystem init!");
        window = SDL_CreateWindow(title, width, height, flags);
        if(window) {
            Xenia::Logger::logMessage("Window created");
            SDL_SetWindowPosition(window, xpos, ypos);
        }
        renderer = SDL_CreateRenderer(window, NULL);
        if(renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            Xenia::Logger::logMessage("Renderer created");
        }

        isRunning = true;
    }
    else {
        isRunning = false;
    }

    // if(debug)
    //     plrTex = AssetManager::loadUncompressedTexture("res/img/guy.png", renderer);
    // if(!debug)
    //     plrTex = AssetManager::loadCompressedTexture("res/img/guy.png", renderer);

    p = new Player("res/img/plr/", 10, 10, renderer, false);

    //map->loadMap("res/map/TestGen.txt");
}
//hi angel-milan
void Game::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
        }
    }
}

void Game::update() {
    p->update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    p->draw();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    Xenia::Logger::logMessage("Stopped.");
}
