#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T17:21:25
#
#-------------------------------------------------

QT       += core gui

TARGET = BooleanParameter
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/generic

SOURCES += genericplugin.cpp

HEADERS += genericplugin.hpp
DISTFILES += BooleanParameter.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
