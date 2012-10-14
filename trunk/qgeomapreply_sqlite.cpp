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

QGeoMapReplySqlite::QGeoMapReplySqlite(QuaZip &m_zip, QString m_tileext,const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
    m_mapManagerEngineOffline = static_cast<QGeoMappingManagerEngineOffline*>(parent);

    m_tileExt=m_tileext;
    //
    m_tileKey = getTileKey(request);
    m_tileFileName = getTileFileName(m_tileKey);
    QString sFileName = m_tileFileName;


    m_zip.setCurrentFile(sFileName);
    bool boolFileExist=m_zip.hasCurrentFile();
    qDebug() << "Check in ZIP: " << sFileName << "=" << boolFileExist;
    if (boolFileExist) {
        QuaZipFile file(&m_zip);
        if(!file.open(QIODevice::ReadOnly)) {
            qDebug() << "ZIP Error File can not be opened";
            // Hier auch nen Fehler Zip hin
            QFile fileError(":tile_notavailable");
            fileError.open(QIODevice::ReadOnly);
            qDebug() << "Error Not in ZIP";
            setMapImageData(fileError.readAll());
            setMapImageFormat("PNG");
            fileError.close();
            setFinished(true);
        }
        else {
            // Alles OK. - File rauspicken
            QByteArray tileRaw=file.readAll();
            setMapImageData(tileRaw);
            setMapImageFormat(m_tileext.toUpper());
            file.close();
            setFinished(true);
        }
    }
    else {
        //Hier muss ein Tile hin (nicht im ZIP File enthalten)
        QFile fileError(":tile_notavailable");
        fileError.open(QIODevice::ReadOnly);
        qDebug() << "Error Not in ZIP";
        setMapImageData(fileError.readAll());
        setMapImageFormat("PNG");
        fileError.close();
        setFinished(true);
    }
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
