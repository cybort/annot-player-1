#ifndef NULLOUTPUTSTREAM_H
#define NULLOUTPUTSTREAM_H

// nulloutputstream.h
// 2/13/2012

#include "module/stream/outputstream.h"
#include <QtCore/QObject>

class NullOutputStream : public QObject, public OutputStream
{
  Q_OBJECT
  Q_DISABLE_COPY(NullOutputStream)
  typedef NullOutputStream Self;
  typedef QObject Base;

public:
  explicit NullOutputStream(QObject *parent = 0)
    : Base(parent) { }

  virtual qint64 write(const char *data, qint64 maxSize) ///< \override
  { Q_UNUSED(data); return maxSize; }
};

#endif // FILEOUTPUTSTREAM_H
