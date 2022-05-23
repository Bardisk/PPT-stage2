#include "health.h"

Health::Health(int health) : health(health) {}

void Health::beHit() {
  health--;
  if (health <= 0) {
    destory(this->gameObject);
  }
}
