#include "transform.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "gameobject.h"

Transform::Transform() {}
Transform::Transform(const QPointF &pos) { this->setPos(pos); }

int Transform::type() const { return mType; }
void Transform::setType(int type) { mType = type; }

QRectF Transform::boundingRect() const { return QRectF(); }
void Transform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(painter);
  Q_UNUSED(option);
  Q_UNUSED(widget);
}

void Transform::mousePressEvent(QGraphicsSceneMouseEvent *ev) {
  QGraphicsItem::mousePressEvent(ev);
  this->gameObject->onClick(ev);
}

bool Transform::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
  if (this->isAncestorOf(watched)) {
    switch (event->type()) {
      case QEvent::GraphicsSceneMousePress: {
        // Remap positions of event from watched item to this
        // Copied from
        // https://github.com/qt/qtbase/blob/625cfb6b400a88b26b7295f21699c9db28b71483/src/widgets/graphicsview/qgraphicsitem.cpp#L1092-L1101
        QGraphicsSceneMouseEvent *mouseEvent =
            static_cast<QGraphicsSceneMouseEvent *>(event);
        mouseEvent->setPos(this->mapFromItem(watched, mouseEvent->pos()));
        mouseEvent->setLastPos(this->mapFromItem(watched, mouseEvent->pos()));
        for (int i = 0x1; i <= 0x10; i <<= 1) {
          if (mouseEvent->buttons() & i) {
            Qt::MouseButton button = Qt::MouseButton(i);
            mouseEvent->setButtonDownPos(
                button,
                this->mapFromItem(watched, mouseEvent->buttonDownPos(button)));
          }
        }
        this->mousePressEvent(mouseEvent);
        return true;
      }
      default:
        break;
    }
  }
  return QGraphicsItem::sceneEventFilter(watched, event);
}
