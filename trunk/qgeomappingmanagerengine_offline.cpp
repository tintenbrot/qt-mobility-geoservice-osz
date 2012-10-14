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

#include "qgeomappingmanagerengine_osz.h"
#include "qgeomapreply_osz.h"
#include "debug_osz.h"

#include <qgeotiledmaprequest.h>

#include <QDesktopServices>
#include <QSize>
#include <QDir>
#include <QDateTime>
#include <QApplication>
#include <QFileInfo>

#include "quazip.h"
#include "quazipfile.h"


QGeoMappingManagerEngineOsz::QGeoMappingManagerEngineOsz(const QMap<QString, QVariant> &parameters, QGeoServiceProvider::Error *error, QString *errorString)
        : QGeoTiledMappingManagerEngine(parameters),
        m_parameters(parameters)
{
    qDebug() << "QGeoMappingManagerEngineOsz: Konstruktor";
    //
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    QList<QString> keys = m_parameters.keys();

    m_tileExt="png";
    m_oszfile=OSZ_FILE; //Standard
    //
    if (keys.contains("tintenbrot_offline.filename")) {
        QString oszfile = m_parameters.value("tintenbrot_offline.filename").toString();
        if (!oszfile.isEmpty())
            if (QFile(oszfile).exists())
                m_oszfile = oszfile;
    }
    m_zip.setZipName(m_oszfile);

    qDebug() << "m_oszfile=" << m_oszfile;

    setTileSize(QSize(256,256));

    //QuaZip zip(OSZ_FILE);

    //m_zip.setZipName(OSZ_FILE);
    //m_zipdir.s
    qDebug() << "Filename set";
    qDebug() << m_zip.getZipName();
    if(!m_zip.open(QuaZip::mdUnzip)) {
        qDebug() << "Manifest: Error opening OSZ-File";
        setMinimumZoomLevel(0.0);
        setMaximumZoomLevel(18.0);
    }
    else {
        qDebug() << "Zip opened check Maifest-File";
        m_zip.setCurrentFile("Manifest.txt");
        qDebug() << "Check in ZIP: Manifest=" << m_zip.hasCurrentFile();
        if (m_zip.hasCurrentFile()) {
            QuaZipFile manifest(&m_zip);
            if(!manifest.open(QIODevice::ReadOnly)) {
                qDebug() << "ZIP Error File can not be opened";
            }
            else {
                quint16 uiZoomMin=20;
                quint16 uiZoomMax=0;
                char bBuffer[64];
                qDebug() << "Manifest Size=" << manifest.size();
                //while(!manifest.atEnd()) {
                bool boolZoomFinished=false;
                while(!boolZoomFinished) {
                    //qDebug() << manifest.pos() << "," << manifest.size();
                    quint16 uiValue;
                    qint64 iLineLength;
                    //iLineLength=manifest.readLine(bBuffer,sizeof(bBuffer));
                    iLineLength=manifest.readLine(bBuffer,sizeof(bBuffer));
                    qDebug()<<bBuffer << " / " << iLineLength;
                    QString sLine(bBuffer);
                    sLine=sLine.left(iLineLength-1);
                    if (sLine.left(4)=="zoom") {
                        qDebug() << "Line=" << sLine;
                        uiValue=sLine.mid(5).toInt();
                        qDebug() << "Value=" << uiValue;
                        if (uiValue>uiZoomMax) uiZoomMax=uiValue;
                        if (uiValue<uiZoomMin) uiZoomMin=uiValue;
                    }
                    else
                        boolZoomFinished=true;
                    //qDebug() << "Ende " << manifest.pos() << "," << manifest.size();
                }
                manifest.close();
                setMinimumZoomLevel(uiZoomMin);
                setMaximumZoomLevel(uiZoomMax);
                //
                //Ermitteln des fileformates, Idealerweise mit Hilfe der nächsten Datei
                m_zip.goToFirstFile();
                m_zip.goToNextFile();
//                qDebug() << "########################################################";
//                qDebug() << "\n\nNextFileName=" << m_zip.getCurrentFileName() << "\n\n";
                m_tileExt=m_zip.getCurrentFileName().right(3);
                qDebug() << "m_tileExt=" << m_tileExt;
//                qDebug() << "########################################################";
            }
        }
}


    //SL_MAP_TYPE
    QList<QGraphicsGeoMap::MapType> types;
    types << QGraphicsGeoMap::StreetMap;    
    setSupportedMapTypes(types);

    //m_cacheSize = DEFAULT_TILE_CACHE_SIZE;

//    QList<QString> keys = m_parameters.keys();


//    if (keys.contains("mapping.cache.directory")) {
//        QString cacheDir = m_parameters.value("mapping.cache.directory").toString();
//        if (!cacheDir.isEmpty())
//            m_cacheDir = cacheDir;
//    }
//    else
//    {
//        // set default cache dir
//        //        QDir dir = QDir::temp();
//        QDir dir = QDir(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
//	qDebug() << __FUNCTION__ << "Cache at" << dir;
	
//        dir.mkdir(DEFAULT_TILE_CACHE_DIR);

//        dir.cd(DEFAULT_TILE_CACHE_DIR);
//        m_cacheDir = dir.path();
//    }
//    DBG_OSZ(TILES_M, "Setting tile cache dir to " << m_cacheDir);

//    if (keys.contains("mapping.cache.size")) {
//        bool ok = false;
//        qint64 cacheSize = m_parameters.value("mapping.cache.size").toString().toLongLong(&ok);
//        if (ok) {
//            //m_cache->setMaximumCacheSize(cacheSize);
//            m_cacheSize = cacheSize;
//            DBG_OSZ(TILES_M, "Setting tile cache size = " << m_cacheSize);
//        }
//    }

//    // first of all: delete all *.png files that may still be lurking in
//    // the old cache dir as they may affect the gallery (they are named
//    // png, but are not real png files)
//#ifdef MEEGO_EDITON
//    QDir dir=QDir::home();
//    dir.cd("MyDocs");
//#else
//    QDir dir=QDir::home();
//#endif
//    if(dir.cd(DEFAULT_TILE_CACHE_DIR)) {
//      QStringList pngFilters;
//      pngFilters << "*.png";
//      dir.setNameFilters(pngFilters);
//      QStringList pngList = dir.entryList();
//      foreach(QString name, pngList)
//	dir.remove(name);
      
//      dir.cd("..");
//      dir.rmdir(DEFAULT_TILE_CACHE_DIR);
//    }
}

