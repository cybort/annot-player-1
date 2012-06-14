#ifndef BLOCKIODEVICE_H
#define BLOCKIODEVICE_H

// blockiodevice.h
// 12/3/2011

#include <QtCore/QIODevice>

///  Read/write/seek pos must be blocked.
class BlockIODevice : public QIODevice
{
  Q_OBJECT
  Q_DISABLE_COPY(BlockIODevice)
  typedef BlockIODevice Self;
  typedef QIODevice Base;

  // - Constructions -
public:
  explicit BlockIODevice(QObject *parent = 0)
    : Base(parent), fd_((fd_t)-1), blockSize_(0) { }

  explicit BlockIODevice(const QString &fileName, QObject *parent = 0)
    : Base(parent), fd_((fd_t)-1), blockSize_(0), fileName_(fileName) { }

  virtual ~BlockIODevice() { if (isOpen()) close(); } ///< \reimp

  // - Properties -
public:
  const QString &fileName() const { return fileName_; }
  void setBlockIODeviceName(const QString &fileName) { fileName_ = fileName; }

  ///< Return sector size if the file is a block device.
  int blockSize() const { return blockSize_; }

  bool isAudioCD() const;

  // - Overriden -
public:
  virtual bool open(OpenMode mode); ///< \reimp Only read mode is implemented.
  virtual void close(); ///< \reimp

  /*
  virtual bool atEnd() const; ///< \reimp
  virtual qint64 bytesAvailable() const; ///< \reimp
  virtual qint64 bytesToWrite() const; ///< \reimp
  virtual bool isSequential() const; ///< \reimp
  virtual qint64 pos() const; ///< \reimp
  virtual bool reset(); ///< \reimp
  virtual bool seek( qint64 pos ); ///< \reimp
  virtual qint64 size() const; ///< \reimp
  virtual bool waitForBytesWritten( int msecs ); ///< \reimp
  virtual bool waitForReadyRead( int msecs ); ///< \reimp
  */

  ///  \reimp Not implemented.
  virtual bool canReadLine() const
  { return false; }

protected:
  //virtual qint64 readLineData( char * data, qint64 maxSize); ///< \reimp
  virtual qint64 readData(char *data, qint64 maxSize ); ///< \reimp
  virtual qint64 writeData(const char *data, qint64 maxSize); ///< \reimp

  // - Implementations -
public:
#ifdef Q_OS_WIN
  typedef void *fd_t; // file handle
#else
  typedef int fd_t; // file descriptor
#endif // Q_OS_WIN
private:
  fd_t fd_;
  int blockSize_;
  QString fileName_;

};

#endif // BLOCKIODEVICE_H
