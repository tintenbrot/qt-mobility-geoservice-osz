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

QGeoMapReplyOsz::QGeoMapReplyOsz(QuaZip &m_zip, const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
    m_mapManagerEngineOsz = static_cast<QGeoMappingManagerEngineOsz*>(parent);

    m_tileKey = getTileKey(request);
    //m_tileKeyPath = getTileKeyPath(request);
    m_tileFileName = getTileFileName(m_tileKey);
    //m_tileHttpLastModifiedStr = "";
    //m_rawRequest = "";

    // Search this tile in cache:
    //zip = new QuaZip(OSZ_FILE);

//    QFile *file = isTileInCache(m_tileKey);
//    setCached(file?true:false);
//    if (file)
//    {
//        QDataStream in(file);
//        DBG_OSZ(TILES_M, "Tile found in local cache: " << m_tileKey);

//        setMapImageData(file->readAll());
//        setMapImageFormat("PNG");
//        file->close();
//        delete file;
//        file = NULL;

//        setFinished(true);
//        return;
//    }
//    else
    {

//        if(!m_zip.open(QuaZip::mdUnzip)) {
//            qDebug() << "Error opening OSZ-File";
//        }
//        else
         {
//            QDateTime MyDateTime;
//            //qDebug() << "OSZ-File succesfully opened";
//            QString sFileName = m_tileKey + ".png";
            QString sFileName = m_tileFileName;

//            m_zipdir.cd(m_tileKeyPath);
//            qDebug() << "zipPath=" << m_zipdir.path();

//            qDebug() << "zipdir exist=" << m_zipdir.exists(sFileName);
//            sFileName=m_tileKeyPath+sFileName;
            m_zip.setCurrentFile(sFileName);
//            //qDebug() << QDateTime::time();
//            qDebug() << "1 MyDateTime=" << MyDateTime.time().currentTime().msec();
            bool boolFileExist=true;//m_zip.hasCurrentFile();
//            qDebug() << "2 MyDateTime=" << MyDateTime.time().currentTime().msec();
            qDebug() << "Check in ZIP: " << sFileName << "=" << boolFileExist;
            if (boolFileExist) {
                QuaZipFile file(&m_zip);
                if(!file.open(QIODevice::ReadOnly)) {
                    qDebug() << "ZIP Error File can not be opened";
                    // Hier auch nen Fehler Zip hin
                    QFile fileError(":tile_ziperror");
                    fileError.open(QIODevice::ReadOnly);
                    qDebug() << "Error Not in ZIP";
                    setMapImageData(fileError.readAll());
                    setMapImageFormat("PNG");
                    fileError.close();
                    setFinished(true);
                }
                else {
                    // Alles OK. - File rauspicken
                    //file.

                    QByteArray tileRaw=file.readAll();
//qDebug() << "3 MyDateTime=" << MyDateTime.time().currentTime().msec();
                    qDebug() << "tileRawSize=" << tileRaw.size();
                    setMapImageData(tileRaw);
                    setMapImageFormat("PNG");
                    // und im Cache-Dir speichern
                    //QDir().mkpath(QFileInfo(m_tileFileName).dir().absolutePath());
                    //
//                    QFile fileCache(m_tileFileName);
//                    fileCache.open(QIODevice::WriteOnly);
//                    fileCache.write(tileRaw);
//                    fileCache.close();
                    //
                    file.close();
                    setFinished(true);
                }
            }
            else {
                //Hier muss ein Tile hin (nicht im ZIP File enthalten)
                //QByteArray tileRaw=file.readAll();
                QFile fileError(":tile_notavailable");
                fileError.open(QIODevice::ReadOnly);
                qDebug() << "Error Not in ZIP";
                setMapImageData(fileError.readAll());
                setMapImageFormat("PNG");
                fileError.close();
                setFinished(true);
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
    //qDebug() << "getTileKey=" << key;
    return key;
}

//QString QGeoMapReplyOsz::getTileKeyPath(const QGeoTiledMapRequest &request) const
//{
//    QString key = "";
//    //key += "/";
//    key += QString::number(request.zoomLevel());
//    key += "/";
//    key += QString::number(request.column());
//    key += "/";
//    //key += QString::number(request.row());
//    //qDebug() << "getTileKeyPath=" << key;
//    return key;
//}


QString QGeoMapReplyOsz::getTileFileName(const QString &tileKey) const
{
//    QString fname = "tilecache";
    QString fname = "";
//    if (m_mapManagerEngineOsz)
//    {
//        fname = m_mapManagerEngineOsz->getCacheDir();
//    }
//    else
//    {
//        DBG_OSZ(TILES_M, "getTileFileName(): m_mapManagerEngineCm is NULL ");
//    }

    //fname += QDir::separator();
    fname = tileKey + ".png";
    return fname;
}


//QFile* QGeoMapReplyOsz::isTileInCache(const QString &tileKey)
//{
//    QFile *file = new QFile(getTileFileName(tileKey));
//    if (!file->open(QIODevice::ReadOnly))
//    {
//            DBG_OSZ(TILES_M, "Tile is not in cache: " << getTileFileName(tileKey) );
//            delete file;
//            file = NULL;
//    }
//    return file;
//}

void QGeoMapReplyOsz::abort()
{
    //Needed function (it is public)
}
