#include "summondummy.h"

#include <gameobject.h>
#include <transform.h>

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>

#include "health.h"

SummonDummy::SummonDummy() {}

void SummonDummy::onAttach() {
  auto transform = this->gameObject->getComponent<Transform>();
  Q_ASSERT(transform != nullptr);
  auto text = new QGraphicsSimpleTextItem(transform);
  text->setText("生成假人");
  // Handle mouse event of text by this->transform
  transform->setFiltersChildEvents(true);
}

void SummonDummy::onClick(QGraphicsSceneMouseEvent *ev) {
  auto dummy = new GameObject();
  auto dummyTransform = new Transform(QPointF(100, 30));
  auto dummyRect = new QGraphicsRectItem(dummyTransform);
  dummyRect->setRect(QRectF(-20, -20, 40, 40));
  dummy->addComponent(dummyTransform);
  dummy->addComponent(new Health(3));
  attachGameObject(dummy);
}
