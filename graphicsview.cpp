#include "graphicsview.h"
#include "qevent.h"

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_PageDown || event->key() == Qt::Key_PageUp){
        return QGraphicsView::keyPressEvent(event);
    }
    event->ignore();
}

GraphicsView::GraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{}
