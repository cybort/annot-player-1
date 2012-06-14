#ifndef WBTABWIDGET_H
#define WBTABWIDGET_H

// gui/wbtabwidget.h
// 4/21/2012

#include "gui/wbtabbar.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QTabWidget>

class WbTabWidget : public QTabWidget
{
  Q_OBJECT
  Q_DISABLE_COPY(WbTabWidget)
  typedef WbTabWidget Self;
  typedef QTabWidget Base;

public:
  explicit WbTabWidget(QWidget *parent = 0)
    : Base(parent)
  {
    WbTabBar *tb = new WbTabBar;
    connect(tb, SIGNAL(doubleClicked(int)), SIGNAL(tabDoubleClicked(int)));
    setTabBar(tb);
  }

  QTabBar *tabBar() const { return Base::tabBar(); }

signals:
  void doubleClicked();
  void rightButtonClicked();
  void tabDoubleClicked(int index);

  // - Events -
protected:
  virtual void mouseDoubleClickEvent(QMouseEvent *e) ///< \reimp
  {
    if (e->button() == Qt::LeftButton && !e->modifiers()) {
      emit doubleClicked();
      e->accept();
    } else
      Base::mouseDoubleClickEvent(e);
  }

  virtual void mouseReleaseEvent(QMouseEvent *e) ///< \reimp
  {
    if (e->button() == Qt::RightButton)
      emit rightButtonClicked();
    Base::mouseReleaseEvent(e);
  }
};

#endif // WBTABWIDGET_H
