#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T16:23:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Generic-Games
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/Players/Human/Human.cpp \
    src/Players/IA/IA.cpp \
    src/Players/IA/IATree.cpp \
    src/Players/Learner/Learner.cpp \
    src/Model/Coordinates.cpp \
    src/Games/Chess/Chess.cpp \
    src/Games/Draughts/Draughts.cpp \
    src/Games/FourInALine/FourInALine.cpp \
    src/Games/Sticks/Sticks.cpp \
    src/Games/TicTacToe/TicTacToe.cpp \
    lib/sqlite-amalgamation-3081001/sqlite3.c

HEADERS  += mainwindow.h \
    src/Players/Player.hpp \
    src/Players/Human/Human.hpp \
    src/Players/IA/IA.hpp \
    src/Players/IA/IATree.hpp \
    src/Players/Learner/Learner.hpp \
    src/Model/Board.hpp \
    src/Model/Coordinates.hpp \
    src/Model/Pattern.hpp \
    src/Model/Square.hpp \
    src/Games/Game.hpp \
    src/Games/Chess/Chess.hpp \
    src/Games/Draughts/Draughts.hpp \
    src/Games/FourInALine/FourInALine.hpp \
    src/Games/Sticks/Sticks.hpp \
    src/Games/TicTacToe/TicTacToe.hpp \
    lib/sqlite-amalgamation-3081001/sqlite3.h \
    lib/sqlite-amalgamation-3081001/sqlite3ext.h \
    src/Parameters/parameter.hpp \
    src/pluginloader.hpp

FORMS    += mainwindow.ui
