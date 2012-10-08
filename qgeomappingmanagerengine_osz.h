/****************************************************************************
**
** Copyright (C) 2012 Daniel Ache
** All rights reserved.
** Contact: dastintenbrot@gmail.com
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOMAPPINGMANAGERENGINE_OSZ_H
#define QGEOMAPPINGMANAGERENGINE_OSZ_H

#include "qgeoserviceproviderplugin_osz.h"

#include <qgeoserviceprovider.h>
#include <qgeotiledmappingmanagerengine.h>

// If tile is recently updated - do not send any request to server until N days pass
//#define TILE_RELOAD_DAYS        7
// def tile cache size in bytes;  use 0 value for unlimited cache;
#define DEFAULT_TILE_CACHE_SIZE  30000000
#define DEFAULT_TILE_CACHE_DIR  "maptiles-cm"


//class QNetworkAccessManager;
//class QNetworkDiskCache;

QTM_USE_NAMESPACE

class QGeoMappingManagerEngineOsz : public QGeoTiledMappingManagerEngine
{
    Q_OBJECT
public:
    QGeoMappingManagerEngineOsz(const QMap<QString, QVariant> &parameters,
                                  QGeoServiceProvider::Error *error,
                                  QString *errorString);
    ~QGeoMappingManagerEngineOsz();

    QGeoTiledMapReply* getTileImage(const QGeoTiledMapRequest &request);
    //QNetworkAccessManager* getNetworkAccessManager() { return m_nam; };
    //QNetworkDiskCache* getNetworkDiskCache() { return m_cache; };
    const QString getCacheDir() { return m_cacheDir; };
    const QString getTileStyle() { return m_styleId; };

    QString getRequestString(const QGeoTiledMapRequest &request) const;
    // Clean tile cache directory to be <= then sizeLimit
    void cleanCacheToSize(int sizeLimit);

private:
    Q_DISABLE_COPY(QGeoMappingManagerEngineOsz)

    QMap<QString, QVariant> m_parameters;
    //QNetworkAccessManager *m_nam;
    //QNetworkDiskCache *m_cache;
    QString m_cacheDir;
    qint64  m_cacheSize;
    QString m_host;
    QString m_token;
    QString m_styleId;
};

#endif
