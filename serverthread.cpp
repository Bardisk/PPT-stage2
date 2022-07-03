#include "serverthread.h"
#include <ctime>
#include <QDebug>
#include <ai.h>

LocalServer::LocalServer(QObject *parent)
    : QObject(parent)
    , queryBucket()
    , coreData(new GameMainMap())
    , isRunning(false)
    , tickStimulator(new QTimer(this))

{
    tickStimulator->start(TICK);
    for (int i = 0; i < 10; i++)
        Ailist[i] = nullptr;
    load("../PPT-stage2/defmap.json");
}

void LocalServer::run()
{
    if (!isRunning) return ;
    clock_t st = clock();
    qDebug() << "run!";

    int liveCnt=0, lastliver;
    if(!coreData->players.size()) return ;
    for (int i = 0; i < coreData->players.size(); i++)
    {
        if (coreData->isDie(i)) continue;
        liveCnt++;
        lastliver = i;
        auto tmp = coreData->gePlaMem(i);
        if (tmp.isAi){
            if (Ailist[tmp.num] == nullptr)
                Ailist[tmp.num] = new Ai(i);
            DirectionType tmpD = Ailist[tmp.num]->Decision(coreData);
            if (tmpD == NE) continue;
            if (tmpD == BNB) {
                if (coreData->isSomeoneBomb(i))
                    continue;
                releaseBomb(i);
                continue;
            }
            else{
                changePlayerBehavior(true, i, tmpD);
            }
        }
    }
    if (!liveCnt) {
        emit gameEnd(-1);
        return ;
    }
    if (liveCnt == 1) {
        emit gameEnd(lastliver);
        return ;
    }

    clock_t en1 = clock();
    qDebug()<<"Tick elapsed(Run1): "<<en1-st<<" ms";

    for (int i = 0; i < coreData->size(); i++){
        auto &now = coreData->map[i];
        QList<QVariant>::Iterator it;
        bool isHereWave = false;
        int setterNum = -1;
        QList<QVariant>::Iterator wave;
        int wdamage = 0;
        findo(it, WAVE, now){
            wdamage = it->toMap()["damage"].toInt();
            setterNum = it->toMap()["setterNum"].toInt();
            if(setterNum < 0 || setterNum > 3) qDebug()<<"ERROR!";
            WaveEntity(it->toMap()).wear(coreData);
            isHereWave = true;
        }
        findo(it, PLAYER, now)
            Player(it->toMap()).wear(coreData);
        findo(it, BOMB, now)
            BombEntity(it->toMap()).wear(coreData);
        findo(it, ITEM, now)
            ItemEntity(it->toMap()).wear(coreData);
        findo(it, SOFTWALL, now)
            Entity(it->toMap()).wear(coreData);
        findo(it, HARDWALL, now)
            Entity(it->toMap()).wear(coreData);
        if (isHereWave){
            findo(it, PLAYER, now){
                int num = it->toMap()["num"].toInt();
                if (!Player(it->toMap()).damage(coreData, wdamage)) {
                    coreData->players[num] = loca(-1, -1);
                    if (coreData->players[setterNum].isVaild()){
                        Player tmppla(coreData->plaMem(setterNum)->toMap());
                        tmppla.changeScore(1000, coreData);
                    }
                    emit playerDie(num);
                }
            }
            findo(it, SOFTWALL, now)
                if (!Entity(it->toMap()).damage(coreData, wdamage)){
                    if (coreData->players[setterNum].isVaild()){
                        Player tmppla(coreData->plaMem(setterNum)->toMap());
                        tmppla.changeScore(100, coreData);
                    }
                    coreData->map[i].generateItem(coreData, coreData->getpos(i));
                }

            findo(it, ITEM, now)
                Entity(it->toMap()).damage(coreData, wdamage);
        }
    }

    clock_t en2 = clock();
    qDebug()<<"Tick elapsed(Run2): "<<en2-st<<" ms";

    query();
    clock_t en = clock();
    qDebug()<<"Tick elapsed(Run): "<<en-st<<" ms";
    return ;
}

