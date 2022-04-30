#include "mainwindow.h"

#include <gameobject.h>
#include <imagetransform.h>
#include <shooter.h>
#include <summondummy.h>
#include <transformbuilder.h>

#include "./ui_mainwindow.h"

void loadScene(GameScene *gameScene) {
  auto shooter = new GameObject();
  ImageTransformBuilder()
      .setPos(QPointF(100, 100))
      .setImage("enemy_3_0.png")
      .setAlignment(Qt::AlignCenter)
      .addToGameObject(shooter);
  shooter->addComponent(new Shooter);
  gameScene->attachGameObject(shooter);

  auto summonDummyBtn = new GameObject();
  summonDummyBtn->addComponent(new Transform);
  summonDummyBtn->addComponent(new SummonDummy);
  gameScene->attachGameObject(summonDummyBtn);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  gameScene = new GameScene();
  gameScene->setParent(this);
  auto view = new QGraphicsView(gameScene, this);
  view->setFrameStyle(QFrame::NoFrame);
  view->resize(this->size());
  view->setSceneRect(QRect(0, 0, this->width(), this->height()));

  loadScene(gameScene);
}

MainWindow::~MainWindow() { delete ui; }
