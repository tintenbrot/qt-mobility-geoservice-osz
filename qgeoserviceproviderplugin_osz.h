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

#ifndef QGEOSERVICEPROVIDER_OSZ_H
#define QGEOSERVICEPROVIDER_OSZ_H

#include <qgeoserviceproviderfactory.h>
#include <QObject>

QTM_USE_NAMESPACE

class QGeoServiceProviderFactoryOsz : public QObject, public QGeoServiceProviderFactory
{
    Q_OBJECT
    Q_INTERFACES(QtMobility::QGeoServiceProviderFactory)
public:
    QGeoServiceProviderFactoryOsz();
    ~QGeoServiceProviderFactoryOsz();

    QString providerName() const;
    int providerVersion() const;

    QGeoMappingManagerEngine* createMappingManagerEngine(const QMap<QString, QVariant> &parameters,
            QGeoServiceProvider::Error *error,
            QString *errorString) const;

    static const QString defaultToken;
};

#endif
