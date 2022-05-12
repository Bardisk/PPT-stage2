#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtCore>

const int DEFAULTRTIMEB = 50;
const int DEFAULTRTIMEW = 20;
const int DEFAULTDAMAGE = 30;
const int DEFAULTHPOINT = 20;

const int inf = 0x3f3f3f3f;

struct loca
{
    int x, y;
    loca(int _x=0, int _y=0)
        : x(_x)
        , y(_y)
    {}
    loca operator - (const loca &b) const
    {
        return loca(x-b.x, y-b.y);
    }
};

enum EntityType
{
    FLOOR, //not actually used
    HARDWALL,
    SOFTWALL,
    BOMB,
    WAVE,
    ITEM,
    PLAYER,
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
//    ~ItemEntity();
  QVariantMap* toVariant();
};

enum DirectionType{
    UP=1, DW=3, LF=0, RT=4, NE=-1
};

class MoveableEntity : virtual public Entity
{
public:
    bool isMoving;
    loca posTo;
    int stat;
    int speed;
    MoveableEntity(loca _pos, int _tough, int _speed, EntityType _type);
    MoveableEntity(QVariantMap *from);
    ~MoveableEntity();
    DirectionType direction();
    QVariantMap* toVariant();
};

class BombEntity;

class Player : public MoveableEntity
{
public:
    int num;
    QString name;
    int level;
    int possessCount;
    int score;
    bool canMoveBomb;
    Player(loca _pos, int _num, QString _name, int _level=1, int _possessCount=1, int _score=0, bool canMoveBomb=0);
    Player(QVariantMap *from);
    ~Player();
    QVariantMap* toVariant();
    BombEntity* setBomb();
};

class BWEntity : virtual public Entity
{
public:
    BWEntity(loca _pos, int _setterNum, int _rtime = DEFAULTRTIMEW, int _damage = DEFAULTDAMAGE, EntityType _type = WAVE);
    BWEntity(QVariantMap *from);
    ~BWEntity();
    int remainTime;
    int damage;
    int setterNum;
    //Player* setter;
    QVariantMap* toVariant();
};

typedef BWEntity WaveEntity;

class BombEntity : public MoveableEntity, BWEntity
{
    int level;   
public:
    BombEntity(loca _pos, int _setterNum, int _level, int _rtime = DEFAULTRTIMEB, int _damage = DEFAULTDAMAGE);
    BombEntity(QVariantMap *from);
    ~BombEntity();
    QVariantMap* toVariant();
    WaveEntity* setWave();
};



class MapNode{

public:
    QList <QVariant> resideEntities;
    QList <QVariant> movingtoEntities;
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
    QList <Player> players;
    int getnum(int _x, int _y);
    GameMainMap();
    GameMainMap(int _N, int _M);
    GameMainMap(QVariantMap *fromVariant);
    void load(QVariantMap *fromVariant);
    QVariantMap* toVariant();
    QJsonObject* toJSON();
};

#endif // COMPENONT_H
