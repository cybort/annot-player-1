#ifndef DOWNLOAD_H
#define DOWNLOAD_H

// download.h
// 2/4/2012

#include <QtCore/QString>

QT_FORWARD_DECLARE_CLASS(QObject)
QT_FORWARD_DECLARE_CLASS(QNetworkRequest)
QT_FORWARD_DECLARE_CLASS(QUrl)

bool dlget(const QString &path, const QNetworkRequest &req,
           bool async = true, int retries = 5, QObject *parent = 0);

bool dlget(const QString &path, const QUrl &url,
           const QString &header = QString(),
           bool async = true, int retries = 5, QObject *parent = 0);

bool dlpost(const QString &path, const QUrl &url, const QByteArray &data = QByteArray(),
            const QString &header = QString(),
            bool async = false,  int retries = 5, QObject *parent = 0);

#endif // DOWNLOAD_H
