TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(qtgeoservices_osz)
PLUGIN_TYPE=geoservices

CONFIG += mobility
MOBILITY = location

target.path=$$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS += target

HEADERS += \
            qgeoserviceproviderplugin_osz.h


SOURCES += \
            qgeoserviceproviderplugin_osz.cpp


OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
