#ifndef FRAMEWORK_COMPONENT_H_
#define FRAMEWORK_COMPONENT_H_

#include <QObject>

class GameObject;
class QGraphicsSceneMouseEvent;

class Component : public QObject {
  Q_OBJECT

 public:
  Component();
  Component(const Component &) = delete;
  Component &operator=(const Component &) = delete;

  // Lifecycle functions
 public:
  // GameObject 中所有 Component 已添加完成，在挂载到 GameScene 前调用
  virtual void onAttach() {}
  // GameScene 渲染第一帧前调用
  virtual void onFirstUpdate() {}
  // 每一次更新游戏状态时调用，参数为距上次更新的秒数
  virtual void onUpdate(float deltaTime) {}
  // GameObject 从 GameScene 中移除时调用
  virtual void onDetach() {}

  // Game object management API
 public:
  // 将 gameObject 挂载到 GameScene 上
  void attachGameObject(GameObject *gameObject);
  // 将 gameObject 从 GameScene 中移除
  void detachGameObject(GameObject *gameObject);
  // 将 gameObject 从 GameScene 中移除并销毁（delete）
  void destory(GameObject *gameObject);

  // Keyboard input API
 public:
  // 判断某个按键是否被按下
  bool getKey(Qt::Key key);
  // 判断某个按键是否在当前 update 帧中被按下
  bool getKeyDown(Qt::Key key);
  // 判断某个按键是否在当前 update 帧中被释放
  bool getKeyUp(Qt::Key key);

  // Mouse API
 public:
  // 所属 gameObject 的 Transform 被点击时调用
  virtual void onClick(QGraphicsSceneMouseEvent *ev) {}

  // Miscellaneous
 public:
  GameObject *getParentGameObject();
  void setParentGameObject(GameObject *gameObject);

 protected:
  GameObject *gameObject = nullptr;
};

#endif  // FRAMEWORK_COMPONENT_H_
