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
#include "qgeomappingmanagerengine_offline.h"
#include <QApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QtConcurrentRun>
#include <QFutureWatcher>

QGeoMapReplyOsz::QGeoMapReplyOsz(QString sZipFile, QString sTileExt,const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
    m_mapManagerEngineOffline = static_cast<QGeoMappingManagerEngineOffline*>(parent);
    //m_zip=&thezip;
    m_zip.setZipName(sZipFile);
    m_zip.open(QuaZip::mdUnzip);
    m_tileExt=sTileExt;
    //
    m_tileKey = getTileKey(request);
    m_tileFileName = getTileFileName(m_tileKey);

//    QFuture<void> future = QtConcurrent::run(QGeoMapReplyOsz::getTileItself);
    QFuture<void> future = QtConcurrent::run(this, &QGeoMapReplyOsz::getTileItself);
    m_fwatcher.setFuture(future);
    connect(&m_fwatcher, SIGNAL(finished()), this, SLOT(getTileItselfFinished()));

 }

void QGeoMapReplyOsz::getTileItself()
{
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
        }
        else {
            // Alles OK. - File rauspicken
            QByteArray tileRaw=file.readAll();
            setMapImageData(tileRaw);
            setMapImageFormat(m_tileExt.toUpper());
            file.close();
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
    }

}

void QGeoMapReplyOsz::getTileItselfFinished()
{
    setFinished(true);

}

QGeoMapReplyOsz::~QGeoMapReplyOsz()
{
//    m_zip.close();
}

QString QGeoMapReplyOsz::getTileKey(const QGeoTiledMapRequest &request) const
{
    QString key = "";
    key += QString::number(request.zoomLevel());
    key += "/";
    key += QString::number(request.column());
    key += "/";
    key += QString::number(request.row());
    return key;
}


QString QGeoMapReplyOsz::getTileFileName(const QString &tileKey) const
{
    QString fname = "";
    fname = tileKey + "."+m_tileExt;
    return fname;
}

void QGeoMapReplyOsz::abort()
{
    //Needed function (it is public)
}