void LocalServer::start(){
    emit changeServerStatus(tr("Starting"));
    connect(tickStimulator, &QTimer::timeout, this, &LocalServer::run);
    emit changeServerStatus(tr("Running"));
    isRunning = true;
    return ;
}

void LocalServer::suspend(){
    if (!isRunning) {
        emit changeServerStatus(tr("Running"));
        connect(tickStimulator, &QTimer::timeout, this, &LocalServer::run);
        isRunning = true;
    }
    else {
        emit changeServerStatus(tr("Suspended"));
        disconnect(tickStimulator, &QTimer::timeout, this, &LocalServer::run);
        isRunning = false;
    }
    return ;
}

void LocalServer::query(){
    if (!isRunning) return ;
//    while(1);
    clock_t st = clock();
    queryBucket = coreData->toJSON();
    clock_t en = clock();
    emit queryFinished(*queryBucket);
    qDebug()<<"Tick elapsed(Query): "<<en-st<<" ms";
    return ;
}

void LocalServer::close(){
    emit changeServerStatus(tr("Closing"));
    tickStimulator->stop();
    delete tickStimulator;
    //tell the mainwindow it has closed
    emit changeServerStatus(tr("Closed"));
    isRunning = false;
    //call the thread to exit
//    emit over(0);
    return ;
}

int LocalServer::load(QString filename){
    QFile qsavdata(filename);
    if(!qsavdata.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "fatal: file open failed";
        return -1;
    }
    QByteArray readStuff = qsavdata.readAll();
    qsavdata.close();
    QVariant tmp = QJsonDocument::fromJson(readStuff).toVariant();
    QVariantMap tmpMap = tmp.toMap();
    coreData->load(&tmpMap);
    return 0;
}
int LocalServer::save(QString filename){
    query();
    QJsonDocument tmp(*queryBucket);
    QByteArray writeStuff = tmp.toJson();
    QFile qsavdata(filename);
    if (!qsavdata.exists()){
        if (!qsavdata.open(QIODevice::NewOnly)) {
            qDebug() << "fatal: file create failed";
            return -1;
        }
        qsavdata.close();
    }
    if (!qsavdata.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "fatal: file open failed";
        return -1;
    }
    qsavdata.write(writeStuff);
    qsavdata.close();
    return 0;
}

int LocalServer::qload()
{
    return load("../PPT-stage2/savs/savq.json");
}

int LocalServer::qsave()
{
    return save("../PPT-stage2/savs/savq.json");
}

void LocalServer::changePlayerBehavior(bool isPress, int playerNum, DirectionType direction)
{
    if (!~playerNum)
        return ;
    if (direction == DirectionType::NE) return ;
    if (!coreData->players[playerNum].isVaild())
        return ;
    auto tmp = coreData->plaMem(playerNum);
    Player tmpPla(tmp->toMap());
    if (isPress) memset(tmpPla.isPressed, 0, sizeof(tmpPla.isPressed));
    tmpPla.isPressed[direction] = isPress;
    *tmp = tmpPla.Variant();
    return ;
}

void LocalServer::generatePlayer(bool isAi, loca pos, QString name, int toDe){
    int nowPlayerNum = coreData->players.size();
    Player tmpNewPlayer(pos, nowPlayerNum, name, isAi);
    coreData->players.push_back(pos);
    coreData->mapMem(pos).resideEntities.push_back(tmpNewPlayer.Variant());
    emit playerGenerationCompleted(nowPlayerNum+1, nowPlayerNum);
}

void LocalServer::releaseBomb(int playerNum){
    if (!~playerNum)
        return ;
    if (!coreData->players[playerNum].isVaild())
        return ;
    auto tmp = coreData->gePlaMem(playerNum);
//    if (tmp.isMoving)
//        return ;
    tmp.releaseBomb(coreData);
    qDebug() << "receive setBomb!" << playerNum;
}
