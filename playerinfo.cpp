#include "playerinfo.h"


PlayerInfo::PlayerInfo(Qt::Key _u, Qt::Key _d, Qt::Key _l, Qt::Key _r, Qt::Key _keydown, QString _name)
    : playerNum(-1)
    , name(_name)
    , up(_u)
    , down(_d)
    , left(_l)
    , right(_r)
    , keydown(_keydown)
{}


DirectionType PlayerInfo::getDirection(QKeyEvent *event)
{
    if (event->key() == up) {
        return DirectionType::UP;
    }
    if (event->key() == down) {
        return DirectionType::DW;
    }
    if (event->key() == left) {
        return DirectionType::LF;
    }
    if (event->key() == right) {
        return DirectionType::RT;
    }
    return DirectionType::NE;
}

bool PlayerInfo::isKeyDown(QKeyEvent *event)
{
    return event->key() == keydown;
}

void PlayerInfo::setnum(int _num)
{
    playerNum = _num;
}
