#include "component.h"

#include "gameobject.h"

Component::Component() : QObject() {}

void Component::attachGameObject(GameObject *gameObject) {
  this->gameObject->attachGameObject(gameObject);
}
void Component::detachGameObject(GameObject *gameObject) {
  this->gameObject->detachGameObject(gameObject);
}
void Component::destory(GameObject *gameObject) {
  this->gameObject->destory(gameObject);
}

bool Component::getKey(Qt::Key key) { return this->gameObject->getKey(key); }
bool Component::getKeyDown(Qt::Key key) {
  return this->gameObject->getKeyDown(key);
}
bool Component::getKeyUp(Qt::Key key) {
  return this->gameObject->getKeyUp(key);
}

GameObject *Component::getParentGameObject() { return this->gameObject; }
void Component::setParentGameObject(GameObject *gameObject) {
  this->setParent(gameObject);
  this->gameObject = gameObject;
}
