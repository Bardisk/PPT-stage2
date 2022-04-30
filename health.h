#ifndef HEALTH_H_
#define HEALTH_H_

#include "hitable.h"

class Health : public Hitable {
 public:
  explicit Health(int health);

  void beHit() override;

 protected:
  int health;
};

#endif  // HEALTH_H_
