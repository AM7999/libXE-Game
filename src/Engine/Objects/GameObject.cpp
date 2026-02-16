#include "GameObject.hpp"
#include "../Debug/Logger.hpp"
#include "../Entity/Entity.hpp"

Xenia::GameObject::GameObject(float x, float y, SDL_Texture* img, SDL_Renderer* rnd, std::string name, std::string space, bool inter) {
  Xenia::Logger::logMessage("GameObject: " + space + ":" + name);

  this->x = x;
  this->y = y;
  this->img = img;
  this->rnd = rnd;

  this->name = name;
  this->space = space;

  this->rect.x = x;
  this->rect.y = y;

  this->rect.h = 48.0;
  this->rect.w = 48.0;

  this->interactable = inter;
}

Xenia::GameObject::~GameObject() {}

void Xenia::GameObject::draw() {
  SDL_RenderTexture(this->rnd, this->img, NULL, &rect);
}

void Xenia::GameObject::interact(Xenia::Entity entity) {
  // FIX: was `entity.getY()` which is truthy for any non-zero Y, not an actual bounds check
  if(entity.getX() >= x && entity.getY() >= y) {
    if(interactable)
      Xenia::Logger::logMessage("Interact.");
    if(!interactable)
      Xenia::Logger::logWarning("Not interactable");
  }
}
