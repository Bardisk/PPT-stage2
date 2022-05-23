#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>
#include <QPainter>
#include <QPixmap>
#include "../server/component.h"

#ifndef GRAPHICBUTTONS_H
#define GRAPHICBUTTONS_H


class GraphicButtons : public QGraphicsObject
{
    Q_OBJECT
    QPoint pos;
    QPixmap pic;
signals:
    void clicked();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
    GraphicButtons(const QPoint &_pos, const QPixmap &_pic);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
};

#endif // GRAPHICBUTTONS_H
