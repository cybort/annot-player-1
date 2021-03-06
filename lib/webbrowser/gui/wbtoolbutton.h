#ifndef WBTOOLBUTTON_H
#define WBTOOLBUTTON_H

// gui/wbtoolbutton.h
// 4/25/2012

#include "qtx/qxtoolbutton.h"

typedef QxToolButton WbToolButtonBase;
class WbToolButton : public WbToolButtonBase
{
  Q_OBJECT
  Q_DISABLE_COPY(WbToolButton)
  typedef WbToolButton Self;
  typedef WbToolButtonBase Base;

public:
  explicit WbToolButton(QWidget *parent = nullptr)
    : Base(parent) { }

};

#endif // WBTOOLBUTTON_H
