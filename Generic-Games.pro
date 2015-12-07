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
    mainmodel.cpp \
    maincontroller.cpp \
    src/Model/Coordinates.cpp

HEADERS  += mainwindow.h \
    src/Players/Player.hpp \
    src/Games/Game.hpp \
    src/pluginloader.hpp \
    mainmodel.h \
    maincontroller.h \
    src/Model/Coordinates.hpp

FORMS    += mainwindow.ui \
    parameterdialog.ui
