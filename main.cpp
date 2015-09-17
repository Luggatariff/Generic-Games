#include "mainwindow.h"
#include "src/pluginloader.hpp"
#include "src/Parameters/parameter.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QList<Parameter *> plugin_list=PluginLoader::pluginByDir<Parameter>(qApp->applicationDirPath() + "/plugins/");

    qDebug() << qApp->applicationDirPath() + "/plugins/";

    QList<Parameter *>::iterator plugin_list_iterator;
    for (plugin_list_iterator=plugin_list.begin(); plugin_list_iterator < plugin_list.end(); plugin_list_iterator++){
        qDebug() << 1;
        qDebug() << (*plugin_list_iterator)->getName();
    }

    w.show();

    return a.exec();
}
