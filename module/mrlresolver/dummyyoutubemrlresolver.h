#ifndef DUMMYYOUTUBEMRLRESOLVER_H
#define DUMMYYOUTUBEMRLRESOLVER_H

// dummygooglevideomrlresolver.h
// 2/2/2012

#include "module/mrlresolver/mrlresolver.h"

class DummyYoutubeMrlResolver : public MrlResolver
{
  Q_OBJECT
  Q_DISABLE_COPY(DummyYoutubeMrlResolver)
  typedef DummyYoutubeMrlResolver Self;
  typedef MrlResolver Base;

public:
  explicit DummyYoutubeMrlResolver(QObject *parent = 0)
    : Base(parent) { }

public:
  bool matchMedia(const QString &href) const; ///< \reimp

public slots:
  bool resolveMedia(const QString &href); ///< \reimp
};

#endif // DUMMYYOUTUBEMRLRESOLVER_H
