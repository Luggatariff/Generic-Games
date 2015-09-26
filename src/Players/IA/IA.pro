#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T16:45:53
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = IA
TEMPLATE = lib
CONFIG += plugin

DESTDIR = .

SOURCES += IA.cpp \
    ../../Model/Coordinates.cpp \
    IATree.cpp

HEADERS += IA.hpp \
    ../Player.hpp \
    ../../Games/Game.hpp \
    ../../Model/Coordinates.hpp \
    IATree.hpp

DISTFILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
