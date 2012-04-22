// remotestream.cc
// 2/15/2012

#include "module/stream/remotestream.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QEventLoop>

// - Construction -

RemoteStream::RemoteStream(QObject *parent)
  : Base(parent), size_(0)
{ nam_ = new QNetworkAccessManager(this); }

// - Wait -

void
RemoteStream::waitForReady()
{
  enum { max_redirect = 5 };
  int retries = max_redirect;
  do waitForReadyRead();
  while (retries--  && tryRedirect());
}

void
RemoteStream::waitForReadyRead()
{
  QEventLoop loop;
  connect(this, SIGNAL(error(QString)), &loop, SLOT(quit()));
  connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
  connect(this, SIGNAL(readyRead()), &loop, SLOT(quit()));
  connect(this, SIGNAL(stopped()), &loop, SLOT(quit()));
  loop.exec();
}

// EOF
