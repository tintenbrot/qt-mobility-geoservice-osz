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

#include "qgeomapreply_sqlite.h"
#include "debug_osz.h"
#include "qgeomappingmanagerengine_offline.h"
#include <QApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QFile>

QGeoMapReplySqlite::QGeoMapReplySqlite(QString sSqliteFile, QString sTileExt,const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
//    m_mapManagerEngineOffline = static_cast<QGeoMappingManagerEngineOffline*>(parent);

    QFile fileError(":tile_working");
    fileError.open(QIODevice::ReadOnly);
    setMapImageData(fileError.readAll());
    setMapImageFormat("PNG");
    fileError.close();
    setFinished(true);
}


QGeoMapReplySqlite::~QGeoMapReplySqlite()
{
}

QString QGeoMapReplySqlite::getTileKey(const QGeoTiledMapRequest &request) const
{
    QString key = "";
    key += QString::number(request.zoomLevel());
    key += "/";
    key += QString::number(request.column());
    key += "/";
    key += QString::number(request.row());
    return key;
}


QString QGeoMapReplySqlite::getTileFileName(const QString &tileKey) const
{
    QString fname = "";
    fname = tileKey + "."+m_tileExt;
    return fname;
}

void QGeoMapReplySqlite::abort()
{
    //Needed function (it is public)
}
