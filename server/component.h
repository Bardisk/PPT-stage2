#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtCore>

const int DEFAULTRTIMEB = 50;
const int DEFAULTRTIMEW = 20;
const int DEFAULTDAMAGE = 30;

const int inf = 0x3f3f3f3f;

struct loca
{
    int x, y;
    loca(int _x=0, int _y=0)
        : x(_x)
        , y(_y)
    {}
};

enum EntityType
{
    FLOOR, //not actually used
    HARDWALL,
    SOFTWALL,
    BOMB,
    WAVE,
    ITEM,
    UNDEF=0xff
};

enum ItemType
{
    SPEED,
    LEVEL,
    STRENGHTH,
    RICHMENT
};

class Entity
{
public:
    EntityType entityType;
    loca pos;
    int toughness;

    Entity(loca _pos, int _tough, EntityType _type=UNDEF);
    Entity(QVariantMap *from);
    ~Entity();
    virtual QVariantMap* toVariant();
    QJsonObject* toJSON();

};


class ItemEntity : public Entity
{
public:
    ItemType itemType;
    int value;
    ItemEntity(loca _pos, int _v, ItemType _itemType);
    ItemEntity(QVariantMap *from);
    ~ItemEntity();
    QVariantMap* toVariant();
};

enum DirectionType{
    UP, DW, LF, RT
};

class MoveableEntity : virtual public Entity
{
public:
    bool isMoving;
    loca posTo;
    double speed;
    MoveableEntity(loca _pos, int _tough, double _speed, EntityType _type);
    MoveableEntity(QVariantMap *from);
    ~MoveableEntity();
    DirectionType direction();
    QVariantMap* toVariant();
};

class BombEntity;

class Player : public MoveableEntity
{
public:
    QString name;
    int level;
    int possessCount;
    int score;
    bool canMoveBomb;
    Player();
    Player(QVariantMap *from);
    ~Player();
    QVariantMap* toVariant();
    BombEntity* setBomb();
};

class BWEntity : virtual public Entity
{
public:
    BWEntity(loca _pos, Player* _setter, int _rtime, int _damage, EntityType _type);
    BWEntity(QVariantMap *from);
    int remainTime;
    int damage;
    Player* setter;
    QVariantMap* toVariant();
};

typedef BWEntity WaveEntity;

class BombEntity : public MoveableEntity, BWEntity
{
    int level;   
public:
    BombEntity(loca _pos, Player* _setter, int _rtime, int _damage);
    BombEntity(QVariantMap *from);
    ~BombEntity();
    QVariantMap* toVariant();
    WaveEntity* setWave();
};



class MapNode{

public:
    QList <QVariantMap> resideEntites;
    QList <QVariantMap> movingtoEntities;
    MapNode();
    MapNode(QVariantMap *from);
    ~MapNode();
    QVariantMap* toVariant();
    QJsonObject* toJSON();
};

class GameMainMap{
    int szN, szM;
public:
    QList <MapNode> map;
    int getnum(int _x, int _y);
    GameMainMap();
    GameMainMap(int _N, int _M);
    GameMainMap(QVariantMap *fromVariant);
    QVariantMap* toVariant();
    QJsonObject* toJSON();
};

#endif // COMPENONT_H
