#ifndef DATAINPUTSTREAM_H
#define DATAINPUTSTREAM_H

// datainputstream.h
// 3/14/2012

#include "module/stream/inputstream.h"
#include <QtCore/QByteArray>
#include <QtCore/QObject>

class DataInputStream : public QObject, public InputStream
{
  Q_OBJECT
  Q_DISABLE_COPY(DataInputStream)
  typedef DataInputStream Self;
  typedef QObject Base;

  const QByteArray &data_;
  qint64 pos_;

public:
  explicit DataInputStream(const QByteArray &data, QObject *parent = 0)
    : Base(parent), data_(data), pos_(0) { }

  const QByteArray &data() const { return data_; }

public:
  virtual qint64 size() const { return data_.size(); } ///< \reimp
  virtual qint64 pos() const { return pos_; } ///< \reimp

  virtual bool reset()  { pos_ = 0; return true; } ///< \reimp

  virtual bool seek(qint64 pos) ///< \reimp
  { if (pos_ >= data_.size()) return false; pos_ = pos; return true; }

  virtual qint64 skip(qint64 count) ///< \reimp
  { qint64 diff = qMin(count, data_.size() - pos_); pos_ += diff; return diff; }

  virtual qint64 read(char *data, qint64 maxSize); ///< \reimp

  virtual QByteArray readAll()  ///< \reimp
  { return pos_ ? data_.mid(pos_) : data_; }

  virtual bool writeFile(const QString &path); ///< \reimp
};

#endif // DATAINPUTSTREAM_H
