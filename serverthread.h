#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QTimer>

#include "server/component.h"

const int TICK = 200;

class LocalServer : public QObject
{
    Q_OBJECT
public:
    LocalServer(QObject *parent = nullptr);
    QJsonObject *queryBucket;
    GameMainMap *coreData;
    bool isRunning;

private slots:
    void run();

private:
    QTimer *tickStimulator;

public slots:
    void start();
    void close();
    void suspend();
    void query();
    int load();
    int save();
    int qload();
    int qsave();

signals:
    void over(int exitNum=-1);
    void queryFinished(QJsonObject now);
    void changeServerStatus(QString caption);
};


#endif // SERVERTHREAD_H
