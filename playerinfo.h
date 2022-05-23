#include <QApplication>
#include <QKeyEvent>
#include "server/component.h"
#ifndef PLAYERINFO_H
#define PLAYERINFO_H


class PlayerInfo
{
public slots:
    void setnum(int _num);
public:
    int playerNum;
    QString name;
    Qt::Key up, down, left, right, keydown;
    PlayerInfo(Qt::Key _u, Qt::Key _d, Qt::Key _l, Qt::Key _r, Qt::Key _keydown, QString _name);
    DirectionType getDirection(QKeyEvent *event);
    bool isKeyDown(QKeyEvent *event);
};

#endif // PLAYERINFO_H
