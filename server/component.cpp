#include "component.h"


Entity::Entity(loca _pos, int _tough, EntityType _type)
    : entityType(_type)
    , pos(_pos)
    , toughness(_tough)
{}

ItemEntity::ItemEntity(loca _pos, int _v, ItemType _itemType)
    : Entity(_pos, 0, ITEM)
    , itemType(_itemType)
    , value(_v)
{}

MoveableEntity::MoveableEntity(loca _pos, int _tough, int _speed, EntityType _type)
    : Entity(_pos, _tough, _type)
    , isMoving(false)
    , posTo()
    , stat(0)
    , speed(_speed)
{}

//both class for wave and base for bomb
BWEntity::BWEntity(loca _pos, int _setterNum, int _rtime, int _damage, EntityType _type)
    : Entity(_pos, inf, _type)
    , remainTime(_rtime)
    , damage(_damage)
    , setterNum(_setterNum)
{}

BombEntity::BombEntity(loca _pos, int _setterNum, int _level, int _rtime, int _damage)
    : Entity(_pos, inf, BOMB)
    , MoveableEntity(_pos, inf, 0, BOMB)
    , BWEntity(_pos, _setterNum, _rtime, _damage)
    , level(_level)
{}

Player::Player(loca _pos, int _num, QString _name, int _level, int _possessCount, int _score, bool _canMoveBomb)
    : Entity(_pos, DEFAULTHPOINT, PLAYER)
    , MoveableEntity(_pos, DEFAULTHPOINT, 50, PLAYER)
    , num(_num)
    , name(_name)
    , level(_level)
    , possessCount(_possessCount)
    , score(_score)
    , canMoveBomb(_canMoveBomb)
{}

QVariantMap* Entity::toVariant()
{
    QVariantMap *current = new QVariantMap();
    (*current)["entityType"] = (int) entityType;
    (*current)["posx"] = pos.x;
    (*current)["posy"] = pos.y;
    (*current)["toughness"] = toughness;
    return current;
}

Entity::Entity(QVariantMap *from)
    : pos((*from)["posx"].toInt(), (*from)["posy"].toInt())
    , toughness((*from)["toughness"].toInt())
{
    entityType = (EntityType) (*from)["entityType"].toInt();
}

QVariantMap* ItemEntity::toVariant()
{
    QVariantMap *current = Entity::toVariant();
    (*current)["itemType"] = (int) itemType;
    (*current)["value"] = value;
    return current;
}

ItemEntity::ItemEntity(QVariantMap *from)
    : Entity(from)
    , value((*from)["value"].toInt())
{
    itemType = (ItemType) (*from)["itemType"].toInt();
}


QVariantMap* MoveableEntity::toVariant()
{
    QVariantMap *current = Entity::toVariant();
    (*current)["isMoving"] = isMoving;
    (*current)["posTox"] = posTo.x;
    (*current)["posToy"] = posTo.y;
    (*current)["stat"] = stat;
    (*current)["speed"] = speed;
    return current;
}

MoveableEntity::MoveableEntity(QVariantMap *from)
    : Entity(from)
    , isMoving((*from)["isMoving"].toBool())
    , posTo((*from)["posTox"].toInt(), (*from)["posToy"].toInt())
    , stat((*from)["stat"].toInt())
    , speed((*from)["speed"].toInt())
{}


QVariantMap* BWEntity::toVariant()
{
    QVariantMap *current = Entity::toVariant();
    (*current)["remainTime"] = remainTime;
    (*current)["damage"] = damage;
    (*current)["setterNum"] = setterNum;
    return current;
}

BWEntity::BWEntity(QVariantMap *from)
    : Entity(from)
    , remainTime((*from)["remainTime"].toInt())
    , damage((*from)["damage"].toInt())
    , setterNum((*from)["setterNum"].toInt())
{}

QVariantMap* BombEntity::toVariant()
{
    QVariantMap *currentA = MoveableEntity::toVariant();
    QVariantMap *currentB = BWEntity::toVariant();
    for (auto i = currentB->constBegin(); i != currentB->constEnd(); ++i)
        currentA->insert(i.key(), i.value());
    delete currentB;
    (*currentA)["level"] = level;
    return currentA;
}

BombEntity::BombEntity(QVariantMap *from)
    : Entity(from)
    , MoveableEntity(from)
    , BWEntity(from)
    , level((*from)["level"].toInt())
{}

QVariantMap* Player::toVariant(){
    QVariantMap *current = MoveableEntity::toVariant();
    (*current)["num"] = num;
    (*current)["level"] = level;
    (*current)["possessCount"] = possessCount;
    (*current)["score"] = score;
    (*current)["canMoveBomb"] = canMoveBomb;
    return current;
}

