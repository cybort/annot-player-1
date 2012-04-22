#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

// downloadmanager.h
// 2/20/2012

#include "downloadtask.h"
#include <QtCore/QObject>

class DownloadManager : public QObject
{
  Q_OBJECT
  typedef DownloadManager Self;
  typedef QObject Base;

  DownloadTaskList tasks_;

  int threadCount_;

public:
  explicit DownloadManager(QObject *parent = 0)
    : Base(parent), threadCount_(0) { }

  // - Properties -
public:
  const DownloadTaskList &tasks() const { return tasks_; }

  bool isEmpty() const { return tasks_.isEmpty(); }

  void addTask(DownloadTask *t);

  DownloadTask *taskWithId(int tid);
  DownloadTask *taskWithUrl(const QString &url);
  void removeTask(DownloadTask *t); // also delete task if owned

  int maxThreadCount() const { return threadCount_; }

public slots:
  void stopAll();
  void removeAll();
  void refreshSchedule();
  void setMaxThreadCount(int n) { threadCount_ = n; }

protected:
  QString normalizeUrl(const QString &url);
};

#endif // DOWNLOADMANAGER_H
