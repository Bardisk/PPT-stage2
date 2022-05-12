#include "serverthread.h"
#include <ctime>
#include <QDebug>

LocalServer::LocalServer(QObject *parent)
    : QObject(parent)
    , queryBucket()
    , coreData(new GameMainMap())
    , isRunning(false)
    , tickStimulator(new QTimer(this))
{
    tickStimulator->start(TICK);
}

void LocalServer::run(){
    if (!isRunning) return ;
    clock_t st = clock();
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
    emit queryFinished();
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

int LocalServer::qload()
{
    QFile qsavdata("savs/savq.json");
    if(!qsavdata.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "fatal: file open failed";
        return -1;
    }
    QByteArray readStuff = qsavdata.readAll();
    qsavdata.close();
    QVariant tmp = QJsonDocument::fromJson(readStuff).toVariant();
    QVariantMap tmpMap = tmp.toMap();
//    coreData->load(&tmpMap);
    return 0;
}

int LocalServer::qsave()
{
    query();
    QJsonDocument tmp(*queryBucket);
    QByteArray writeStuff = tmp.toJson();
    QFile qsavdata("savs/savq.json");
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
