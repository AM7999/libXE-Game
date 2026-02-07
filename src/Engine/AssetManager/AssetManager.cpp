#include "AssetManager.hpp"
#include "../Debug/Logger.hpp"

#include <vector>
#include <zip.h>
#include <zipconf.h>

#include <fstream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

const char *resPack = "resources.zip";
const char *langPack = "translations.zip";

SDL_Texture *Xenia::AssetManager::loadUncompressedTexture(std::string fileName, SDL_Renderer *ren) {
  Xenia::Logger::logMessage("Load Texture from: " + fileName);
  SDL_Texture *tex = IMG_LoadTexture(ren, fileName.c_str());

  if (tex == nullptr) {
    return nullptr;
  }

  SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

  return tex;
}

SDL_Texture *
Xenia::AssetManager::loadCompressedTexture(const std::string &pathToTexture, SDL_Renderer *ren) {
  Xenia::Logger::logMessage("Load Texture from: " + pathToTexture);
  int err = 0;
  zip *archive = zip_open(resPack, 0, &err);
  if (!archive) {
    Xenia::Logger::logError("Failed to open Resources!", 1);
    return nullptr;
  }

  struct zip_stat st;
  zip_stat_init(&st);
  if (zip_stat(archive, pathToTexture.c_str(), 0, &st) != 0) {
    Xenia::Logger::logError("Failed to find file in archive: " + pathToTexture, 1);
    zip_close(archive);
    return nullptr;
  }

  if (st.size > SIZE_MAX) {
    Xenia::Logger::logError("File too large: " + pathToTexture, 1);
    zip_close(archive);
    return nullptr;
  }

  zip_file *file = zip_fopen(archive, pathToTexture.c_str(), 0);
  if (!file) {
    Xenia::Logger::logError("Failed to open file in archive", 1);
    zip_close(archive);
    return nullptr;
  }

  std::vector<char> buffer(static_cast<size_t>(st.size));
  zip_int64_t bytesRead = zip_fread(file, buffer.data(), buffer.size());

  zip_fclose(file);
  zip_close(archive);

  if (bytesRead != static_cast<zip_int64_t>(buffer.size())) {
    Xenia::Logger::logError("Failed to extract data", 1);
    return nullptr;
  }

  SDL_IOStream *io = SDL_IOFromConstMem(buffer.data(), buffer.size());
  if (!io) {
    Xenia::Logger::logError(SDL_GetError(), 1);
    return nullptr;
  }

  SDL_Texture *tex = IMG_LoadTexture_IO(ren, io, true);
  if (!tex) {
    return nullptr;
  }

  SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

  return tex;
}

std::string Xenia::AssetManager::loadUncompressedData(std::string fileName) {
  Xenia::Logger::logMessage("Load Data from: " + fileName);
  std::ifstream file(fileName);
  if (!file.is_open()) {
    Xenia::Logger::logError("Failed to open!", 0);
    return "";
  }

  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();
  return content;
}

std::string Xenia::AssetManager::loadCompressedData(std::string filename) {
  int err = 0;

  zip *a = zip_open(resPack, 0, &err);
  if (a == nullptr) {
    Xenia::Logger::logError("Failed to open Resources!", 1);
  }

  struct zip_stat st;
  zip_stat_init(&st);
  if (zip_stat(a, filename.c_str(), 0, &st) != 0) {
    zip_close(a);
    Xenia::Logger::logError( "Failed to find file details in archive, looked for: " + filename, 1);
  }

  zip_file *file = zip_fopen(a, filename.c_str(), 0);
  if (file == nullptr) {
    Xenia::Logger::logError("Failed to find file in archive", 1);
  }

  std::vector<char> buffer(st.size);

  zip_int64_t bytesRead = zip_fread(file, buffer.data(), st.size);
  if (bytesRead < 0 || (zip_uint64_t)bytesRead != st.size) {
    zip_fclose(file);
    zip_close(a);
    Xenia::Logger::logError("Failed to extract data", 1);
  }

  std::string content(buffer.begin(), buffer.end());

  zip_fclose(file);
  zip_close(a);

  return content;
}
