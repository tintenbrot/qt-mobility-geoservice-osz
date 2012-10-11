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

#include "qgeomapreply_osz.h"
#include "debug_osz.h"
#include "qgeomappingmanagerengine_osz.h"
#include <QApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include "quazip.h"
#include "quazipfile.h"

QGeoMapReplyOsz::QGeoMapReplyOsz(const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
    m_mapManagerEngineOsz = static_cast<QGeoMappingManagerEngineOsz*>(parent);

    m_tileKey = getTileKey(request);
    m_tileFileName = getTileFileName(m_tileKey);
    m_tileHttpLastModifiedStr = "";
    m_rawRequest = "";

    // Search this tile in cache:

    QFile *file = isTileInCache(m_tileKey);
    setCached(file?true:false);
    if (file)
    {
        QDataStream in(file);
        DBG_OSZ(TILES_M, "Tile found in local cache: " << m_tileKey);

        setMapImageData(file->readAll());
        setMapImageFormat("PNG");
        file->close();
        delete file;
        file = NULL;

        setFinished(true);
        return;
    }
    else
    {
        QuaZip zip(OSZ_FILE);
        if(!zip.open(QuaZip::mdUnzip)) {
            qDebug() << "Error opening OSZ-File";
        }
        else {
            qDebug() << "OSZ-File succesfully opened";
            QString sFileName = m_tileKey + ".png";
            zip.setCurrentFile(sFileName);
            qDebug() << "Check in ZIP: " << sFileName << "=" << zip.hasCurrentFile();
            if (zip.hasCurrentFile()) {
                QuaZipFile file(&zip);
                if(!file.open(QIODevice::ReadOnly)) {
                    qDebug() << "ZIP Error File can not be opened";
                    // Hier auch nen Fehler Zip hin
                }
                else {
                    // Alles OK. - File rauspicken
                    //file.

                    QByteArray tileRaw=file.readAll();
                    qDebug() << "tileRawSizue=" << tileRaw.size();
                    setMapImageData(tileRaw);
                    setMapImageFormat("PNG");
                    // und im Cache-Dir speichern
                    QDir().mkpath(QFileInfo(m_tileFileName).dir().absolutePath());
                    //
                    QFile fileCache(m_tileFileName);
                    fileCache.open(QIODevice::WriteOnly);
                    fileCache.write(tileRaw);
                    fileCache.close();
                    //
                    file.close();
                    setFinished(true);
                }
            }
            else {
                //Hier muss ein Tile hin (nicht im ZIP File enthalten)
            }
        }
    }
}


QGeoMapReplyOsz::~QGeoMapReplyOsz()
{
}

QString QGeoMapReplyOsz::getTileKey(const QGeoTiledMapRequest &request) const
{
    QString key = "";
    key += QString::number(request.zoomLevel());
    key += "/";
    key += QString::number(request.column());
    key += "/";
    key += QString::number(request.row());
    qDebug() << "getTileKey " << key;
    return key;
}


QString QGeoMapReplyOsz::getTileFileName(const QString &tileKey) const
{
    QString fname = "tilecache";
    if (m_mapManagerEngineOsz)
    {
        fname = m_mapManagerEngineOsz->getCacheDir();
    }
    else
    {
        DBG_OSZ(TILES_M, "getTileFileName(): m_mapManagerEngineCm is NULL ");
    }

    fname += QDir::separator();
    fname += tileKey + ".png";
    return fname;
}


QFile* QGeoMapReplyOsz::isTileInCache(const QString &tileKey)
{
    QFile *file = new QFile(getTileFileName(tileKey));
    if (!file->open(QIODevice::ReadOnly))
    {
            DBG_OSZ(TILES_M, "Tile is not in cache: " << getTileFileName(tileKey) );
            delete file;
            file = NULL;
    }
    return file;
}

void QGeoMapReplyOsz::abort()
{
    //Needed function (it is public)
}
