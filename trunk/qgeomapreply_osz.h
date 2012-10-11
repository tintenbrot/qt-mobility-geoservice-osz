/****************************************************************************
**
** Copyright (C) 2012 tintenbrot
** All rights reserved.
** Many thanks to Cloudmade. Their qtmobility-plugin was a nice sample to
** understand the geoservice of qtmobility.
** Contact: dastintenbrot@gmail.com
**
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** dastintenbrot@gmail.com.
**
****************************************************************************/

#ifndef QGEOMAPREPLY_OSZ_H
#define QGEOMAPREPLY_OSZ_H

#include <qgeotiledmapreply.h>
#include <QNetworkReply>
//#include <QIODevice>
#include <QFile>
#include "qtimer.h"


QTM_USE_NAMESPACE

class QGeoMappingManagerEngineOsz;

class QGeoMapReplyOsz : public QGeoTiledMapReply
{
    Q_OBJECT

public:
    QGeoMapReplyOsz(QNetworkReply *reply, const QGeoTiledMapRequest &request, QObject *parent = 0);
    //QGeoMapReplyOsz(QIODevice *reply, const QGeoTiledMapRequest &request, QObject *parent = 0);
    ~QGeoMapReplyOsz();

    void abort();

    QNetworkReply* networkReply() const;
    //QIODevice* networkReply() const;

    // caching
    QString getTileKey(const QGeoTiledMapRequest &request) const;
    QString getTileFileName(const QString &tileKey) const;

    // If tile exist in cache - return pointer to it's file
    // otherwise return NULL
    QFile* isTileInCache(const QString &tileKey, QDateTime &lastModified);

    QString toHttpDate(const QDateTime &dt) const;
    //QDateTime fromHttpDate(const QString &value);

private slots:
    void replyDestroyed();
    void networkFinished();
    void networkError(QNetworkReply::NetworkError error);
    void timeout();
    void resendRequest();

private:
    bool cleanedUp;
    QNetworkReply *m_netReply;
    //QIODevice *m_netReply;
    QNetworkRequest *m_netRequest;
    const QGeoTiledMapRequest &m_tileRequest;
    QTimer *m_timeoutTimer;
    int m_resendCounter;
    QString m_tileKey;
    QString m_tileFileName;
    QString m_tileHttpLastModifiedStr;
    QString m_rawRequest;
    QNetworkAccessManager *m_namPtr;
    QGeoMappingManagerEngineOsz *m_mapManagerEngineOsz;
};

#endif
