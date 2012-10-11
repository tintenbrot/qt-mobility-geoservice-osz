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

#ifndef QGEOMAPPINGMANAGERENGINE_OSZ_H
#define QGEOMAPPINGMANAGERENGINE_OSZ_H

#include "qgeoserviceproviderplugin_osz.h"

#include <qgeoserviceprovider.h>
#include <qgeotiledmappingmanagerengine.h>

// If tile is recently updated - do not send any request to server until N days pass
// def tile cache size in bytes;  use 0 value for unlimited cache;
#define DEFAULT_TILE_CACHE_SIZE  30000000
#define DEFAULT_TILE_CACHE_DIR  "maptiles-osz"

#if defined(__ARMEL__) //Harmattan
#define TILES_DIR "/home/user/MyDocs/tile-maps"
#else
#define TILES_DIR "/home/daniel/tile-maps"
#endif

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
    const QString getCacheDir() { return m_cacheDir; };
    //const QString getTileStyle() { return m_styleId; };

    const QString getTilesDir() { return m_tilesDir; }; //DA

    QString getRequestString(const QGeoTiledMapRequest &request) const;
    // Clean tile cache directory to be <= then sizeLimit
    void cleanCacheToSize(int sizeLimit);

private:
    Q_DISABLE_COPY(QGeoMappingManagerEngineOsz)

    QMap<QString, QVariant> m_parameters;
    QString m_cacheDir;
    qint64  m_cacheSize;
    //QString m_styleId;
    QString m_tilesDir; //DA
};

#endif
