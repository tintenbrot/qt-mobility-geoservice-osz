TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(qtgeoservices_osz)
PLUGIN_TYPE=geoservices

include(../common.pri)

#QT += network

CONFIG += mobility
MOBILITY = location

HEADERS += \
            qgeomappingmanagerengine_osz.h \
            qgeomapreply_osz.h \
            qgeoserviceproviderplugin_osz.h \
            debug_osz.h


SOURCES += \
            qgeomappingmanagerengine_osz.cpp \
            qgeomapreply_osz.cpp \
            qgeoserviceproviderplugin_osz.cpp

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = Location NetworkServices
    #TARGET.UID3 = 0x20045CF8
}

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/osz_offline/lib
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
