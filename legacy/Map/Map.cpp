#include "Map.hpp"
#include "Tile.hpp"

#include "../Filesystem/AssetManager.hpp"

#include "../Debug/Logger.hpp"

int mapTileNum[12][16];

Map::Map(SDL_Renderer* renderer) : tSet{Tile(nullptr), Tile(nullptr), Tile(nullptr)} {
    this->mapRend = renderer;
}

Map::~Map() {

}

void Map::getTileImages() {
    tSet[0] = Tile(AssetManager::loadCompressedTexture("res/tile/grass.png", this->mapRend));
    tSet[1] = Tile(AssetManager::loadCompressedTexture("res/tile/water.png", this->mapRend));
    tSet[2] = Tile(AssetManager::loadCompressedTexture("res/tile/stone.png", this->mapRend));
}

void Map::loadMap(const char* filename) {
    std::string map = AssetManager::loadCompressedData(filename);
    int row = 0;
    int col = 0;

    while(col < 16 && row < 12) {
        char c = map.at((row*32) + col*2);
        int num = c - '0';
        mapTileNum[row][col] = num;
        col++;
        if(col == 16) {
            col = 0;
            row++;
        }
    }
}

void Map::draw(SDL_Renderer* renderer) {
    SDL_FRect srcR, destR;
    srcR.x = 0;
    srcR.y = 0;
    srcR.w = 16;
    srcR.h = 16;

    destR.w = 16*3;
    destR.h = 16*3;

    for(int row = 0; row < 12; row++) {
        for(int col = 0; col < 16; col++) {
            destR.x = col * 16 * 3;
            destR.y = row * 16 * 3;

            int tileNum = mapTileNum[row][col];

            SDL_RenderTexture(renderer, tSet[tileNum].getTex(), &srcR, &destR);
        }
    }
}
