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
//#include <QNetworkDiskCache>
#include <QDateTime>
#include <QDir>
#include <QFile>

#define HTTP_NOT_MODIFIED 304
#define HTTP_OK           200

QGeoMapReplyOsz::QGeoMapReplyOsz(const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
    //m_netRequest = 0;       // network request
    //m_netReply = 0;         // network reply
    //m_resendCounter = 3;    // Number of retries left to send network request/reply for tile image

//    m_namPtr = 0;
    m_mapManagerEngineOsz = static_cast<QGeoMappingManagerEngineOsz*>(parent);
    if (m_mapManagerEngineOsz) {
        //m_namPtr = m_mapManagerEngineOsz->getNetworkAccessManager();
    }

    m_tileKey = getTileKey(request);
    m_tileFileName = getTileFileName(m_tileKey);
    m_tileHttpLastModifiedStr = "";
    m_rawRequest = "";

    // Search this tile in cache:
    //QDateTime lastModified; // last-modified field, returned by server when tile was loaded
    QDateTime downloadDate; // date of latest download, or reply from server that tile not modified

    QFile *file = isTileInCache(m_tileKey, downloadDate);
    //qDebug() << "Filename=" << file->fileName();
    setCached(file?true:false);
    if (file)
    {
        QDataStream in(file);
	//        in >> lastModified;
	//        m_tileHttpLastModifiedStr = toHttpDate(lastModified);
	//        DBG_CM(TILES_M, INFO_L, "Tile found in local cache: " << m_tileKey << "lastModified: " << toHttpDate(lastModified));
        DBG_OSZ(TILES_M, "Tile found in local cache: " << m_tileKey << "downloadDate: " << toHttpDate(downloadDate));

        setMapImageData(file->readAll());
        setMapImageFormat("PNG");
        //emit updateMapDisplay(); // Note: display can be updated to show tile from cache even before reply from server
        file->close();
        delete file;
        file = NULL;

        // If tile is recently updated - do not send any request to server until N days pass
        QDateTime expireDate = downloadDate.addDays(TILE_RELOAD_DAYS);
        QDateTime now = QDateTime::currentDateTime();
        if ((now < expireDate) && (downloadDate < now )) {
            setFinished(true);
            DBG_OSZ(TILES_M, "Tile was very recently requested from server - not sending any request now (load from cache)");
            return;
        }
    }
    else
    {
        DBG_OSZ(TILES_M, "Tile not found in cache, loading from server.. " << m_tileKey );
        QDir().mkpath(QFileInfo(m_tileFileName).dir().absolutePath());
        QString sFileName=TILES_DIR;
        sFileName+=QDir::separator();
        sFileName+= m_tileKey + ".png";
        qDebug() << "NewFileName=" << sFileName << " to " << m_tileFileName;
        qDebug() << QFile::copy(sFileName,m_tileFileName);
        //QFile *file = new QFile(sFileName);
        //file->copy()
        setCached(true);
        QFile file(m_tileFileName);
        //file.open();
        setMapImageData(file.readAll());
        setMapImageFormat("PNG");
        file.close();

        setFinished(true);

    }
    //setCached(file?true:false);
    resendRequest();
}

QString QGeoMapReplyOsz::toHttpDate(const QDateTime &dt) const
{
    return QLocale::c().toString(dt, QLatin1String("ddd, dd MMM yyyy hh:mm:ss 'GMT'"))
        .toLatin1();
}

/*
QDateTime QGeoMapReplyCm::fromHttpDate(const QString &value)
{
    // HTTP dates have three possible formats:
    //  RFC 1123/822      -   ddd, dd MMM yyyy hh:mm:ss "GMT"
    //  RFC 850           -   dddd, dd-MMM-yy hh:mm:ss "GMT"
    //  ANSI C's asctime  -   ddd MMM d hh:mm:ss yyyy
    // We only handle them exactly. If they deviate, we bail out.

    int pos = value.indexOf(',');
    QDateTime dt;
#ifndef QT_NO_DATESTRING
    if (pos == -1) {
        // no comma -> asctime(3) format
        dt = QDateTime::fromString(QString::fromLatin1(value), Qt::TextDate);
    } else {
        // eat the weekday, the comma and the space following it
        QString sansWeekday = QString::fromLatin1(value.constData() + pos + 2);

        QLocale c = QLocale::c();
        if (pos == 3)
            // must be RFC 1123 date
            dt = c.toDateTime(sansWeekday, QLatin1String("dd MMM yyyy hh:mm:ss 'GMT"));
        else
            // must be RFC 850 date
            dt = c.toDateTime(sansWeekday, QLatin1String("dd-MMM-yy hh:mm:ss 'GMT'"));
    }
#endif // QT_NO_DATESTRING

    if (dt.isValid())
        dt.setTimeSpec(Qt::UTC);
    return dt;
}
*/

