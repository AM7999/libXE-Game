#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "../Objects/GameObject.hpp"
#include "../Objects/Tile.hpp"
#include <vector>

namespace Xenia {
  class GameObjectManager {
    public:
      static void addGameObject(Xenia::GameObject);
      static void addTile(Xenia::Tile);

      static std::vector<GameObject> getGameObjects() {return objects;}
      static std::vector<Tile> getTiles() {return tiles;}
    private:
      static std::vector<GameObject> objects;
      static std::vector<Tile> tiles;
  };
}

#endif
