#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QPushButton>

#include "mainwindow.h"
#include "maptdialog.h"
#include "./ui_mainwindow.h"
#include "gitems/graphicbuttons.h"
#include "graphicsview.h"
#include <QGraphicsItem>
#include <QTime>


//void MainWindow::SendMessageManager::sendExit(){
//    return ;
//}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , userConfig()
//    , smr(new SendMessageManager(this))
{
    ui->setupUi(this);

    setWindowTitle(tr("PPT - MAINGAME"));

    //to-do: read the config.json

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

    quit = new QAction(tr("Quit"), this);
    quit->setShortcut(tr("Q"));
    game->addAction(quit);
    connect(quit, &QAction::triggered, this, &MainWindow::quitGame);

    exit = new QAction(tr("Exit"), this);
    exit->setShortcut(tr("Esc"));
    game->addAction(exit);
    connect(exit, &QAction::triggered, this, &MainWindow::exitGame);

    QMenu *effect = menuBar()->addMenu(tr("&Effect"));

    QMenu *cao = menuBar()->addMenu(tr("&Configure and Advanced Options"));

    maptrans = new QAction(tr("Map Translation"));
    maptrans->setShortcut(tr("Ctrl+T"));
    cao->addAction(maptrans);
    connect(maptrans, &QAction::triggered, this, &MainWindow::openMaptrans);

    QMenu *help = menuBar()->addMenu(tr("&Help"));

    //    QMenu *
//    game = new QAction(tr("&Game"))

    gameSpace = new QGraphicsScene();
    menuScene = new QGraphicsScene();

    overview = new GraphicsView(this);
    overview->setScene(menuScene);
    overview->setBackgroundBrush(QBrush(QPixmap(tr(":/images/mapics/fbsea"))));
    GraphicButtons *stbt = new GraphicButtons(QPoint(0,0), QPixmap(tr(":/buttons/stbt")));
    menuScene->addItem(stbt);
    connect(stbt, &GraphicButtons::clicked, this, &MainWindow::startGame);
    setCentralWidget(overview);

//    startButton = new QPushButton(tr("Start Game"), this);

//    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);


    statusBar();
    cStatus = new QLabel(tr("Client: Not connected"));
//    cStatus->setMargin(1);
    sStatus = new QLabel(tr("Server: Not running"));
//    sStatus->setMargin(1);
    statusBar()->addPermanentWidget(cStatus);
    statusBar()->addPermanentWidget(sStatus);


    //start the server thread
    opStartServer();

    player1 = new PlayerInfo(Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_Space, "Player 1");
    player2 = new PlayerInfo(Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Return, "Player 2");

    qDebug() << "Done!";
}

void MainWindow::startGame(){
//    server->run();
    qDebug()<<"Received Game Start!";

    emit doGeneratePlayer(false, loca(1, 1), player1->name, 1);
    emit doGeneratePlayer(false, loca(13, 15), player2->name, 2);
    emit doGeneratePlayer(true, loca(1, 15), "AI1", 3);
    emit doGeneratePlayer(true, loca(13, 1), "AI2", 4);
    overview->setScene(gameSpace);
}

void MainWindow::quitGame(){
    overview->setScene(menuScene);
    opRestartServer();
}

