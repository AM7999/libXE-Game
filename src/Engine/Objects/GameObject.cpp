#include "GameObject.hpp"
#include "../Debug/Logger.hpp"
#include "../Entity/Entity.hpp"

//SDL_FRect 

Xenia::GameObject::GameObject(float x, float y, SDL_Texture* img, SDL_Renderer* rnd, std::string name, std::string space, bool inter) {
  Xenia::Logger::logMessage("Xenia::GameObject::GameObject");

  this->x = x;
  this->y = y;
  this->img = img;
  this->rnd = rnd;

  this->name = name;
  this->space = space;

  this->interactable = inter;
}

void Xenia::GameObject::draw() {
  SDL_RenderTexture(this->rnd, this->img, NULL, &rect);
}

void Xenia::GameObject::interact(Xenia::Entity entity) {
  //if(entity.getX() && )
  if(interactable)
    Xenia::Logger::logMessage("Interact.");
  if(!interactable)
    Xenia::Logger::logWarning("Not interactable");
}
