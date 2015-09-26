#include "mainwindow.h"
#include "src/pluginloader.hpp"
#include "src/Players/Player.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QList<Player *> plugin_list=PluginLoader::pluginByDir<Player>(qApp->applicationDirPath() + "/plugins/");

    qDebug() << qApp->applicationDirPath() + "/plugins/";

    QList<Player *>::iterator plugin_list_iterator;
    for (plugin_list_iterator=plugin_list.begin(); plugin_list_iterator < plugin_list.end(); plugin_list_iterator++){
        qDebug() << (*plugin_list_iterator)->getType();
        (*plugin_list_iterator)->getParameterFrame()->setParent(&w);
        //(*plugin_list_iterator)->getParameterFrame()->setFrameStyle(QFrame::Raised);
    }

    w.show();
    int main_return=a.exec();

    for (plugin_list_iterator=plugin_list.begin(); plugin_list_iterator < plugin_list.end(); plugin_list_iterator++){
        delete (*plugin_list_iterator);
    }

    return main_return;
}