void MainWindow::restartGame(){
    quitGame();
    startGame();
    return ;
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
    connect(this, &MainWindow::doPlayerBehavior, server, &LocalServer::changePlayerBehavior);
    connect(this, &MainWindow::doGeneratePlayer, server, &LocalServer::generatePlayer);
    connect(this, &MainWindow::doReleaseBomb, server, &LocalServer::releaseBomb);


    connect(server, &LocalServer::over, localServerWorker, &QThread::exit);
    connect(localServerWorker, &QThread::finished, localServerWorker, &QThread::deleteLater);
    connect(server, &LocalServer::changeServerStatus, this, &MainWindow::onServerChange);
    connect(server, &LocalServer::queryFinished, this, &MainWindow::advance);
    connect(server, &LocalServer::playerGenerationCompleted, this, &MainWindow::getPlayerNum);

    localServerWorker->start();
    emit doStartServer();
    onClientChange("Local Server Connected");
    serverSign = 1;

    if (userConfig.enableAutoQload)
        emit doQuickLoad();

    return ;
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


void MainWindow::openMaptrans(){
    maptDialog *win = new maptDialog(this);
    win->show();
    return ;
}

void MainWindow::openHelpDialog(){
//    HelpDialog *win = new HelpDialog(this);

}

void MainWindow::advance(QJsonObject now)
{
    clock_t st = clock();
    auto tmp = now.toVariantMap();
    GameMainMap v(&tmp);
    gameSpace->clear();

    const qreal frlength = 40.0;
    for (int i = 0; i < v.szN; i++) {
        for (int j = 0; j < v.szM; j++) {
            auto nowit = gameSpace->addPixmap(QPixmap(tr(":/images/mapics/fbsea")));
            nowit->setPos(QPointF(frlength*j, frlength*i));
        }
    }

    for (int i = 0; i < v.szN; i++) {
        for (int j = 0; j < v.szM; j++) {
            foreach (auto k, v.mapMem(i, j).resideEntities) {
                if (k.toMap()["entityType"].toInt() == ITEM){
                    QGraphicsPixmapItem *nowit;
                    switch ((ItemType) k.toMap()["itemType"].toInt()) {
                    case ItemType::LEVEL:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/lvlup")));
                        break;
                    case ItemType::SPEED:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/spd")));
                        break;
                    case ItemType::RICHMENT:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/bombu")));
                        break;
                    case ItemType::STRENGHTH:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/mvbomb")));
                        break;
                    }

                    QSizeF tmpsz = nowit->boundingRect().size();
                    qreal offsetx = (40 - tmpsz.width()) / 2;
                    qreal offsety = 40 - tmpsz.height();
                    nowit->setPos(QPointF(frlength*j + offsetx, frlength*i + offsety));
                }
            }
        }
    }

    for (int i = 0; i < v.szN; i++) {
        for (int j = 0; j < v.szM; j++) {
            foreach (auto k, v.mapMem(i, j).resideEntities) {
                if (k.toMap()["entityType"].toInt() == SOFTWALL) {
                    auto nowit = gameSpace->addPixmap(QPixmap(tr(":/images/mapics/swsea")));
                    nowit->setPos(QPointF(frlength*j, frlength*i - 12));
                }
                if (k.toMap()["entityType"].toInt() == HARDWALL) {
                    auto nowit = gameSpace->addPixmap(QPixmap(tr(":/images/mapics/hdsea")));
                    nowit->setPos(QPointF(frlength*j - 1, frlength*i - 12));
                }
                if (k.toMap()["entityType"].toInt() == BOMB) {
                    auto nowit = gameSpace->addPixmap(QPixmap(tr(":/items/bomb")));
                    auto tmp = k.toMap();
                    auto tmpP = BombEntity(&tmp);
                    qreal offsetx = 1.0, offsety = 2.0;
                    if (tmpP.isMoving){
                        if(tmpP.stat < 0 || tmpP.stat > 10000)
                            int a = 0;
                        qDebug() << "moving!" ;
                        DirectionType movd = tmpP.direction();
                        switch (movd){
                        case LF:
                            nowit->setPos(QPointF(frlength * (j - tmpP.stat/STATCOMPLETED) + offsetx, frlength * i + offsety));
                            qDebug() << (j - tmpP.stat/STATCOMPLETED) + offsetx;
                            break;
                        case UP:
                            nowit->setPos(QPointF(frlength*j + offsetx, frlength * (i - tmpP.stat/STATCOMPLETED) + offsety));
                            break;
                        case RT:
                            qDebug() << frlength * (j + tmpP.stat/STATCOMPLETED) + offsetx;
                            nowit->setPos(QPointF(frlength * (j + tmpP.stat/STATCOMPLETED) + offsetx, frlength * i + offsety));
                            break;
                        case DW:
                            if (tmpP.stat > 5000){
                                int a = 0;
                            }
                            nowit->setPos(QPointF(frlength*j + offsetx, frlength * (i + tmpP.stat/STATCOMPLETED) + offsety));
                            qDebug() << frlength * (i + tmpP.stat/STATCOMPLETED) + offsety;
                            break;
                        default:
                            qDebug() << "error: direction undefined";
                        }
                    }
                    else nowit->setPos(QPointF(frlength*j + 1, frlength*i + 2));
                }
                if (k.toMap()["entityType"].toInt() == PLAYER) {
                    auto tmp = k.toMap();
                    auto tmpP = Player(&tmp);
                    auto nowit = gameSpace->addPixmap(QPixmap(tmpP.intepretPath()));
                    qreal offsetx, offsety;
                    QSizeF tmpsz = nowit->boundingRect().size();
                    offsetx = (40 - tmpsz.width()) / 2;
                    offsety = 40 - tmpsz.height();
                    if (tmpP.isMoving){
                        DirectionType movd = tmpP.direction();
                        switch (movd){
                        case LF:
                            nowit->setPos(QPointF(frlength * (j - tmpP.stat/STATCOMPLETED) + offsetx, frlength * i + offsety));
                            qDebug() << (j - tmpP.stat/STATCOMPLETED) + offsetx;
                            break;
                        case UP:
                            nowit->setPos(QPointF(frlength*j + offsetx, frlength * (i - tmpP.stat/STATCOMPLETED) + offsety));
                            break;
                        case RT:
                            nowit->setPos(QPointF(frlength * (j + tmpP.stat/STATCOMPLETED) + offsetx, frlength * i + offsety));
                            break;
                        case DW:
                            nowit->setPos(QPointF(frlength*j + offsetx, frlength * (i + tmpP.stat/STATCOMPLETED) + offsety));
                            break;
                        default:
                            qDebug() << "error: direction undefined";
                        }
                    }
                    else
                        nowit->setPos(QPointF(frlength*j + offsetx, frlength*i + offsety));
                }
                if (k.toMap()["entityType"].toInt() == WAVE) {
                    QGraphicsPixmapItem *nowit;
                    switch (k.toMap()["isHorizontal"].toInt()) {
                    case 1:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/wavh")));
                        nowit->setPos(QPointF(frlength*j, frlength*i));
                        break;
                    case 0:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/wavv")));
                        nowit->setPos(QPointF(frlength*j, frlength*i));
                        break;
                    case -1:
                        nowit = gameSpace->addPixmap(QPixmap(tr(":/items/wavc")));
                        nowit->setPos(QPointF(frlength*j, frlength*i));
                        break;
                    }
                }
            }
//            foreach (auto k, v.map[v.getnum(i, j)].movingtoEntities) {

//            }
        }
    }
    clock_t en = clock();
    qDebug()<<"Tick elapsed(Advance): "<<en-st<<" ms";
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(player1->isKeyDown(event)) emit doReleaseBomb(player1->playerNum);
    if(player2->isKeyDown(event)) emit doReleaseBomb(player2->playerNum);
    auto tmp1 = player1->getDirection(event);
    auto tmp2 = player2->getDirection(event);
    if (tmp1 != DirectionType::NE)
        emit doPlayerBehavior(true, player1->playerNum, tmp1);
    if (tmp2 != DirectionType::NE)
        emit doPlayerBehavior(true, player2->playerNum, tmp2);
    return ;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    auto tmp1 = player1->getDirection(event);
    auto tmp2 = player2->getDirection(event);
    if (tmp1 != DirectionType::NE)
        emit doPlayerBehavior(false, player1->playerNum, tmp1);
    if (tmp2 != DirectionType::NE)
        emit doPlayerBehavior(false, player2->playerNum, tmp2);
}

void MainWindow::getPlayerNum(int toDe, int num){
    if (toDe == 1)
        player1->setnum(num);
    if (toDe == 2)
        player2->setnum(num);
    return ;
}

MainWindow::~MainWindow()
{
    if (serverSign != -1)
        opCloseServer();
    delete ui;
    delete gameSpace;
    delete menuScene;
}

