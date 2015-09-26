#include "mainwindow.h"
#include "src/pluginloader.hpp"
#include "src/Players/Player.hpp"
#include "src/Games/TicTacToe/TicTacToe.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    int main_return=a.exec();

    return main_return;
}
