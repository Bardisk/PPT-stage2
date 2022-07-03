#include "component.h"


Entity::Entity(loca _pos, int _tough, EntityType _type, int _protectTime)
    : entityType(_type)
    , pos(_pos)
    , toughness(_tough)
    , protectTime(_protectTime)
{}

ItemEntity::ItemEntity(loca _pos, int _v, ItemType _itemType, int _protectTime)
    : Entity(_pos, 0, ITEM, _protectTime)
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

Player::Player(loca _pos, int _num, QString _name, bool _isAi, DirectionType _facing, int _level, int _possessCount, int _score, bool _canMoveBomb, int _speedlevel)
    : Entity(_pos, DEFAULTHPOINT, PLAYER)
    , MoveableEntity(_pos, DEFAULTHPOINT, 0, PLAYER)
    , num(_num)
    , name(_name)
    , level(_level)
    , possessCount(_possessCount)
    , score(_score)
    , isAi(_isAi)
    , canMoveBomb(_canMoveBomb)
    , speedLevel(_speedlevel)
    , ai(nullptr)
    , facing(_facing)
{
    for (int i = 0; i < 5; i++)
        isPressed[i] = false;
}

QVariantMap* Entity::toVariant() const
{
    QVariantMap *current = new QVariantMap();
    (*current)["entityType"] = (int) entityType;
    (*current)["posx"] = pos.x;
    (*current)["posy"] = pos.y;
    (*current)["toughness"] = toughness;
    (*current)["protectTime"] = protectTime;
    return current;
}

Entity::Entity(const QVariantMap *from)
    : pos((*from)["posx"].toInt(), (*from)["posy"].toInt())
    , toughness((*from)["toughness"].toInt())
    , protectTime((*from)["protectTime"].toInt())
{
    entityType = (EntityType) (*from)["entityType"].toInt();
}

QVariantMap* ItemEntity::toVariant() const
{
    QVariantMap *current = Entity::toVariant();
    (*current)["itemType"] = (int) itemType;
    (*current)["value"] = value;
    return current;
}

ItemEntity::ItemEntity(const QVariantMap *from)
    : Entity(from)
    , value((*from)["value"].toInt())
{
    itemType = (ItemType) (*from)["itemType"].toInt();
}


QVariantMap* MoveableEntity::toVariant() const
{
    QVariantMap *current = Entity::toVariant();
    (*current)["isMoving"] = isMoving;
    (*current)["posTox"] = posTo.x;
    (*current)["posToy"] = posTo.y;
    (*current)["stat"] = stat;
    (*current)["speed"] = speed;
    return current;
}

MoveableEntity::MoveableEntity(const QVariantMap *from)
    : Entity(from)
    , isMoving((*from)["isMoving"].toBool())
    , posTo((*from)["posTox"].toInt(), (*from)["posToy"].toInt())
    , stat((*from)["stat"].toInt())
    , speed((*from)["speed"].toInt())
{}


QVariantMap* BWEntity::toVariant() const
{
    QVariantMap *current = Entity::toVariant();
    (*current)["remainTime"] = remainTime;
    (*current)["damage"] = damage;
    (*current)["setterNum"] = setterNum;
    return current;
}

BWEntity::BWEntity(const QVariantMap *from)
    : Entity(from)
    , remainTime((*from)["remainTime"].toInt())
    , damage((*from)["damage"].toInt())
    , setterNum((*from)["setterNum"].toInt())
{}

QVariantMap* BombEntity::toVariant() const
{
    QVariantMap *currentA = MoveableEntity::toVariant();
    QVariantMap *currentB = BWEntity::toVariant();
    for (auto i = currentB->constBegin(); i != currentB->constEnd(); ++i)
        currentA->insert(i.key(), i.value());
    delete currentB;
    (*currentA)["level"] = level;
    return currentA;
}

BombEntity::BombEntity(const QVariantMap *from)
    : Entity(from)
    , MoveableEntity(from)
    , BWEntity(from)
    , level((*from)["level"].toInt())
{
    isMoving = (*from)["isMoving"].toBool();
}

