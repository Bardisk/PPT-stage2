#ifndef COMPONENT_H
#define COMPONENT_H

#include <QtCore>

class Ai;
const int DEFAULTRTIMEB = 50;
const int DEFAULTRTIMEW = 20;
const int DEFAULTDAMAGE = 30;
const int DEFAULTHPOINT = 20;
const double STATCOMPLETED = 10000.0;
const int SCOREBYKILL = 10000;
const int SCOREBYTIME = 1;
const int SCOREBYDESTORY = 500;
const int MAXSPEEDMULTIPLES = 1000;
const int ACCSPEEDMULTIPLES = 200;
const int ITEMBOUNDCNT = 15;
const int MAXLEVEL = 5;
const int MAXPOSSESS = 4;
const int MAXSPEED = 3;
const int BOMBSPEEDREDUCING = 50;

const int inf = 0x3f3f3f3f;

#define findo(it, type, node)\
    if (((it) = (node).getElement((type))) != (node).resideEntities.end())

class GameMainMap;

typedef QList <QVariant>::Iterator lpNodeMem;

struct loca
{
    int x, y;
    loca(int _x=-1, int _y=-1)
        : x(_x)
        , y(_y)
    {}
    loca(const QVariantMap &from)
        : x(from["x"].toInt())
        , y(from["y"].toInt())
    {}
    loca operator - (const loca &b) const
    {
        return loca(x-b.x, y-b.y);
    }
    bool operator == (const loca &b) const{
        return x==b.x && y==b.y;
    }
    bool operator != (const loca &b) const{
        return !((*this) == b);
    }
    QVariantMap* toVariant() const
    {
        QVariantMap* current = new QVariantMap();
        (*current)["x"] = x;
        (*current)["y"] = y;
        return current;
    }
    QVariantMap Variant() const
    {
        QVariantMap *current = toVariant();
        QVariantMap now = *current;
        delete current;
        return now;
    }
    QJsonObject* toJSON()
    {
        QVariantMap* current = toVariant();
        QJsonObject* currentJs = new QJsonObject(QJsonObject::fromVariantMap(*current));
        delete current;
        return currentJs;
    }
    bool isVaild() const{return x>0 && y>0;}
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
    int protectTime;

    Entity(loca _pos, int _tough, EntityType _type=UNDEF, int _protectTime=0);
    Entity(const QVariantMap *from);
    Entity(const QVariantMap &from)
        : Entity(&from)
    {}
    ~Entity();
    virtual QVariantMap* toVariant() const;
    virtual QVariantMap Variant() const;
    virtual void die(GameMainMap *coreData) const;
    virtual void update(GameMainMap *coreData) const;
    virtual void wear(GameMainMap *coreData);
    virtual bool damage(GameMainMap *coreData, int _damage);
    QJsonObject* toJSON();
};


class ItemEntity : public Entity
{
public:
    ItemType itemType;
    int value;
    ItemEntity(loca _pos, int _v, ItemType _itemType, int _protectTime = 0);
    ItemEntity(const QVariantMap *from);
    ItemEntity(const QVariantMap &from)
        : ItemEntity(&from)
    {}
//    ~ItemEntity();
  QVariantMap* toVariant() const;
};

enum DirectionType{
    UP=1, DW=3, LF=0, RT=4, NE=2, BNB=5
};

class MoveableEntity : virtual public Entity
{
public:
    bool isMoving;
    loca posTo;
    int stat;
    int speed;
    MoveableEntity(loca _pos, int _tough, int _speed, EntityType _type);
    MoveableEntity(const QVariantMap *from);
    ~MoveableEntity();
    DirectionType direction() const;
    QVariantMap* toVariant() const;
    void stop(GameMainMap *coreData);
    bool isBlocked(GameMainMap *coreData);
    virtual void move(GameMainMap *coreData) = 0;

};

class BombEntity;

class GameMainMap;

