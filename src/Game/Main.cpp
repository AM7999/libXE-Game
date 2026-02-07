#include <inttypes.h>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "Game.hpp"

Game *g = nullptr;

int main(int argc, char* argv[]) {

    if(!std::filesystem::exists("config.ini")) {
        Xenia::Logger::logWarning("Config Missing! Copying base config from resources");
        std::ofstream config;
        config.open("config.ini");
        config << Xenia::AssetManager::loadCompressedData("res/data/base_cfg.ini");
        config.close();
        if(!std::filesystem::exists("config.ini"))
            Xenia::Logger::logError("i dont even know how you ended up here ngl", 1);

        Xenia::Logger::logMessage("Config created!");

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notice", "A config has been created in the game folder. Please Re-open the game", NULL);

        return 1;
    }

    g = new Game();

    const int targetFps = 60;
    const int frameDelay = 1000/targetFps;

    Uint32 frameStart;
    int frameTime;

    #if defined(VITA)
    // this block is compiled if vita is defined
        g->init("GAME", 100,100, 16*16*3,6*11*3, false, false);
    #else
        g->init("GAME", 100,100, 16*16*3, 16*12*3, false, false);
    #endif

    while(g->running()) {
        frameStart = SDL_GetTicks();

        g->update();
        g->draw();

        g->handleEvents();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    delete g;

    return 0;
}
