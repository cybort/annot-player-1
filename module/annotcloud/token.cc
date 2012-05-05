// annotcloud/token.cc
// 8/7/2011

#include "module/annotcloud/token.h"
#include "module/crypt/crypt.h"
#ifdef WITH_MODULE_IOUTIL
#  include "module/ioutil/ioutil.h"
#endif // WITH_MODULE_IOUTIL
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <memory>

#define DEBUG "module/annotcloud::token"
#include "module/debug/debug.h"

#define DIGEST(_bytes)  Crypt::md5(_bytes)
enum { DIGEST_SIZE = 10 * 1024 * 1024 }; // 10 MB

using namespace AnnotCloud;

// - Meta type -

namespace { // anonymous
  struct init_ { init_() {
    qRegisterMetaType<Token>("Token");
    qRegisterMetaType<TokenList>("TokenList");
  } };
  init_ static_init_;
} // anonymous namespace

// - Digest -

QString
AnnotCloud::
Token::digestFromFile(const QString &input)
{
  DOUT("enter:" << input);

  QByteArray data;
  QString filePath = input;
  QFileInfo fi(input);
  if (fi.isDir()) {
    QString dir = fi.filePath();
    filePath = dir + "/VIDEO_TS/VIDEO_TS.IFO";
    if (!QFile::exists(filePath))
      filePath = dir + "/VIDEO_TS/VIDEO_TS.BUP";
    if (!QFile::exists(filePath))
      filePath = input;
  }

  DOUT("path =" << filePath);

  if (!filePath.isEmpty()) {
#ifdef WITH_MODULE_IOUTIL
    DOUT("with module ioutil");
    data = IOUtil::readBytes(filePath, DIGEST_SIZE);
#else
    DOUT("without module ioutil");
    QFile file(filePath);
    bool ok = file.open(QIODevice::ReadOnly);
    if (!ok) {
      DOUT("exit: Failed to open file for hashing: " << filePath);
      return QByteArray();
    }
    data = file.read(qMin(file.size(), qint64(DIGEST_SIZE)));
    file.close();
#endif // WITH_MODULE_IOUTIL
  }

  if (data.isEmpty()) {
    DOUT("exit: Error: get empty data to hash: " << filePath);
    return QByteArray();
  }

  QByteArray ret = DIGEST(data);
  DOUT("exit: ret =" << ret.toHex());
  return ret.toHex().toUpper();
}

// EOF
