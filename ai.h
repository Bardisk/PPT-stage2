#include <server/component.h>
#ifndef AI_H
#define AI_H

enum ModeType
{
    FLEE,
    EAT,
    OPEN,
    ATK,
    NONE = -1
};

class Ai
{
    int myNum;
public:
    loca target;
    int targetPlayerNum;
    ModeType mode;
    Ai(int _myNum)
        : myNum(_myNum)
        , target(-1, -1)
        , mode(NONE)
    {}
    int map[50][50];
    loca pre[50][50];
    bool vis[50][50];
    DirectionType Decision(GameMainMap *coreData);
    bool isthereWave(loca pos, GameMainMap *coreData);
};

#endif // AI_H
