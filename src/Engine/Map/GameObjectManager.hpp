#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "../Objects/GameObject.hpp"
#include "../Objects/Tile.hpp"
#include <vector>

namespace Xenia {
  class GameObjectManager {
    public:
      // FIX: parameters changed from const ref to pointer, matching the .cpp and the stored type
      static void addGameObject(Xenia::GameObject* go);
      static void addTile(Xenia::Tile* tile);

      // FIX: return const reference to avoid copying the vector on every call
      static const std::vector<GameObject*>& getGameObjects() { return objects; }
      static const std::vector<Tile*>& getTiles() { return tiles; }
    private:
      static std::vector<GameObject*> objects;
      static std::vector<Tile*> tiles;
  };
}

#endif
