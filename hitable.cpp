#include "hitable.h"

Hitable::Hitable() {}

void Hitable::beHit() { destory(this->gameObject); }