class Player : public MoveableEntity
{

public:
    static inline QString playerName[] = {"dao", "luxdao", "marid", "luxmarid"};
    int num;
    QString name;
    int level;
    int possessCount;
    int score;
    bool isAi;
    bool canMoveBomb;
    int speedLevel;
    bool isPressed[5];
    Ai *ai;
    DirectionType facing;
    Player(loca _pos, int _num, QString _name, bool _isAi=false, DirectionType _facing = DW, int _level=1, int _possessCount=1, int _score=0, bool _canMoveBomb=0, int _speedlevel=1);
    Player(const QVariantMap *from);
    Player(const QVariantMap &from)
        : Player(&from)
    {}
    ~Player();
    QVariantMap* toVariant() const;
    BombEntity setBomb();
    void releaseBomb(GameMainMap *coreData);
    void move(GameMainMap *coreData);
    void wear(GameMainMap *coreData);
    void changeScore(int addS, GameMainMap *coreData);
    void eatItem(GameMainMap *coreData);
    QString intepretPath();
};

class BWEntity : virtual public Entity
{
public:
    BWEntity(loca _pos, int _setterNum, int _rtime = DEFAULTRTIMEW, int _damage = DEFAULTDAMAGE, EntityType _type = WAVE);
    BWEntity(const QVariantMap *from);
    BWEntity(const QVariantMap &from)
        : BWEntity(&from)
    {}
    ~BWEntity();
    int remainTime;
    int damage;
    int setterNum;
    //Player* setter;
    QVariantMap* toVariant() const;
};

class WaveEntity : public BWEntity
{
public:
    int isHorizontal;
    WaveEntity(loca _pos, int _setterNum, int _isH, int _rtime = DEFAULTRTIMEW, int _damage = DEFAULTDAMAGE)
        : Entity(_pos, inf, WAVE)
        , BWEntity(_pos, _setterNum, _rtime, _damage)
        , isHorizontal(_isH)
    {}
    WaveEntity(const QVariantMap *from)
        : Entity(from)
        , BWEntity(from)
        , isHorizontal((*from)["isHorizontal"].toInt())
    {}
    WaveEntity(const QVariantMap &from)
        : WaveEntity(&from)
    {}
    void wear(GameMainMap *coreData);
    QVariantMap* toVariant() const
    {
        auto current = BWEntity::toVariant();
        (*current)["isHorizontal"] = isHorizontal;
        return current;
    }
};



class BombEntity : public MoveableEntity, BWEntity
{

public:
    int level;
    BombEntity(loca _pos, int _setterNum, int _level, int _rtime = DEFAULTRTIMEB, int _damage = DEFAULTDAMAGE);
    BombEntity(const QVariantMap *from);
    BombEntity(const QVariantMap &from)
        : BombEntity(&from)
    {}
    ~BombEntity();
    QVariantMap* toVariant() const;
    WaveEntity setWave(loca pos, int isHorizontal);
    void setoff(GameMainMap *coreData);
    void wear(GameMainMap *coreData);
    void move(GameMainMap *coreData);
    bool beTriggered(GameMainMap *coreData, DirectionType now, int _speed, int _stat);
};



class MapNode{

public:
    QList <QVariant> resideEntities;
    QList <loca> movingtoEntities;
    MapNode();
    MapNode(const QVariantMap *from);
    MapNode(const QVariantMap &from)
        : MapNode(&from)
    {}
    ~MapNode();
    QVariantMap* toVariant() const;
    QVariantMap Variant() const
    {
        QVariantMap *current = toVariant();
        QVariantMap now = *current;
        delete current;
        return now;
    }
    QJsonObject* toJSON() const;
    QList<QVariant>::iterator getElement(EntityType target);
    void generateItem(GameMainMap *coreData, loca pos);
    bool findElement(EntityType target);
    bool repWave(GameMainMap *coreData, const WaveEntity &nowWave);
};

class GameMainMap{
public:
    int szN, szM;
    QList <MapNode> map;
    QList <loca> players;
    int size() const;
    int getnum(int _x, int _y) const;
    int getnum(loca x) const;
    int geti(int _num) const;
    int getj(int _num) const;
    loca getpos(int _num) const
    {
        return loca(geti(_num), getj(_num));
    }
    void erasePlayers();
    GameMainMap();
    GameMainMap(int _N, int _M);
    GameMainMap(QVariantMap *fromVariant);
    void load(QVariantMap *fromVariant);
    QVariantMap* toVariant() const;
    QJsonObject* toJSON() const;
    MapNode& mapMem(loca x);
    MapNode& mapMem(int _x, int _y);
    QList <QVariant>::Iterator plaMem(int playerNum);
    Player gePlaMem(int playerNum);
    bool isDie(int playerNum);
};

#endif // COMPENONT_H
