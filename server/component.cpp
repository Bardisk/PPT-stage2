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

MoveableEntity::MoveableEntity(loca _pos, int _tough, double _speed, EntityType _type)
    : Entity(_pos, _tough, _type)
    , isMoving(false)
    , posTo()
    , speed(_speed)
{}

//both class for wave and base for bomb
BWEntity::BWEntity(loca _pos, Player* _setter, int _rtime = DEFAULTRTIMEW, int _damage = DEFAULTDAMAGE, EntityType _type = WAVE)
    : Entity(_pos, inf, _type)
    , remainTime(_rtime)
    , damage(_damage)
    , setter(_setter)
{}

BombEntity::BombEntity(loca _pos, Player* _setter, int _rtime = DEFAULTRTIMEB, int _damage = DEFAULTDAMAGE)
    : Entity(_pos, inf, BOMB)
    , MoveableEntity(_pos, inf, 0, BOMB)
    , BWEntity(_pos, _setter, _rtime, _damage)
    , level(setter->level)
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
    (*current)["speed"] = speed;
    return current;
}

MoveableEntity::MoveableEntity(QVariantMap *from)
    : Entity(from)
    , isMoving((*from)["isMoving"].toBool())
    , posTo((*from)["posTox"].toInt(), (*from)["posToy"].toInt())
    , speed((*from)["speed"].toDouble())
{}

QVariantMap* Player::toVariant(){
    QVariantMap *current = MoveableEntity::toVariant();
    (*current)["level"] = level;
    (*current)["possessCount"] = possessCount;
    (*current)["score"] = score;
    (*current)["canMoveBomb"] = canMoveBomb;
    return current;
}

Player::Player(QVariantMap *from)
    : Entity(from)
    , MoveableEntity(from)
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
    return new BombEntity(pos, this);
}

WaveEntity* BombEntity::setWave()
{
    return new WaveEntity(pos, setter);
}

QVariantMap* BWEntity::toVariant(){
    QVariantMap *current = Entity::toVariant();
    return current;
}

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



QJsonObject* Entity::toJSON()
{
    QVariantMap *current = this->toVariant();
    QJsonObject *currentJs = new QJsonObject(QJsonObject::fromVariantMap(*current));
    return currentJs;
}
