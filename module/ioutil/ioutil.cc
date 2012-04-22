// ioutil.cc
// 12/6/2011

#include "ioutil.h"
#include <QtCore/QFile>

#ifdef WITH_MODULE_BLOCKIODEVICE
#  include "module/blockiodevice/blockiodevice.h"
#  ifdef Q_OS_WIN
#    ifdef WITH_WIN_QTWIN
#      include "win/qtwin/qtwin.h"
#    else
#      error "qtwin is required"
#    endif // WITH_WIN_QTWIN
#  endif // Q_OS_WIN
#  ifdef Q_OS_UNIX
#    ifdef WITH_UNIX_QTUNIX
#      include "unix/qtunix/qtunix.h"
#    else
#      error "qtunix is required"
#    endif // WITH_UNIX_QTUNIX
#  endif // Q_OS_UNIX
#endif // WITH_MODULE_BLOCKIODEVICE

#define DEBUG "ioutil"
#include "module/debug/debug.h"

bool
IOUtil::isAudioCD(const QString &input)
{
  QString filePath = input;
  bool ret = false;
#ifdef WITH_MODULE_BLOCKIODEVICE
  if (!filePath.isEmpty()) {
#ifdef Q_OS_WIN
    QString deviceFileName = QtWin::guessDeviceFileName(filePath);
    if (QtWin::isValidDeviceFileName(deviceFileName)) {
      filePath = deviceFileName;
#elif defined (Q_OS_UNIX)
    if (QtUnix::isDeviceFile(filePath)) {
#else
    if (0) {
#endif // Q_OS_
      BlockIODevice file(filePath);
      bool ok = file.open(QIODevice::ReadOnly);
      if (ok) {
        ret = file.isAudioCD();
        file.close();
      }
    }
  }
#endif // WITH_MODULE_BLOCKIODEVICE
  return ret;
}

QByteArray
IOUtil::readBytes(const QString &input, qint64 maxSize)
{
  DOUT("enter: input =" << input << ", maxSize =" << maxSize);
  QString filePath = input;
  QByteArray data;
  if (!filePath.isEmpty()) {
#ifdef WITH_MODULE_BLOCKIODEVICE
#ifdef Q_OS_WIN
    QString deviceFileName = QtWin::guessDeviceFileName(filePath);
    if (QtWin::isValidDeviceFileName(deviceFileName)) {
      filePath = deviceFileName;
#elif defined (Q_OS_UNIX)
    if (QtUnix::isDeviceFile(filePath)) {
#else
    if (0) {
#endif // Q_OS_
      BlockIODevice file(filePath);
      bool ok = file.open(QIODevice::ReadOnly);
      if (!ok) {
        DOUT("exit: Failed to open file for hashing: " << filePath);
        return QByteArray();
      }

      int readSize = maxSize;
      int blockSize = file.blockSize();
      if (blockSize > 0 && maxSize % blockSize) {
        readSize = ((maxSize / blockSize) + 1) * blockSize;
        DOUT("blockSize =" << blockSize <<
             ", maxSize =" << maxSize <<
             ", readSize =" << readSize);
      }

      data = file.read(readSize);
      file.close();
    } else
#endif // WITH_MODULE_BLOCKIODEVICE
    {
      QFile file(filePath);
      bool ok = file.open(QIODevice::ReadOnly);
      if (!ok) {
        DOUT("exit: Failed to open file for hashing: " << filePath);
        return QByteArray();
      }

      data = file.read(maxSize);
      file.close();
    }
  }

  if (data.isEmpty()) {
    DOUT("exit: error: get empty data to hash: " << filePath);
    return QByteArray();
  }

  if (data.size() > maxSize) {
     DOUT("truncate from" << data.size() << " to" << maxSize);
     data.truncate(maxSize);
  }
  return data;
}
// EOF
