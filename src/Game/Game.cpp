#include "Game.hpp"
#include <string>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_internal.h>

#include "../include/json.hpp"

#include "../Engine/Objects/GameObject.hpp"
#include "../Engine/AssetManager/AssetManager.hpp"
#include "../Engine/Map/GameObjectManager.hpp"

Game::Game() {
    Xenia::Logger::logMessage("Game Started.");
}

Game::~Game() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    Xenia::Logger::logMessage("Game Stopped.");
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, bool debug) {
    SDL_WindowFlags flags = 0;
    if(fullscreen) {flags = SDL_WINDOW_FULLSCREEN;}

    if(SDL_Init(SDL_INIT_VIDEO)) {
        Xenia::Logger::logMessage("SDL_Init");

        window = SDL_CreateWindow(title, width, height, flags);
        if(!window) {
            Xenia::Logger::logError(std::string("Game.cpp: 20, SDL_CreateWindow Failed! ") + SDL_GetError(), 1);
        }
        SDL_SetWindowPosition(window, xpos,ypos);
        SDL_SetWindowResizable(window, false);
        renderer = SDL_CreateRenderer(window, NULL);
        if(!renderer) {
            Xenia::Logger::logError(std::string("Game.cpp: 27, SDL_CreateRenderer Failed! ") + SDL_GetError(), 1);
        }

        SDL_SetRenderDrawColor(renderer, 255,255,0,255);

        isRunning = true;

        player = new Player("res/img/plr/", 48, 48, renderer, true);

        grass = Xenia::AssetManager::loadCompressedTexture("res/tile/grass.png", renderer);

        nlohmann::json j = nlohmann::json::parse(Xenia::AssetManager::loadCompressedData("res/data/objs.json"));

        for (const auto& item : j["Objects"]) {
            objects.emplace_back(item);
        }

        auto* gameObj = new Xenia::GameObject(48,48, Xenia::AssetManager::loadCompressedTexture("res/img/guy.png", renderer), renderer, "guy", "xenia" ,false);

        gameObjects.push_back(gameObj);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);
    }
    else {
        Xenia::Logger::logWarning(std::string("Game.cpp: 38, SDL_Init Failed! ") + SDL_GetError());
        isRunning = false;
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        switch(event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            default:
                break;
        }
    }
}

void Game::update() {
    player->update();
}

void Game::draw() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    bool showEx = false;

    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Show example window.")) { showEx = true; }
            if (ImGui::MenuItem("Attempt Save.", "Ctrl+S")) { Xenia::Logger::logWarning("Not implemented"); }
            if (ImGui::MenuItem("Quit.", "Ctrl+Q")) { isRunning = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("plrY: %d", player->getY());
    ImGui::Text("plrX: %d", player->getX());

    ImGui::End();

    static int selectedIndex = -1;

    ImGui::Begin("Object View");
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        std::string label = gameObjects[i]->getSpace() + ":" + gameObjects[i]->getName();

        if (ImGui::Selectable(label.c_str(), selectedIndex == (int)i)) {
            selectedIndex = i;  // Update selection
        }
        // Show image of selected object (outside the loop, every frame)
        if (selectedIndex >= 0 && selectedIndex < (int)gameObjects.size()) {

            ImGui::Separator();
            ImGui::Text("Selected Object:");
            ImGui::Image((ImTextureID)(intptr_t)gameObjects[selectedIndex]->getImg(),
                         ImVec2(48.0f, 48.0f));
            ImGui::Text("Position: %.1f, %.1f\nInteractable: %s", gameObjects[selectedIndex]->getX(), gameObjects[selectedIndex]->getY(), gameObjects[selectedIndex]->isInteractable() ? "Yes" : "No");
        }
        ImGui::End();

        SDL_RenderClear(renderer);

        for (size_t i = 0; i < gameObjects.size(); i++) {
            gameObjects[i]->draw();
        }

        player->draw();

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }
}