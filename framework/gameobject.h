#ifndef FRAMEWORK_GAMEOBJECT_H_
#define FRAMEWORK_GAMEOBJECT_H_

#include <QObject>

class Component;
class GameScene;
class QGraphicsSceneMouseEvent;

class GameObject final : public QObject {
  Q_OBJECT

 public:
  GameObject();
  ~GameObject();
  GameObject(const GameObject &) = delete;
  GameObject &operator=(const GameObject &) = delete;

  // Component management API
 public:
  template <typename T>
  T *getComponent() {
    for (auto component : components) {
      auto ptr = dynamic_cast<T *>(component);
      if (ptr != nullptr) return ptr;
    }
    return nullptr;
  }
  void addComponent(Component *component);
  void removeComponent(Component *component);

  // Lifecycle functions
 public:
  // GameObject 中所有 Component 已添加完成，在挂载到 GameScene 前调用
  void onAttach();
  // GameScene 渲染第一帧前调用
  void onFirstUpdate();
  // 每一次更新游戏状态时调用，参数为距上次更新的秒数
  void onUpdate(float deltaTime);
  // GameObject 从 GameScene 中移除时调用
  void onDetach();

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

  // Mouse input API
 public:
  // Transform 组件被点击时调用
  void onClick(QGraphicsSceneMouseEvent *ev);

 public:
  void setParentGameScene(GameScene *gameScene);

 private:
  GameScene *gameScene = nullptr;
  QList<Component *> components;
};

#endif  // FRAMEWORK_GAMEOBJECT_H_
