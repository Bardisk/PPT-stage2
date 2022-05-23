#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QTimer>

#include "server/component.h"

const int TICK = 25;

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
    int load(QString filename);
    int save(QString filename);
    int qload();
    int qsave();
    void changePlayerBehavior(bool isPress, int playerNum, DirectionType direction);
    void generatePlayer(bool isAi, loca pos, QString name, int toDe);
    void releaseBomb(int playerNum);


signals:
    void sendnum(int open);
    void over(int exitNum=-1);
    void queryFinished(QJsonObject now);
    void changeServerStatus(QString caption);
    void playerGenerationCompleted(int toDe, int num=-1);
    void playerDie(int num);
};


#endif // SERVERTHREAD_H
