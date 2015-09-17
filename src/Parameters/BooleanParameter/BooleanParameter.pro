#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T17:21:25
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = BooleanParameter
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ..

SOURCES += genericplugin.cpp

HEADERS += genericplugin.hpp \
    ../parameter.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
