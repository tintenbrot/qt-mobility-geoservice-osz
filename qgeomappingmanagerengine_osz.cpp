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

#include "qgeomappingmanagerengine_osz.h"
//#include "qgeomapreply_cm.h"
//#include "debug_cm.h"

#include <qgeotiledmaprequest.h>

//#include <QNetworkAccessManager>
//#include <QNetworkDiskCache>
#include <QDesktopServices>
//#include <QNetworkProxy>
//#include <QSize>
#include <QDir>
#include <QDateTime>
#include <QApplication>
#include <QFileInfo>

//        : QGeoTiledMappingManagerEngine(parameters),
//        m_parameters(parameters)
    //m_host("b.tile.cloudmade.com"),
    //m_token(QGeoServiceProviderFactoryCm::defaultToken)
QGeoMappingManagerEngineOsz::QGeoMappingManagerEngineOsz(const QMap<QString, QVariant> &parameters, QGeoServiceProvider::Error *error, QString *errorString)
    : QGeoTiledMappingManagerEngine(parameters),
    m_parameters(parameters),
    m_host("b.tile.cloudmade.com"),
    m_token(QGeoServiceProviderFactoryOsz::defaultToken)
{
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    setTileSize(QSize(256,256));
    setMinimumZoomLevel(12.0);
    setMaximumZoomLevel(16.0);

    m_styleId = m_parameters.value("style", "1").toString();

    //SL_MAP_TYPE
    QList<QGraphicsGeoMap::MapType> types;
    types << QGraphicsGeoMap::StreetMap;    
    setSupportedMapTypes(types);

    //m_nam = new QNetworkAccessManager(this);
    //m_cache = new QNetworkDiskCache(this);
    m_cacheSize = DEFAULT_TILE_CACHE_SIZE;

    QList<QString> keys = m_parameters.keys();

//    if (keys.contains("mapping.proxy")) {
//        QString proxy = m_parameters.value("mapping.proxy").toString();
//        if (!proxy.isEmpty())
//            m_nam->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, proxy, 8080));
//    }

//    if (keys.contains("mapping.host")) {
//        QString host = m_parameters.value("mapping.host").toString();
//        if (!host.isEmpty())
//            m_host = host;
//    }

    if (keys.contains("mapping.cache.directory")) {
        QString cacheDir = m_parameters.value("mapping.cache.directory").toString();
        if (!cacheDir.isEmpty())
            m_cacheDir = cacheDir;
            //m_cache->setCacheDirectory(cacheDir);
    }
    else
    {
        // set default cache dir
        //        QDir dir = QDir::temp();
        QDir dir = QDir(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
//	qDebug() << __FUNCTION__ << "Cache at" << dir;
	
        dir.mkdir(DEFAULT_TILE_CACHE_DIR);
	//	QFileInfo info(dir.absolutePath());
	//	qDebug() << __FUNCTION__ << "Cache size" << info.size();

        dir.cd(DEFAULT_TILE_CACHE_DIR);
        //m_cache->setCacheDirectory(dir.path());
        m_cacheDir = dir.path();
    }
//    DBG_CM(TILES_M, INFO_L, "Setting tile cache dir to " << m_cacheDir);

    if (keys.contains("mapping.cache.size")) {
        bool ok = false;
        qint64 cacheSize = m_parameters.value("mapping.cache.size").toString().toLongLong(&ok);
        if (ok) {
            //m_cache->setMaximumCacheSize(cacheSize);
            m_cacheSize = cacheSize;
//            DBG_CM(TILES_M, INFO_L, "Setting tile cache size = " << m_cacheSize);
        }
    }

    // first of all: delete all *.png files that may still be lurking in
    // the old cache dir as they may affect the gallery (they are named 
    // png, but are not real png files)
    QDir dir = QDir::temp();
    if(dir.cd("maptiles-cm")) {
      QStringList pngFilters;
      pngFilters << "*.png";
      dir.setNameFilters(pngFilters);
      QStringList pngList = dir.entryList();
      foreach(QString name, pngList)
	dir.remove(name);
      
      dir.cd("..");
      dir.rmdir("maptiles-cm");
    }

    //    if (m_cacheSize > 0) cleanCacheToSize(m_cacheSize);
}

QGeoMappingManagerEngineOsz::~QGeoMappingManagerEngineOsz()
{
    // 0 - means unlimited cache, no cleaning!
  //    if (m_cacheSize > 0) cleanCacheToSize(m_cacheSize);
}

QGeoTiledMapReply* QGeoMappingManagerEngineOsz::getTileImage(const QGeoTiledMapRequest &request)
{
    QGeoTiledMapReply* mapReply;// = new QGeoMapReplyOsz(/*netReply*/ 0, request, this);
    return mapReply;
}

QString QGeoMappingManagerEngineOsz::getRequestString(const QGeoTiledMapRequest &request) const
{
    QString requestString = "/home/daniel/test.png";
//    QString requestString = "http://";
//    QString tileDimension = "256";

//    requestString += m_host;
//    if (!m_token.isNull())
//	requestString += '/' + m_token;
//    requestString += '/';
//    requestString += m_styleId;
//    requestString += '/';
//    requestString += tileDimension;
//    requestString += '/';
//    requestString += QString::number(request.zoomLevel());
//    requestString += '/';
//    requestString += QString::number(request.column());
//    requestString += '/';
//    requestString += QString::number(request.row());
//    requestString += '.';
//    requestString += "png";

    return requestString;
}

