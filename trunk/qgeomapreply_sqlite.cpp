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
//#include <QtSql>
//#include <QtSql/QSQLiteDriver>
//#include <QSqlQueryModel>
//#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

QGeoMapReplySqlite::QGeoMapReplySqlite(QString sSqliteFile, const QGeoTiledMapRequest &request, QObject *parent)
        : QGeoTiledMapReply(request, parent),
        m_tileRequest(request)
{
    bool ok;
    m_mapManagerEngineOffline = static_cast<QGeoMappingManagerEngineOffline*>(parent);

    //m_mapManagerEngineOffline->m_sqlite->open();
//    sSqliteFile=sSqliteFile.replace("/","\\");

    m_sqlite=new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE","tintenbrotConnect"));
    qDebug() << "isValid() = " << m_sqlite->isValid();
    //m_sqlite->addDatabase("QSQLITE", sSqliteFile);

    //m_sqlite=new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE",sSqliteFile));

    qDebug() << "Sqlite: m_offlinefile=" << sSqliteFile;
    m_sqlite->setDatabaseName(sSqliteFile);
    ok=m_sqlite->open();
    qDebug() << "SQLITE: Open DB=" << ok;
    if (!ok) {
        qDebug() << "lastError=" << m_sqlite->lastError();
    }
//    qDebug() << "Tables: " << m_sqlite->tables().size();
//    foreach(QString table, m_sqlite->tables())
//    {
//        qDebug() << table;
//    }
    qDebug() << "Column=" << request.column();
    qDebug() << "ConnectionName=" << m_sqlite->connectionName();
    QSqlQuery query(*m_sqlite);
    //query.clear();

    QString sQuery=QString("SELECT image FROM tiles WHERE x=%1 AND y=%2").arg(request.column()).arg(request.row());
    qDebug() << "Query=" << sQuery;
    ok=query.prepare(sQuery);
    //ok=query.prepare(getQueryString(request));
    //ok= query.prepare(QString("SELECT image FROM tiles WHERE x=2073 AND y=1556"));
    //query.prepare(QString("SELECT * FROM tiles"));
    //query.prepare(QString("SELECT minzoom FROM info"));
    //ok = query.prepare(QString("SELECT * FROM tiles"));
    if (!ok) {
        qDebug() << query.lastError();
    }
    ok = query.exec();
    qDebug() << "SQLITE: Query: " << ok;
    if (!ok) {
        qDebug() << query.lastError();
    }
    //
    // Image ist eindeutig, also mit Next kommt ein Bild oder nicht
    if (query.next())
    {
        setMapImageData(query.value(0).toByteArray());
        setMapImageFormat("JPG");
        setFinished(true);
    }
    else
    {
        QFile fileError(":tile_notavailable");
        fileError.open(QIODevice::ReadOnly);
        setMapImageData(fileError.readAll());
        setMapImageFormat("PNG");
        fileError.close();
        setFinished(true);
    }
    m_sqlite->close();
//    while (query.next()) {
//        //qDebug("id = %d, text = %s.", query.value(0).toInt(), qPrintable(query.value(1).toString()));
//        setMapImageData();
//    }


//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(sSqliteFile);
//    if (db.open()) {
//        qDebug() << "Database opened successfully";

//        db.close();
//    }
//    else
//        qDebug() << "Could not open database-file";
//    //QSql MySql();

}


QGeoMapReplySqlite::~QGeoMapReplySqlite()
{
    m_sqlite->close();
    delete m_sqlite;
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
