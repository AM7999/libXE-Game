#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Map/Map.hpp"

#include "Entity/Player.hpp"

class TileManager; // forward declaration

// Class and method declerations
class Game {
    public:
        // constructor
        Game();
        // deconstructor
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, bool debug);

        void handleEvents();

        void update();
        void render();
        void clean();

        bool running() { return isRunning; }

        // Public access for tile manager and other systems
        SDL_Renderer *renderer;

    private:
        int cnt;

        bool isRunning;

        SDL_Window *window;

        Player *p;

        Map *map;

        bool debug;
        bool vsync;
};

#endif
