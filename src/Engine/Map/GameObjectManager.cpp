#include "GameObjectManager.hpp"

std::vector<Xenia::GameObject*> Xenia::GameObjectManager::objects;
std::vector<Xenia::Tile*> Xenia::GameObjectManager::tiles;

// FIX: signatures updated to match header (non-const pointer, not const ref)
void Xenia::GameObjectManager::addGameObject(Xenia::GameObject* go) {
    objects.push_back(go);
}

void Xenia::GameObjectManager::addTile(Xenia::Tile* tile) {
    tiles.push_back(tile);
}
