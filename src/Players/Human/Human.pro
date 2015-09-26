#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T14:40:49
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = Human
TEMPLATE = lib
CONFIG += plugin

DESTDIR = .

SOURCES += Human.cpp \
    ../../Model/Coordinates.cpp

HEADERS += Human.hpp \
    ../Player.hpp \
    ../../Games/Game.hpp \
    ../../Model/Coordinates.hpp
DISTFILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
