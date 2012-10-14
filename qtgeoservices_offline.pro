TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(qtgeoservices_offline)
INCLUDEPATH += ./quazip-0.3/quazip
QUAZIP=./quazip-0.3/quazip

PLUGIN_TYPE=geoservices

include(../common.pri)

#QT += network

CONFIG += mobility
MOBILITY = location

HEADERS += \
            qgeomapreply_osz.h \
            qgeomapreply_sqlite.h \
            qgeomappingmanagerengine_offline.h \
            qgeoserviceproviderplugin_offline.h \
            debug_offline.h


SOURCES += \
            qgeomapreply_osz.cpp \
            qgeomapreply_sqlite.cpp \
            qgeomappingmanagerengine_offline.cpp \
            qgeoserviceproviderplugin_offline.cpp

# Include local Quazip for ZIP handling (Version 0.3)
HEADERS += $$QUAZIP/crypt.h $$QUAZIP/quachecksum32.h $$QUAZIP/quazipfileinfo.h $$QUAZIP/quazipnewinfo.h $$QUAZIP/zip.h $$QUAZIP/ioapi.h $$QUAZIP/quacrc32.h $$QUAZIP/quaadler32.h $$QUAZIP/quazipfile.h $$QUAZIP/quazip.h $$QUAZIP/unzip.h
SOURCES += $$QUAZIP/ioapi.c $$QUAZIP/unzip.c $$QUAZIP/zip.c $$QUAZIP/quaadler32.cpp $$QUAZIP/quazip.cpp $$QUAZIP/quacrc32.cpp $$QUAZIP/quazipnewinfo.cpp $$QUAZIP/quazipfile.cpp

# Include local Quazip for ZIP handling (Version 0.5)
#HEADERS += $$QUAZIP/crypt.h $$QUAZIP/quachecksum32.h $$QUAZIP/quazipfileinfo.h $$QUAZIP/quazipnewinfo.h $$QUAZIP/zip.h $$QUAZIP/ioapi.h $$QUAZIP/quacrc32.h $$QUAZIP/quaadler32.h $$QUAZIP/quazipfile.h $$QUAZIP/quazip.h $$QUAZIP/unzip.h \
#           $$QUAZIP/JlCompress.h $$QUAZIP/quagzipfile.h $$QUAZIP/quaziodevice.h $$QUAZIP/quazipdir.h $$QUAZIP/quazip_global.h
#SOURCES += $$QUAZIP/unzip.c $$QUAZIP/zip.c $$QUAZIP/quaadler32.cpp $$QUAZIP/quazip.cpp $$QUAZIP/quacrc32.cpp $$QUAZIP/quazipnewinfo.cpp $$QUAZIP/quazipfile.cpp \
#           $$QUAZIP/JlCompress.cpp $$QUAZIP/qioapi.cpp $$QUAZIP/quagzipfile.cpp $$QUAZIP/quaziodevice.cpp $$QUAZIP/quazipdir.cpp


RESOURCES = qtgeoservices_offline.qrc

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = Location NetworkServices
    #TARGET.UID3 = 0x20045CF8
}

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/qtgeoservices_offline/lib
    INSTALLS += target
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