QVariantMap* Player::toVariant() const{
    QVariantMap *current = MoveableEntity::toVariant();
    (*current)["num"] = num;
    (*current)["level"] = level;
    (*current)["possessCount"] = possessCount;
    (*current)["score"] = score;
    (*current)["canMoveBomb"] = canMoveBomb;
    (*current)["isAi"] = isAi;
    (*current)["facing"] = facing;
    (*current)["speedLevel"] = speedLevel;
    (*current)["isPressedUp"] = isPressed[DirectionType::UP];
    (*current)["isPressedDw"] = isPressed[DirectionType::DW];
    (*current)["isPressedLf"] = isPressed[DirectionType::LF];
    (*current)["isPressedRt"] = isPressed[DirectionType::RT];
    return current;
}

Player::Player(const QVariantMap *from)
    : Entity(from)
    , MoveableEntity(from)
    , num((*from)["num"].toInt())
    , level((*from)["level"].toInt())
    , possessCount((*from)["possessCount"].toInt())
    , score((*from)["score"].toInt())
    , isAi((*from)["isAi"].toBool())
    , canMoveBomb((*from)["canMoveBomb"].toBool())
    , speedLevel((*from)["speedLevel"].toInt())
    , ai(nullptr)
    , facing((DirectionType) (*from)["facing"].toInt())
{
    isPressed[DirectionType::UP] = (*from)["isPressedUp"].toBool();
    isPressed[DirectionType::DW] = (*from)["isPressedDw"].toBool();
    isPressed[DirectionType::RT] = (*from)["isPressedRt"].toBool();
    isPressed[DirectionType::LF] = (*from)["isPressedLf"].toBool();
}

BombEntity Player::setBomb()
{
    if (possessCount > 0)
        --possessCount;
    else return BombEntity(pos, num, -1);
    return BombEntity(pos, num, level);
}

WaveEntity BombEntity::setWave(loca pos, int isHorizontal)
{
    return WaveEntity(pos, setterNum, isHorizontal);
}

