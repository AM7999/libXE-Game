#include "Game.hpp"
#include "Macros.h"

// #include "Sound/"

#include "Filesystem/AssetManager.hpp"

#include "Save/Config.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

#include <zip.h>

#include <SDL3/SDL_video.h>

#include "Debug/Logger.hpp"

// main game object
Game *g = nullptr;

std::string params[] = {
    "--debug",
    "--sndTest"
};

int main(int argc, char* argv[]) {
    if(argc > 1 && params[1] == argv[1]) {

    }

    // constructing game so its no longer nullptr
    g = new Game();

    const int targetFps = 60;
    const int frameDelay = 1000/targetFps;

    Uint32 frameStart;
    int frameTime;

    // simple config check
    if(!std::filesystem::exists("config.ini")) {
        Xenia::Logger::logWarning("config missing! copying from resources");
        std::ofstream config;
        config.open("config.ini");
        config << AssetManager::loadCompressedData("res/data/base_cfg.ini");
        config.close();
        Xenia::Logger::logMessage("config created!");

        // exit on first run
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notice", "A config has been created in the game folder. Please Re-open the game", NULL);

        return 1;
    }

    // running the initialization function
    g->init("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DESKTOP_EQUATION_W, DESKTOP_EQUATION_H, Config::fullscreen(), Config::debug());

    // Game loop
    while(g->running()) {

        frameStart = SDL_GetTicks();

        g->handleEvents();

        // added here to stop and clean before closing
        // finally closes right away though
        if(!g->running())
            break;

        g->update();
        g->render();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    g->clean();

    return 0;
}
