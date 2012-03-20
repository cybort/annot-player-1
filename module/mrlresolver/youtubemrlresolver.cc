// youtubemrlresolver.cc
// 1/25/2012

#include "youtubemrlresolver.h"
#include <QtCore>
#include <QtScript>
#include <QtNetwork>
#include <cstdlib>

//#define DEBUG "youtubemrlresolver"
#include "module/debug/debug.h"

// See: jd/plugins/hoster/YouTubeCom.java
// See: jd/plugins/decrypter/TbCm.java

// - Analysis -

bool
YoutubeMrlResolver::matchMedia(const QString &href) const
{
  QRegExp rx("^http://(|www\\.)youtube\\.com/watch?", Qt::CaseInsensitive);
  return href.contains(rx);
}

// Example: http://www.youtube.com/watch?v=-DJqnomZoLk&list=FLc18abM35KhjkqsLzTmOEjA&feature=plcp
bool
YoutubeMrlResolver::resolveMedia(const QString &href)
{
  static const QString errorMessage = tr("failed to resolve URL");
  QUrl url(href);
  if (!url.host().endsWith("youtube.com", Qt::CaseInsensitive) ||
      url.path().compare("/watch", Qt::CaseInsensitive)) {
    emit error(errorMessage + ": " + href);
    //return;
  }

  QString v = url.queryItemValue("v");
  if (v.isEmpty()) {
    emit error(errorMessage + ": " + href);
    return false;
  }

  QString mrl = "http://www.youtube.com/watch?v=" + v;
  emit message(tr("resolving media URL ...") + ": " + mrl);
  MediaInfo mi;
  mi.mrls.append(MrlInfo(mrl));
  mi.refurl = mrl;
  emit mediaResolved(mi, 0);
  return true;
}

// EOF