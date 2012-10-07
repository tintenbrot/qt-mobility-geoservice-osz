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

#include "qgeoserviceproviderplugin_osz.h"
#include "qgeomappingmanagerengine_osz.h"

#include <QtPlugin>

QGeoServiceProviderFactoryOsz::QGeoServiceProviderFactoryOsz() {}

QGeoServiceProviderFactoryOsz::~QGeoServiceProviderFactoryOsz() {}

QString QGeoServiceProviderFactoryOsz::providerName() const
{
    return "OSZ-Offline map";
}

int QGeoServiceProviderFactoryOsz::providerVersion() const
{
    return 12;
}


Q_EXPORT_PLUGIN2(qtgeoservices_osz, QGeoServiceProviderFactoryOsz)
