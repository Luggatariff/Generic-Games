#include "maincontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainController c;
    c.init();
    int main_return=a.exec();
    return main_return;
}
