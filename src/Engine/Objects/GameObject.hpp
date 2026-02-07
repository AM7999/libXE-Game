#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <SDL3_image/SDL_image.h>
#include "../Entity/Entity.hpp"

namespace Xenia {
  class GameObject {
    public:
      GameObject(float x, float y, SDL_Texture* img, SDL_Renderer* ren, std::string name, std::string space, bool inter);
      ~GameObject();
      void draw();
      void interact(Xenia::Entity entity);

      const std::string getName() const { return name; }
      const std::string getSpace() const { return space; }
      float getX() const { return x; }
      float getY() const { return y; }
      bool isInteractable() const { return interactable; }

      SDL_Texture* getImg() const { return img; }
    protected:

      std::string name;
      std::string space;

      float x = 0;
      float y = 0;

      SDL_FRect rect = {x,y, 48, 48};

      SDL_Texture* img;
      SDL_Renderer* rnd;
      bool interactable;
  };
}

#endif
