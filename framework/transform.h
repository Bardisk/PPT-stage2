#ifndef FRAMEWORK_TRANSFORM_H_
#define FRAMEWORK_TRANSFORM_H_

#include <QGraphicsItem>

#include "component.h"

class Transform : public Component, public QGraphicsItem {
 public:
  Transform();
  explicit Transform(const QPointF &pos);

  Transform(const Transform &) = delete;
  Transform &operator=(const Transform &) = delete;

  virtual int type() const override;
  void setType(int type);

 protected:
  int mType = QGraphicsItem::Type;

 protected:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *ev) override;

 protected:
  bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;
};

#endif  // FRAMEWORK_TRANSFORM_H_
