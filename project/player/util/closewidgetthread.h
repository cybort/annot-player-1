#ifndef CLOSEWIDGETTHREAD_H
#define CLOSEWIDGETTHREAD_H

// CloseWidgetThread.h
// 10/31/2011

#include <QtGui/QWidget>
#include <QtCore/QThread>

class CloseWidgetThread : public QThread
{
  Q_OBJECT
  Q_DISABLE_COPY(CloseWidgetThread)
  typedef CloseWidgetThread Self;
  typedef QThread Base;

  QWidget *w_;

  virtual void run() { w_->close(); } // \override

public:
  explicit CloseWidgetThread(QWidget *w, QObject *parent = 0)
    : Base(parent), w_(w) { Q_ASSERT(w_); }
};

#endif // CLOSEWIDGETTHREAD_H
