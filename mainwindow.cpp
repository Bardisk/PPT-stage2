#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QPushButton>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTime>

void MainWindow::SendMessageManager::sendExit(){
    return ;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , smr(new SendMessageManager(this))
{
    ui->setupUi(this);

    setWindowTitle(tr("PPT - MAINGAME"));

    QMenu *game = menuBar()->addMenu(tr("&Game"));
    QMenu *savs = game->addMenu(tr("&Save"));
    QMenu *loas = game->addMenu(tr("&Load"));

    save = new QAction(tr("Save As"), this);
    save->setShortcut(tr("Ctrl+Shift+S"));
    save->setStatusTip(tr("Save as a given file"));
    quickSave = new QAction(tr("Quick Save"), this);
    quickSave->setShortcut(QKeySequence::Save);
    savs->addAction(quickSave);
    connect(quickSave, &QAction::triggered, this, &MainWindow::doQuickSave);
    savs->addAction(save);

    load = new QAction(tr("Load From"), this);
    load->setShortcut(tr("Ctrl+Shift+L"));
    quickLoad = new QAction(tr("Quick Load"), this);
    quickLoad->setShortcut(tr("Ctrl+L"));
    loas->addAction(quickLoad);
    connect(quickLoad, &QAction::triggered, this, &MainWindow::doQuickLoad);
    loas->addAction(load);

    pause = new QAction(tr("Pause | Resume"), this);
    pause->setShortcut(tr("P"));
    game->addAction(pause);
    connect(pause, &QAction::triggered, this, &MainWindow::doSuspend);

    exit = new QAction(tr("Exit"), this);
    exit->setShortcut(tr("Esc"));
    game->addAction(exit);
    connect(exit, &QAction::triggered, this, &MainWindow::exitGame);

    QMenu *effect = menuBar()->addMenu(tr("&Effect"));

    QMenu *cao = menuBar()->addMenu(tr("&Configure and Advanced Options"));

    QMenu *help = menuBar()->addMenu(tr("&Help"));

    //    QMenu *
//    game = new QAction(tr("&Game"))


    startButton = new QPushButton(tr("Start Game"), this);
    setCentralWidget(startButton);
    connect(startButton, &QPushButton::pressed, this, &MainWindow::startGame);


    statusBar();
    cStatus = new QLabel(tr("Client: Not connected"));
//    cStatus->setMargin(1);
    sStatus = new QLabel(tr("Server: Not running"));
//    sStatus->setMargin(1);
    statusBar()->addPermanentWidget(cStatus);
    statusBar()->addPermanentWidget(sStatus);

    //start the server thread
    opStartServer();


    qDebug() << "Done!";
}

void MainWindow::startGame(){
//    server->run();
    qDebug()<<"Received Game Start!";
}
void MainWindow::exitGame(){
    if (serverSign != -1)
        opCloseServer();
    emit doExit();
    return ;
}

void MainWindow::opStartServer(){
    if (serverSign == 1) return ;
    server = new LocalServer();
    localServerWorker = new QThread(this);
    server->moveToThread(localServerWorker);
    connect(this, &MainWindow::doStartServer, server, &LocalServer::start);
    connect(this, &MainWindow::doCloseServer, server, &LocalServer::close);
    connect(this, &MainWindow::doSuspend, server, &LocalServer::suspend);
    connect(this, &MainWindow::doQuickSave, server, &LocalServer::qsave);
    connect(this, &MainWindow::doQuickLoad, server, &LocalServer::qload);

    connect(server, &LocalServer::over, localServerWorker, &QThread::exit);
    connect(localServerWorker, &QThread::finished, localServerWorker, &QThread::deleteLater);
    connect(server, &LocalServer::changeServerStatus, this, &MainWindow::onServerChange);

    localServerWorker->start();
    emit doStartServer();
    onClientChange("Local Server Connected");
    serverSign = 1;
}


void MainWindow::opCloseServer(){
    if (serverSign == -1) return ;
    emit doCloseServer();
    onClientChange("Not Connected");
    QThread::msleep(50);
    delete server;
    localServerWorker->exit(0);
    serverSign = -1;
    return ;
}

void MainWindow::opRestartServer(){
    opCloseServer();
    QTimer::singleShot(0, this, &MainWindow::opStartServer);
    return ;
}



void MainWindow::onClientChange(QString neuCaption){
    cStatus->setText(neuCaption);
    return ;
}
void MainWindow::onServerChange(QString neuCaption){
    qDebug()<<neuCaption;
    sStatus->setText(neuCaption);
    return ;
}


MainWindow::~MainWindow()
{
    if (serverSign != -1)
        opCloseServer();
    delete ui;
}

