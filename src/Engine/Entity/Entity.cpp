#include "Entity.hpp"

bool Xenia::Entity::loadTextures(std::string pathToTex, std::string thing, bool compression) {
  up1 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_up_1.png", tmpRnd);
  up2 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_up_2.png", tmpRnd);

  down1 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_down_1.png", tmpRnd);
  down2 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_down_2.png", tmpRnd);

  left1 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_left_1.png", tmpRnd);
  left2 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_left_2.png", tmpRnd);

  right1 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_right_1.png", tmpRnd);
  right2 = Xenia::AssetManager::loadCompressedTexture(pathToTex + thing + "_right_2.png", tmpRnd);

  // oh my god there has to be a better way to do this
  if (up1 == nullptr || up2 == nullptr || down1 == nullptr ||
      down2 == nullptr || left1 == nullptr || left2 == nullptr || right1 == nullptr ||
      right2 == nullptr) {
    return false;
  }

    return true;
}
