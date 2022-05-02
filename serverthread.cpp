#include "serverthread.h"
#include <ctime>
#include <QDebug>

LocalServer::LocalServer(QObject *parent)
    : QObject(parent)
    , isRunning(false)
    , tickStimulator(new QTimer(this))
{
    tickStimulator->start(TICK);
}

void LocalServer::run(){
    clock_t st = clock();
    clock_t en = clock();
    qDebug()<<"Tick elapsed(Run): "<<en-st<<" ms";
    return ;
}

void LocalServer::start(){
    emit changeServerStatus(tr("Starting"));
    connect(tickStimulator, &QTimer::timeout, this, &LocalServer::run);
    emit changeServerStatus(tr("Running"));
    return ;
}

void LocalServer::suspend(){
    if(!isRunning){
        emit changeServerStatus(tr("Running"));
        connect(tickStimulator, &QTimer::timeout, this, &LocalServer::run);
        isRunning = true;
    }
    else{
        emit changeServerStatus(tr("Suspended"));
        disconnect(tickStimulator, &QTimer::timeout, this, &LocalServer::run);
        isRunning = false;
    }
    return ;
}

void LocalServer::query(){
    clock_t st = clock();
    clock_t en = clock();
    emit queryFinished();
    qDebug()<<"Tick elapsed(Query): "<<en-st<<" ms";
    return ;
}

void LocalServer::close(){

    emit changeServerStatus(tr("Closing"));
    //tell the mainwindow it has closed
    emit changeServerStatus(tr("Closed"));
    this->deleteLater();
    //call the thread to exit
    emit done(0);
    return ;
}