Player::Player(QVariantMap *from)
    : Entity(from)
    , MoveableEntity(from)
    , num((*from)["num"].toInt())
    , level((*from)["level"].toInt())
    , possessCount((*from)["possessCount"].toInt())
    , score((*from)["score"].toInt())
    , canMoveBomb((*from)["canMoveBomb"].toBool())
{}

BombEntity* Player::setBomb()
{
    if (possessCount > 0)
        --possessCount;
    else return nullptr;
    return new BombEntity(pos, num, level);
}

WaveEntity* BombEntity::setWave()
{
    return new WaveEntity(pos, setterNum);
}

DirectionType MoveableEntity::direction()
{
    loca tmp = pos - posTo;
    if (tmp.x == 1)
        return DW;
    if (tmp.x == -1)
        return UP;
    if (tmp.y == 1)
        return LF;
    if (tmp.y == -1)
        return RT;
    return NE;
}

QJsonObject* Entity::toJSON()
{
    QVariantMap *current = this->toVariant();
    QJsonObject *currentJs = new QJsonObject(QJsonObject::fromVariantMap(*current));
    delete current;
    return currentJs;
}

Entity::~Entity()
{}

MoveableEntity::~MoveableEntity()
{}

BWEntity::~BWEntity()
{}

BombEntity::~BombEntity()
{}

Player::~Player()
{}

MapNode::MapNode()
{}

MapNode::MapNode(QVariantMap *from)
    : resideEntities((*from)["rsEntities"].toList())
    , movingtoEntities((*from)["mtEntities"].toList())
{}

QVariantMap* MapNode::toVariant()
{
    QVariantMap* current = new QVariantMap();
    (*current)["mtEntities"] = QVariant(movingtoEntities);
    (*current)["rsEntities"] = QVariant(resideEntities);
    return current;
}

QJsonObject* MapNode::toJSON()
{
    QVariantMap *current = this->toVariant();
    QJsonObject *currentJs = new QJsonObject(QJsonObject::fromVariantMap(*current));
    delete current;
    return currentJs;
}

MapNode::~MapNode()
{}

GameMainMap::GameMainMap()
    : szN(0)
    , szM(0)
{}

GameMainMap::GameMainMap(int _N, int _M)
    : szN(_N)
    , szM(_M)
{}

QVariantMap* GameMainMap::toVariant()
{
    QVariantMap* current = new QVariantMap();
    qDebug()<<szN<<szM;
    (*current)["N"] = szN;
    (*current)["M"] = szM;
    QList <QVariant> tmp1;
    foreach(MapNode i, map)
    {
        QVariantMap *tmpnow = i.toVariant();
        tmp1.append(QVariant(*tmpnow));
        delete tmpnow;
    }
    (*current)["mapDt"] = tmp1;
    QList <QVariant> tmp2;
    foreach(Player i, players)
    {
        QVariantMap *tmpnow = i.toVariant();
        tmp2.append(QVariant(*tmpnow));
        delete tmpnow;
    }
    (*current)["plaDt"] = tmp2;
    return current;
}

QJsonObject* GameMainMap::toJSON()
{
    QVariantMap *current = this->toVariant();
    QJsonObject *currentJs = new QJsonObject(QJsonObject::fromVariantMap(*current));
    delete current;
    return currentJs;
}

int GameMainMap::geti(int _num)
{
    return _num / szM;
}
int GameMainMap::getj(int _num)
{
    return _num % szM;
}

GameMainMap::GameMainMap(QVariantMap *fromVariant)
    : szN((*fromVariant)["N"].toInt())
    , szM((*fromVariant)["M"].toInt())
{
    auto tmpmap = (*fromVariant)["mapDt"].toList();
    auto tmppla = (*fromVariant)["plaDt"].toList();
    foreach(QVariant i, tmpmap)
    {
        auto tmptmp = i.toMap();
        map.push_back(MapNode(&tmptmp));
    }
    foreach(QVariant i, tmppla)
    {
        auto tmptmp = i.toMap();
        players.push_back(Player(&tmptmp));
    }
}

void GameMainMap::load(QVariantMap *fromVariant)
{
    szN = (*fromVariant)["N"].toInt();
    szM = (*fromVariant)["M"].toInt();
    qDebug() << szN;
    qDebug() << szM;
    map.clear();
    players.clear();
    auto tmpmap = (*fromVariant)["mapDt"].toList();
    auto tmppla = (*fromVariant)["plaDt"].toList();
    foreach(QVariant i, tmpmap)
    {
        auto tmptmp = i.toMap();
        map.push_back(MapNode(&tmptmp));
    }
    foreach(QVariant i, tmppla)
    {
        auto tmptmp = i.toMap();
        players.push_back(Player(&tmptmp));
    }
    return ;
}

int GameMainMap::size(){
    return szN * szM;
}