QGeoMapReplyOsz::~QGeoMapReplyOsz()
{
}

QString QGeoMapReplyOsz::getTileKey(const QGeoTiledMapRequest &request) const
{
    QString key = "";
//    QString tileDimension = "256";

//    if (m_mapManagerEngineCm)
//    {
//        key += m_mapManagerEngineCm->getTileStyle();
//        key += "/";
//    }
//    else
//    {
//        DBG_CM(TILES_M, ERR_L, "getTileKey(): m_mapManagerEngineCm is NULL ");
//    }
    //key += tileDimension;
    //key += "/";
    key += QString::number(request.zoomLevel());
    key += "/";
    key += QString::number(request.column());
    key += "/";
    key += QString::number(request.row());
    qDebug() << "getTileKey " << key;
    return key;
}

//QString QGeoMapReplyOsz::getTileFileName(const QString &tileKey) const
//{
//    QString fname = "tilecache";
//    fname = m_mapManagerEngineOsz->getTilesDir();//TILES_DIR;
//    fname += QDir::separator();
//    fname += tileKey + ".png";

//    qDebug() << "getTileFileName " << fname;
//    return fname;
//}

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


QFile* QGeoMapReplyOsz::isTileInCache(const QString &tileKey, QDateTime &lastModified)
{
    QFile *file = new QFile(getTileFileName(tileKey));
    if (!file->open(QIODevice::ReadOnly))
    {
            DBG_OSZ(TILES_M, "Tile is not in cache: " << getTileFileName(tileKey) );
            delete file;
            file = NULL;
    }
    else
    {
        QFileInfo fi(*file);
        QDateTime now = QDateTime::currentDateTime();
        lastModified = now;//fi.lastModified();
    }

    return file;
}

//QFile* QGeoMapReplyOsz::isTileInCache(const QString &tileKey, QDateTime &lastModified)
//{
//    QFile *file = new QFile(getTileFileName(tileKey));
//    if (!file->open(QIODevice::ReadOnly))
//    {
//            DBG_OSZ(TILES_M, "Tile is not in cache: " << getTileFileName(tileKey) );
//            qDebug() << "Tile is not in cache: " << getTileFileName(tileKey);
//            qDebug() << "Hier ansetzen und Datei aus OSZ-File fischen...";
//            delete file;
//            file = NULL;
//    }
//    else
//    {
//        qDebug() << "Tile found use it";
//        //QFileInfo fi(*file);
//        QDateTime now = QDateTime::currentDateTime();
//        lastModified = now;//fi.lastModified();
//    }

//    return file;
//}

//QNetworkReply* QGeoMapReplyOsz::networkReply() const
//{
//    return m_netReply;
//}

void QGeoMapReplyOsz::abort()
{
//    if (!m_netReply)
//        return;

//    m_netReply->abort();
//    m_netReply->deleteLater();
//    m_netReply = 0;
}

void QGeoMapReplyOsz::replyDestroyed()
{
//    m_netReply = 0;
}

// net timeout signal - will re-send tile request
// Note: now is turned off
void QGeoMapReplyOsz::timeout()
{
//    if (m_netReply) {
//        if (m_netReply->isFinished())
//            return;
//    }

//    resendRequest();
}

void QGeoMapReplyOsz::resendRequest()
{
//    // delete reply if it exist
//    if (m_netReply) {
//        m_netReply->abort();
//        m_netReply->deleteLater();
//        m_netReply = 0;
//    }

//    // delete request if it exist
//    if (m_netRequest) {
//        delete m_netRequest;
//        m_netRequest = 0;
//    }

//    if ((m_namPtr) && (m_mapManagerEngineOsz)) {
//        DBG_OSZ(TILES_M, "resendRequest(): Sending request: retries left: " << m_resendCounter);

//        if (m_rawRequest.length() == 0) {
//            m_rawRequest = m_mapManagerEngineOsz->getRequestString(m_tileRequest);
//        }

//        DBG_OSZ(TILES_M, "tileURL:" << m_rawRequest );
//        if (!m_netRequest) {
//            m_netRequest = new QNetworkRequest(QUrl(m_rawRequest));
//            //m_netRequest->setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
//            QString ua = QFileInfo(QApplication::applicationFilePath()).fileName();
//            ua.remove(QChar('"'), Qt::CaseInsensitive);
//            ua += " (Qt";
//            ua += qVersion();
//            ua += " QtMobility 1.1 ) CloudMade qt-mobility geoservice plugin";
//            m_netRequest->setRawHeader("User-Agent", ua.toAscii());

//            if (m_tileHttpLastModifiedStr.length() > 0)
//            {
//                m_netRequest->setRawHeader("If-Modified-Since", m_tileHttpLastModifiedStr.toAscii() );
//                DBG_OSZ(TILES_M, "use If-Modified-Since field for tile:" << m_tileFileName << " : " << m_tileHttpLastModifiedStr);
//            }
//            //m_mapManagerEngineCm->getNetworkDiskCache()->metaData(m_netRequest->url()).setLastModified(QDateTime::currentDateTime());
//        }

//        // create new network reply
//        m_netReply = m_namPtr->get(*m_netRequest);

//        // -------Note: this part was previously in QGeoMapReplyCm constructor
//        m_netReply->setParent(this);

//        connect(m_netReply,
//                SIGNAL(finished()),
//                this,
//                SLOT(networkFinished()));

//        connect(m_netReply,
//                SIGNAL(error(QNetworkReply::NetworkError)),
//                this,
//                SLOT(networkError(QNetworkReply::NetworkError)));
//        connect(m_netReply,
//                SIGNAL(destroyed()),
//                this,
//                SLOT(replyDestroyed()));

//        /* TODO: re-sending net request upto 3 times - do not work now
//        if (m_resendCounter > 0)
//        {
//            // No timeout on last resend, will wait reply until network timeout (could be long)
//            QTimer::singleShot(100, this, SLOT(timeout()));
//            m_resendCounter--;
//        }
//        */
//    }
}

