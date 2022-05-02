#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include "serverthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef QAction* lpQAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

    //Server Controlling Signals
    void doStartServer();
    void doCloseServer();

    //Game Actions
    void doSave(QString filename);
    void doLoad(QString filename);
    void doQuickSave();
    void doQuickLoad();
    void doRestart();
    void doSuspend();
    void doQuit();
    void doExit();

    //Effect Actions
    void doVolumeup();
    void doVolumedown();
    void doSilence();

    //C&A Actions
    void doConfigure();
    void doAdvancedOptions();

    //Help Actions
    void Document();
    void About();

protected:

    void closeEvent(QCloseEvent *event){
        opCloseServer();
        QMainWindow::closeEvent(event);
        return ;
    }

private:
    Ui::MainWindow *ui;
    //A subclass responsible for sending JSON to server
    class SendMessageManager;
    friend class SendMessageManager;
    SendMessageManager *smr;

    //Actions
//    lpQAction game, effect, cao, help;
//    lpQAction savs, loas;
    lpQAction save, load, quickSave, quickLoad, pause, restart, quit, exit;
    lpQAction volumeup, volumedown, silence, configure, restartServer, advancedOptions;
    lpQAction document, about;


    QLabel *cStatus, *sStatus;

    int serverSign;
    LocalServer *server;
    QThread *localServerWorker;

    QPushButton *startButton;

    void startGame();
    void exitGame();
    void suspendGame();
    void onServerChange(QString neuCaption);
    void onClientChange(QString neuCaption);

    void opRestartServer();
    //should not be called by the user
    void opStartServer();
    void opCloseServer();

};

class MainWindow::SendMessageManager : public QObject
{
    Q_OBJECT
private:
    MainWindow* p;
    //Game Actions
    void sendSave(QString filename);
    void sendLoad(QString filename);
    void sendQuickSave();
    void sendQuickLoad();
    void sendRestart();
    void sendQuit();
    void sendExit();

    //Effect Actions
    void sendVolumeup();
    void sendVolumedown();
    void sendSilence();

    //C&A Actions
    void sendConfigure();
    void sendRestartServer();
    void sendAdvancedOptions();

    //Help Actions
    void Document();
    void About();

public:
    SendMessageManager(MainWindow *father)
        : QObject(father)
        , p(father)
    {
        connect(father, &MainWindow::doExit, this, &SendMessageManager::sendExit);
//        connect(father, &MainWindow::doExit, this, &SendMessageMananger::sendExit);
//        connect(father, &MainWindow::dot, this, &SendMessageMananger::sendExit);
//        connect(father, &MainWindow::doQuit, this, &SendMessageMananger::sendQuit);
//        connect(father, &MainWindow::doExit, this, &SendMessageMananger::sendExit);

//        connect(father, &MainWindow::doExit, this, &SendMessageMananger::sendExit);
//        connect(father, &MainWindow::doExit, this, &SendMessageMananger::sendExit);
    }

};

#endif // MAINWINDOW_H
