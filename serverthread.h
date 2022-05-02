#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QTimer>

const int TICK = 200;

class LocalServer : public QObject
{
    Q_OBJECT
public:
    LocalServer(QObject *parent = nullptr);

private slots:
    void run();

private:
    bool isRunning;
    QTimer *tickStimulator;

public slots:
    void start();
    void close();
    void suspend();
    void query();

signals:
    void done(int exitNum=-1);
    void queryFinished();
    void changeServerStatus(QString caption);
};


#endif // SERVERTHREAD_H
