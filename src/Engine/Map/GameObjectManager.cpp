#include "GameObjectManager.hpp"

void Xenia::GameObjectManager::addGameObject(Xenia::GameObject go) {
    objects.push_back(go);
}
void Xenia::GameObjectManager::addTile(Xenia::Tile tile) {
    // TODO: actually implement tile methods
    tiles.push_back(tile);
}
