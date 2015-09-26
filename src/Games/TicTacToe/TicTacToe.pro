#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T18:56:24
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = TicTacToe
TEMPLATE = lib
CONFIG += plugin

DESTDIR = .

SOURCES += TicTacToe.cpp \
    ../../Model/Coordinates.cpp

HEADERS += TicTacToe.hpp \
    ../Game.hpp \
    ../../Model/Coordinates.hpp \
    ../../Model/Board.hpp \
    ../../Model/Pattern.hpp \
    ../../Model/Square.hpp
DISTFILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
