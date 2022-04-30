#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "component.h"

class Transform;

class Shooter : public Component {
 public:
  Shooter();

  void onAttach() override;
  void onUpdate(float deltaTime) override;

 protected:
  Transform *transform = nullptr;

  const float interval = .5;
  float cooldown = interval;
};

#endif  // SHOOTER_H_
