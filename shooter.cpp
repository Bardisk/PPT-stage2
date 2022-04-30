#include "shooter.h"

#include <ammo.h>
#include <gameobject.h>
#include <transform.h>

#include <QGraphicsRectItem>

Shooter::Shooter() : Component() {}

void Shooter::onAttach() {
  this->transform = this->gameObject->getComponent<Transform>();
  Q_ASSERT(this->transform != nullptr);
  auto rect = new QGraphicsRectItem(this->transform);
  rect->setRect(QRectF(-30, -30, 60, 60));
}

void Shooter::onUpdate(float deltaTime) {
  cooldown -= deltaTime;
  if (cooldown > 0) return;
  cooldown = interval;

  QPointF velocity;
  if (this->getKey(Qt::Key_W))
    velocity = QPointF(0, -100);
  else if (this->getKey(Qt::Key_A))
    velocity = QPointF(-100, 0);
  else if (this->getKey(Qt::Key_S))
    velocity = QPointF(0, 100);
  else
    velocity = QPointF(100, 0);
  auto ammo = new GameObject();
  ammo->addComponent(new Transform(this->transform->pos()));
  ammo->addComponent(new Ammo(velocity));
  attachGameObject(ammo);
}