void QGeoMapReplyOsz::networkFinished()
{
//    DBG_OSZ(TILES_M, "networkFinished() started..");

//    if (!m_netReply)
//    {
//        DBG_OSZ(TILES_M, "networkFinished(): m_netReply == 0");
//        return;
//    }

//    if (m_netReply->error() != QNetworkReply::NoError) {
//        DBG_OSZ(TILES_M, "networkFinished(): m_netReply->error() == " << m_netReply->error());
//        return;
//    }

//    int replyStatus = m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    DBG_OSZ(TILES_M, "reply status = " << replyStatus << " tileKey: " << m_tileKey);

//    if (replyStatus == HTTP_OK) {
//        QDateTime lastModified = QDateTime::currentDateTime();
//        QVariant lm = m_netReply->header(QNetworkRequest::LastModifiedHeader);
//        if (lm.isValid() && lm.canConvert<QDateTime>() )  {
//            lastModified = lm.toDateTime();
//        }

//        setMapImageData(m_netReply->readAll());
//        setMapImageFormat("PNG");
//        setFinished(true);

//	// make sure tile cache directory exists
//	QDir().mkpath(QFileInfo(m_tileFileName).dir().absolutePath());

//        // writing tile to cache:
//        QFile file( m_tileFileName );
//        if ( file.open( QIODevice::WriteOnly ) ) {

//            QDataStream out(&file);
//	    //            out << lastModified;
//            file.write(mapImageData());
//            file.close();
//            DBG_OSZ(TILES_M, "tile written to cache: " << m_tileFileName);
//            DBG_OSZ(TILES_M, "last_modified (written before .png) = " << toHttpDate(lastModified));
//        }
//        else
//        {
//            DBG_OSZ(TILES_M, "error writing file to cache: " << m_tileFileName);
//        }
//    }
//    else if (replyStatus == HTTP_NOT_MODIFIED) {
//        setFinished(true);
//        // update tile cache file modification timestamp!!
//        // TODO: clarify performance&compatibility! Now it writes 1 byte to file, and then resize to original size
//        QFile file( m_tileFileName );
//        if ( file.open( QIODevice::Append ) ) {
//            qint64 fsize = file.size();
//            if (1 == file.write("x", 1)) {
//                if (file.resize(fsize)) {
//                    DBG_OSZ(TILES_M, "OK: updated cache file modification time to current datetime: " << m_tileFileName);
//                }
//                else
//                {
//                    DBG_OSZ(TILES_M, "error resizing tile back! " << m_tileFileName);
//                }
//            }
//            file.close();
//        }
//        else
//        {
//            DBG_OSZ(TILES_M, "error setting tile cache file modification time: " << m_tileFileName);
//        }

//    }
//    else {
//        DBG_OSZ(TILES_M, "Unknown reply http status: " << replyStatus);
//    }

//    m_netReply->deleteLater();
//    m_netReply = 0;
}

//void QGeoMapReplyOsz::networkError(QNetworkReply::NetworkError error)
//{
//    if (!m_netReply)
//    {
//        DBG_OSZ(TILES_M, "networkError(): m_netReply == 0");
//        return;
//    }
//    if (error != QNetworkReply::OperationCanceledError)
//        setError(QGeoTiledMapReply::CommunicationError, m_netReply->errorString());
//    DBG_OSZ(TILES_M, "networkError(): m_netReply error: " << m_netReply->errorString());
//    m_netReply->deleteLater();
//    m_netReply = 0;
//}
