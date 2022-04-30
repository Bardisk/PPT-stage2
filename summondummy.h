#ifndef SUMMONDUMMY_H_
#define SUMMONDUMMY_H_

#include <component.h>

class SummonDummy : public Component {
 public:
  SummonDummy();

  void onAttach() override;

  void onClick(QGraphicsSceneMouseEvent *ev) override;
};

#endif  // SUMMONDUMMY_H_