DirectionType MoveableEntity::direction() const
{
    if (!posTo.isVaild()) return DirectionType::NE;
    loca tmp = pos - posTo;
    if (tmp.x == -1)
        return DW;
    if (tmp.x == 1)
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

QVariantMap Entity::Variant() const
{
    QVariantMap *current = toVariant();
    QVariantMap now = *current;
    delete current;
    return now;
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

MapNode::MapNode(const QVariantMap *from)
    : resideEntities((*from)["rsEntities"].toList())
{
    auto tmpMt = (*from)["mtEntities"].toList();
    foreach(auto i, tmpMt)
        movingtoEntities.push_back(loca(i.toMap()));
}

QVariantMap* MapNode::toVariant() const
{
    QVariantMap* current = new QVariantMap();
    QList <QVariant> tmpMt;
    foreach(loca i, movingtoEntities)
        tmpMt.push_back(i.Variant());
    (*current)["mtEntities"] = tmpMt;
    (*current)["rsEntities"] = resideEntities;
    return current;
}

QJsonObject* MapNode::toJSON() const
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

QVariantMap* GameMainMap::toVariant() const
{
    QVariantMap* current = new QVariantMap();
    qDebug()<<szN<<szM;
    (*current)["N"] = szN;
    (*current)["M"] = szM;
    QList <QVariant> tmpmap;
    foreach(MapNode i, map)
        tmpmap.append(i.Variant());
    (*current)["mapDt"] = tmpmap;
    QList <QVariant> tmppla;
    foreach(loca i, players)
        tmppla.append(i.Variant());
    (*current)["plaDt"] = tmppla;
    return current;
}

QJsonObject* GameMainMap::toJSON() const
{
    QVariantMap *current = this->toVariant();
    QJsonObject *currentJs = new QJsonObject(QJsonObject::fromVariantMap(*current));
    delete current;
    return currentJs;
}

int GameMainMap::geti(int _num) const
{
    return _num / szM;
}
int GameMainMap::getj(int _num) const
{
    return _num % szM;
}

int GameMainMap::getnum(loca x) const
{
    return getnum(x.x, x.y);
}

GameMainMap::GameMainMap(QVariantMap *fromVariant)
{
    load(fromVariant);
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
        map.push_back(MapNode(i.toMap()));
//    if (map.size() > 20 && map[19].findElement(BOMB)){

//        qDebug() << map[19].resideEntities.size();
//        qDebug() << map[19].getElement(BOMB)->toMap()["isMoving"].toBool();
//        qDebug() << map[19].getElement(BOMB)->toMap()["posTox"].toInt();
//    }

    foreach(QVariant i, tmppla)
        players.push_back(loca(i.toMap()));
    return ;
}

int GameMainMap::getnum(int _i, int _j) const
{
    return _i * szM + _j;
}

int GameMainMap::size() const
{
    return szN * szM;
}

QString Player::intepretPath()
{
    QString ans = ":/";
    ans += playerName[num];
    ans += "/";
    DirectionType decide = isMoving ? direction() : facing;
    switch (decide)
    {
    case DirectionType::LF:
        ans += "left";
        break;
    case DirectionType::DW:
        ans += "down";
        break;
    case DirectionType::RT:
        ans += "right";
        break;
    case DirectionType::UP:
        ans += "up";
        break;
    default:
        ans += "down";
    }
    return ans;
}

QList<QVariant>::iterator MapNode::getElement(EntityType target){
    for (auto now = resideEntities.begin(); now != resideEntities.end(); ++now) {
        if (now->toMap()["entityType"].toInt() == target) {
            return now;
        }
    }
    return resideEntities.end();
}
bool MapNode::findElement(EntityType target){
    return getElement(target) != resideEntities.end();
}

bool MapNode::repWave(GameMainMap *coreData, const WaveEntity &nowWave){
    QList<QVariant>::iterator it;
    findo(it, BOMB, *this) return false;
    findo(it, HARDWALL, *this) return false;
    findo(it, WAVE, *this){
        //bug here: remainTime and damage
        it->toMap()["remainTime"] = qMax(it->toMap()["remainTime"].toInt(), nowWave.remainTime);
        it->toMap()["isHorizontal"] = nowWave.isHorizontal;
        return true;
    }
//    bool swflag = false;
//    findo(it, SOFTWALL, *this){
//        swflag = true;
//        loca tmppos(it->toMap()["posx"].toInt(), it->toMap()["posy"].toInt());
//        resideEntities.erase(it);
//        if(!findElement(ITEM)){
//            int rnow = QRandomGenerator::global()->bounded(4);
//            if (rnow < 4){
//                auto tmpM = ItemEntity(tmppos, 1, (ItemType) rnow).toVariant();
//                resideEntities.push_back(*tmpM);
//                delete tmpM;
//            }
//        }
//    }
//    findo(it, ITEM, *this){
//        if(!swflag)
//            resideEntities.erase(it);
//    }
    resideEntities.push_back(nowWave.Variant());
    return !findElement(SOFTWALL);
}

MapNode& GameMainMap::mapMem(loca x)
{
    return map[getnum(x)];
}

MapNode& GameMainMap::mapMem(int _x, int _y)
{
    return map[getnum(_x, _y)];
}

void Player::releaseBomb(GameMainMap *coreData)
{
//    if (isMoving && stat > 0.2 * STATCOMPLETED)
//        return ;
    MapNode &nowNode = coreData->mapMem(pos);

    auto tmpB = setBomb();
    auto it = nowNode.getElement(PLAYER);
    *it = Variant();
    if (~tmpB.level) {
        if (!isMoving){
            nowNode.resideEntities.push_back(tmpB.Variant());
            return ;
        }
        if (stat < 0.4*STATCOMPLETED) {
            nowNode.resideEntities.push_back(tmpB.Variant());
            return ;
        }
        tmpB.pos = posTo;
        coreData->mapMem(posTo).resideEntities.push_back(tmpB.Variant());
    }
    return ;
}

QList <QVariant>::Iterator GameMainMap::plaMem(int playerNum)
{
    return mapMem(players[playerNum]).getElement(PLAYER);
}

Player GameMainMap::gePlaMem(int playerNum)
{
    auto &tmp = mapMem(players[playerNum]);
    QList <QVariant>::Iterator it;
    findo(it, PLAYER, tmp);
    return Player(it->toMap());
}

void BombEntity::setoff(GameMainMap *coreData){
    coreData->mapMem(pos).repWave(coreData, setWave(pos, -1));
    for (int i = 1; i <= level; i++)
        if (!coreData->mapMem(pos.x+i, pos.y).repWave(coreData, setWave(loca(pos.x+i, pos.y), 0))) break;
    for (int i = 1; i <= level; i++)
        if (!coreData->mapMem(pos.x-i, pos.y).repWave(coreData, setWave(loca(pos.x-i, pos.y), 0))) break;
    for (int i = 1; i <= level; i++)
        if (!coreData->mapMem(pos.x, pos.y+i).repWave(coreData, setWave(loca(pos.x, pos.y+i), 1))) break;
    for (int i = 1; i <= level; i++)
        if (!coreData->mapMem(pos.x, pos.y-i).repWave(coreData, setWave(loca(pos.x, pos.y-i), 1))) break;
    return ;
}

void BombEntity::wear(GameMainMap *coreData)
{
    Entity::wear(coreData);
    qDebug() << remainTime;
    if(!(--remainTime)){
        die(coreData);
        setoff(coreData);
        if(!coreData->isDie(setterNum)){
            Player tmppla(coreData->gePlaMem(setterNum));
            ++tmppla.possessCount;
            tmppla.update(coreData);
        }
    }
    else{
        update(coreData);
        move(coreData);
    }
}


void Player::wear(GameMainMap *coreData)
{
    Entity::wear(coreData);
    move(coreData);
    eatItem(coreData);
}

void WaveEntity::wear(GameMainMap *coreData){
    Entity::wear(coreData);
    qDebug() << remainTime;
    if(!(--remainTime)) die(coreData);
    else update(coreData);
}

//void Player::die(GameMainMap *coreData){
//    auto tmp = coreData->mapMem(pos).getElement(PLAYER);
//    coreData->mapMem(pos).resideEntities.erase(tmp);
//}

void Entity::die(GameMainMap *coreData) const
{
    if(Variant()["isMoving"].toBool())
        coreData->mapMem(Variant()["posTox"].toInt(), Variant()["posToy"].toInt()).movingtoEntities.clear();
    coreData->mapMem(pos).resideEntities.erase(coreData->mapMem(pos).getElement(entityType));
    return ;
}

void Entity::update(GameMainMap *coreData) const
{
    if(entityType == BOMB){
        if(Variant()["isMoving"].toBool())
            int a = 0;
    }
    *coreData->mapMem(pos).getElement(entityType) = Variant();
    return ;
}

void Entity::wear(GameMainMap *coreData)
{
    if (protectTime > 0)
        --protectTime;
    update(coreData);
    return ;
}

bool Entity::damage(GameMainMap *coreData, int _damage)
{
    if (protectTime > 0){
        qDebug() << "protected!" << protectTime;
        return true;
    }
    if ((toughness -= _damage) < 0){
        die(coreData);
        return false;
    }
    else{
        update(coreData);
        return true;
    }
}

void MapNode::generateItem(GameMainMap *coreData, loca pos)
{
    QList<QVariant>::Iterator it;
    findo(it, ITEM, *this){
        ItemEntity tmp(it->toMap());
        tmp.protectTime = DEFAULTRTIMEW + 5;
        tmp.update(coreData);
    }
    else {
        int rnow = QRandomGenerator::global()->bounded(ITEMBOUNDCNT);
        if (rnow < 4)
            resideEntities.push_back(ItemEntity(pos, 1, (ItemType) rnow, DEFAULTRTIMEW + 5).Variant());
    }

    return ;
}

void Player::eatItem(GameMainMap *coreData){
    ItemEntity toGet(loca(), 0, LEVEL);
    if (!isMoving){
        lpNodeMem it;
        findo(it, ITEM, coreData->mapMem(pos))
            toGet = ItemEntity(it->toMap());
    }
    else{
        lpNodeMem it;
        if (stat > 0.2 * STATCOMPLETED)
            findo(it, ITEM, coreData->mapMem(posTo))
                toGet = ItemEntity(it->toMap());
    }
    if (!toGet.pos.isVaild()) return ;
    toGet.die(coreData);

    switch (toGet.itemType) {
    case ItemType::LEVEL:
        level += toGet.value;
        level = qMin(MAXLEVEL, level);
        score += 100;
        break;
    case ItemType::SPEED:
        speedLevel += toGet.value;
        speedLevel = qMin(MAXSPEED, speedLevel);
        score += 100;
        break;
    case ItemType::RICHMENT:
        possessCount += toGet.value;
        possessCount = qMin(MAXPOSSESS, possessCount);
        score += 100;
        break;
    case ItemType::STRENGHTH:
        canMoveBomb = true;
        score += 100;
        break;
    }
    update(coreData);
}

bool BombEntity::beTriggered(GameMainMap *coreData, DirectionType now, int _speed, int _stat)
{
    loca tmpPosTo;
    switch (now) {
    case DirectionType::DW:
        tmpPosTo = loca(pos.x+1, pos.y);
        break;
    case DirectionType::LF:
        tmpPosTo = loca(pos.x, pos.y-1);
        break;
    case DirectionType::RT:
        tmpPosTo = loca(pos.x, pos.y+1);
        break;
    case DirectionType::UP:
        tmpPosTo = loca(pos.x-1, pos.y);
        break;
    default:
        return false;
    }
    auto &nownode = coreData->mapMem(tmpPosTo);

    if (nownode.findElement(SOFTWALL)
            ||nownode.findElement(HARDWALL)
            ||nownode.findElement(PLAYER)
            ||nownode.findElement(ITEM)
            ||!nownode.movingtoEntities.empty())
        return false;
    lpNodeMem it;
    findo(it, BOMB, nownode){
        BombEntity tmpB(it->toMap());
        if(tmpB.isMoving){
            if (tmpB.direction() != direction())
                return false;
            tmpB.speed = qMax(tmpB.speed, _speed);
            tmpB.stat = qMax(tmpB.stat, _stat);
            tmpB.update(coreData);
        }
        else
            if(!tmpB.beTriggered(coreData, now, _speed, _stat)) return false;
    }
    isMoving = true;
    speed = _speed;
    stat = _stat;
    posTo = tmpPosTo;
    nownode.movingtoEntities.push_back(pos);
    update(coreData);
    return true;
}

void BombEntity::move(GameMainMap *coreData)
{
    if (!isMoving) return ;
    auto &nownode = coreData->mapMem(posTo);
    if (nownode.findElement(SOFTWALL)||nownode.findElement(HARDWALL)||nownode.findElement(ITEM)) {
        isMoving = false;
        stat = 0;
        speed = 0;
        posTo = loca();
        update(coreData);
        return ;
    }

    speed = qMax(speed - BOMBSPEEDREDUCING, BOMBSPEEDREDUCING);

    stat += speed;
    lpNodeMem it;
    findo(it, BOMB, coreData->mapMem(posTo)){
        BombEntity tmpB(it->toMap());
        tmpB.speed = qMax(tmpB.speed, speed);
        tmpB.stat = qMax(tmpB.stat, stat);
        tmpB.update(coreData);
    }
    auto tmp = direction();
    auto tmpTo = posTo;
    if (stat >= STATCOMPLETED) {
        stat = 0;
        isMoving = false;
        coreData->mapMem(posTo).movingtoEntities.clear();
        die(coreData);
        pos = posTo;
        coreData->mapMem(pos).resideEntities.push_back(Variant());
        update(coreData);
//        if (speed < 2*BOMBSPEEDREDUCING || (!beTriggered(coreData, tmp, speed, 0))){
            speed = 0;
            isMoving = false;
            posTo = loca();
            update(coreData);
            return ;
//        }
//        else{
//            isMoving = true;
//            posTo = tmpTo;
//            update(coreData);
//            return move(coreData);
//        }
    }
    qDebug() << "nowBOMBSpeed:" << speed;
    if (isMoving)
    qDebug() << "moving in Server!";
    update(coreData);
    return ;
}

void Player::move(GameMainMap *coreData)
{
    DirectionType now = DirectionType::NE;
    if (isPressed[DirectionType::UP])
        int a = 4;
    for (int i = 0; i < 5; i++) {
        if (i == DirectionType::NE) continue;
        if (isPressed[i]){
            now = (DirectionType) i;
            if(QRandomGenerator::global()->bounded(10)>9)
                break;
        }
    }
    if (!isMoving) {
        loca tmpPosTo;
        switch (now) {
        case DirectionType::DW:
            tmpPosTo = loca(pos.x+1, pos.y);
            break;
        case DirectionType::LF:
            tmpPosTo = loca(pos.x, pos.y-1);
            break;
        case DirectionType::RT:
            tmpPosTo = loca(pos.x, pos.y+1);
            break;
        case DirectionType::UP:
            tmpPosTo = loca(pos.x-1, pos.y);
            break;
        case DirectionType::NE:
            speed = 0;
            return ;
        }
        DirectionType fd = facing;
        facing = now;
        update(coreData);
        auto &nownode = coreData->mapMem(tmpPosTo);

        if (nownode.findElement(SOFTWALL)
                ||nownode.findElement(HARDWALL)
                ||nownode.findElement(PLAYER)
                ||!nownode.movingtoEntities.empty())
            return ;
        lpNodeMem it;
        findo(it, BOMB, nownode){
            if (!canMoveBomb) return ;
            BombEntity tmpB(it->toMap());
            if (tmpB.isMoving) {
                if (tmpB.direction() != direction())
                    return ;
                tmpB.speed = qMax(tmpB.speed, speed);
                tmpB.stat = qMax(tmpB.stat, stat);
                tmpB.update(coreData);
            }
            else if(!tmpB.beTriggered(coreData, facing, speed, stat))
                return ;
        }
        isMoving = true;
        posTo = tmpPosTo;
        if (fd != direction())
            speed = 0;
        nownode.movingtoEntities.push_back(pos);
        update(coreData);
    }
    else {
        auto &nownode = coreData->mapMem(posTo);
        facing = direction();
        if (nownode.findElement(SOFTWALL)||nownode.findElement(HARDWALL)) {
            isMoving = false;
            stat = 0;
            speed = 0;
            posTo = loca();
            update(coreData);
            return ;
        }
        if (direction() + now != 4 && direction() != now) goto updst;
        if (direction() != now) {
            speed -= speedLevel * ACCSPEEDMULTIPLES;
            if (speed < 0) {
                speed = -speed;
                speed = qMin(speedLevel * MAXSPEEDMULTIPLES, speed);
                stat = STATCOMPLETED - stat;
                coreData->mapMem(posTo).movingtoEntities.clear();
                coreData->mapMem(pos).movingtoEntities.push_back(posTo);
                die(coreData);
                std::swap(pos, posTo);
                facing = direction();
                coreData->mapMem(pos).resideEntities.push_back(Variant());
                coreData->players[num] = pos;
            }
        }
        else speed = qMin(speedLevel * MAXSPEEDMULTIPLES, speed + speedLevel * ACCSPEEDMULTIPLES);

updst:  stat += speed;
        lpNodeMem it;
        findo(it, BOMB, coreData->mapMem(posTo)){
            BombEntity tmpB(it->toMap());
            tmpB.speed = qMax(tmpB.speed, speed);
            tmpB.stat = qMax(tmpB.stat, stat);
            tmpB.update(coreData);
        }
        if (stat >= STATCOMPLETED) {
            stat = 0;
            facing = direction();
            isMoving = false;
            coreData->mapMem(posTo).movingtoEntities.clear();
            die(coreData);
            pos = posTo;
            coreData->mapMem(pos).resideEntities.push_back(Variant());
            coreData->players[num] = pos;
            update(coreData);
            return move(coreData);
        }
        qDebug() << "nowSpeed:" << speed;
        update(coreData);
    }
}

bool GameMainMap::isDie(int playerNum){
    return !players[playerNum].isVaild();
}

void Player::changeScore(int addS, GameMainMap *coreData)
{
    score += addS;
    update(coreData);
}

void MoveableEntity::stop(GameMainMap *coreData)
{
    coreData->mapMem(posTo).movingtoEntities.clear();
    isMoving = false;
    posTo = loca();
    stat = speed = 0;
    update(coreData);
}

bool MoveableEntity::isBlocked(GameMainMap *coreData)
{
    if (!isMoving) return false;
    if(coreData->mapMem(posTo).findElement(HARDWALL)
        || coreData->mapMem(posTo).findElement(SOFTWALL)
        || coreData->mapMem(posTo).findElement(BOMB)
        || coreData->mapMem(posTo).findElement(PLAYER)
        || (!coreData->mapMem(posTo).movingtoEntities.empty() && !(coreData->mapMem(posTo).movingtoEntities[0] == pos)))
        return true;
    return false;
}

bool GameMainMap::isSomeoneBomb(int playerNum)
{
    for (int i = 0; i < szN; i++)
        for (int j = 0; j < szM; j++)
        {
            lpNodeMem it;
            findo(it, BOMB, mapMem(i, j))
                if (it->toMap()["setterNum"].toInt() == playerNum)
                    return true;
        }
    return false;
}
