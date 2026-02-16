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
        Xenia::Logger::logMessage("Config created!");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Notice", "A config has been created in the game folder. Please Re-open the game", NULL);
        return 1;
    }

    g = new Game();

    const int targetFps = 60;
    const int frameDelay = 1000/targetFps;

    Uint32 frameStart;
    int frameTime;

    g->init("GAME", 100,100, 16*16*3, 16*12*3, Xenia::Config::getBoolValue("Game", "Fullscreen"), Xenia::Config::getBoolValue("Debug-Ops", "Debug"));

    // Main Game Loop
    while(g->running()) {
        frameStart = SDL_GetTicks();

        g->update();
        g->draw();

        g->handleEvents();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    delete g;

    return 0;
}
