#ifndef FRAMEWORK_GAMESCENE_H_
#define FRAMEWORK_GAMESCENE_H_

#include <QGraphicsScene>
#include <QList>
#include <QMap>

class GameObject;

class GameScene final : public QGraphicsScene {
  Q_OBJECT

 public:
  GameScene();
  ~GameScene();
  GameScene(const GameScene &) = delete;
  GameScene &operator=(const GameScene &) = delete;

  // Game object management API
 public:
  void attachGameObject(GameObject *gameObject);
  void detachGameObject(GameObject *gameObject);
  GameObject *getGameObject(const char *name);

 private:
  QTimer *updateTimer = nullptr;
  QList<GameObject *> gameObjects;
  QList<GameObject *> gameObjectsToAttach;
  QList<GameObject *> gameObjectsToDetach;
  QList<GameObject *> gameObjectsAttachedOnLastUpdate;
  void onUpdate();

  // Keyboard input API
 public:
  bool getKey(Qt::Key key) { return keyTable[key]; }
  bool getKeyDown(Qt::Key key) { return keyDownTable[key]; }
  bool getKeyUp(Qt::Key key) { return keyUpTable[key]; }

  // Require clearing keyDownArray and keyUpArray after update()
 private:
  QMap<int, bool> keyTable;
  QMap<int, bool> keyDownTable, keyUpTable;
  void keyPressEvent(QKeyEvent *ev) override;
  void keyReleaseEvent(QKeyEvent *ev) override;
  void focusOutEvent(QFocusEvent *ev) override;
};

#endif  // FRAMEWORK_GAMESCENE_H_