QGeoMappingManagerEngineOsz::~QGeoMappingManagerEngineOsz()
{
    //cleanCacheToSize(0);
}


QGeoTiledMapReply* QGeoMappingManagerEngineOsz::getTileImage(const QGeoTiledMapRequest &request)
{
    qDebug() << "getTileImage m_tileExt=" << m_tileExt;
    QGeoTiledMapReply* mapReply = new QGeoMapReplyOsz(m_zip, m_tileExt, request, this);
    qDebug() << "getTileImage ";
    return mapReply;
}

//void QGeoMappingManagerEngineOsz::cleanCacheToSize(int sizeLimit)
//{
//    DBG_OSZ(TILES_M, "cleanCacheToSize():  start cleaning cache, sizeLimit = " << sizeLimit);

//    QDir dir;
//    dir.cd(m_cacheDir);

//    QStringList filters;
//    filters << "*.png";
//    dir.setNameFilters(filters);
//    dir.setSorting(QDir::Time);

//    qint64 totalSize = 0;   // SUM of all tiles size (not precize cache size, because of cluster size)
//    QFileInfoList list = dir.entryInfoList();
//    for (int i = 0; i < list.size(); ++i) {
//        totalSize += list.at(i).size();
//        //QFileInfo fileInfo = list.at(i);
//        //qDebug() << fileInfo.lastModified() << "    " << fileInfo.fileName() << "    " << fileInfo.size();
//    }
//    DBG_OSZ(TILES_M, "Current cache size in bytes = " << totalSize);

//    // start cleaning:
//    int listSize = list.size();
//    while ((listSize > 0) && (totalSize > sizeLimit)) {
//        totalSize -= list.at(listSize-1).size();
//        if (!dir.remove(list.at(listSize-1).fileName())) {
//            DBG_OSZ(TILES_M, "Failed to delete file: " << list.at(listSize-1).fileName());
//            totalSize += list.at(listSize-1).size();
//        }
//        listSize--;
//    }
//    DBG_OSZ(TILES_M, "Cache cleaning finished, current cache size = " << totalSize);
//}

