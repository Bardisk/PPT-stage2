#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event);
public:
    GraphicsView(QWidget *parent=nullptr);
};

#endif // GRAPHICSVIEW_H
