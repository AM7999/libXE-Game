#ifndef ASSMAN_H
#define ASSMAN_H

#include <SDL3_image/SDL_image.h>
#include <string>

namespace Xenia {
    class AssetManager {
        public:
            // Textures
            static SDL_Texture* loadUncompressedTexture(std::string fileName, SDL_Renderer *ren);
            static SDL_Texture* loadCompressedTexture(const std::string& pathToTexture, SDL_Renderer *ren);
            // Data
            static std::string loadUncompressedData(std::string fileName);
            static std::string loadCompressedData(std::string filename);
        private:
    };
}



#endif
