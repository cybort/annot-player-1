#ifndef STOPPABLE_H
#define STOPPABLE_H

// stoppable.h
// 2/14/2011

#include <QtCore/QRunnable>
#include <QtCore/QThread>

class Stoppable
{
public:
  virtual void stop() = 0;
};

class StoppableTask : public QRunnable, public Stoppable
{ };

class StoppableThread : public QThread, public Stoppable
{
  Q_OBJECT
  Q_DISABLE_COPY(StoppableThread)
  typedef QThread Base;
public:
  explicit StoppableThread(QObject *parent = 0) : Base(parent) { }
public slots:
  virtual void run() { Base::run(); } ///< \override
  virtual void stop() { } ///< \override
};

#endif // STOPPABLE_H
