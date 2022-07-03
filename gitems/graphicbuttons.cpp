#include "graphicbuttons.h"

GraphicButtons::GraphicButtons(const QPoint &_pos, const QPixmap &_pic, const QSize &_siz)
    : QGraphicsObject()
    , pos(_pos)
    , pic(_pic)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
    pic.scaled(_siz);
}
GraphicButtons::GraphicButtons(const QPoint &_pos, const QPixmap &_pic)
    : QGraphicsObject()
    , pos(_pos)
    , pic(_pic)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
}


void GraphicButtons::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    return event->accept();
}
void GraphicButtons::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    return ;
}
void GraphicButtons::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->drawPixmap(boundingRect(), pic, pic.rect());
    painter->restore();
}

QRectF GraphicButtons::boundingRect() const
{
    return QRectF(pos, pic.rect().size());
}
