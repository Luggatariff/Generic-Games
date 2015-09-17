#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

// On n'oublie pas les inclusions.

#include <QList>
#include <QPluginLoader>
#include <QObject>
#include <QString>
#include <QDir>
#include <QObject>

namespace PluginLoader {

    template<typename T> T* pluginByName(const QString& fileName) {
        QPluginLoader loader(fileName);
        QObject *plugin = loader.instance();
        return qobject_cast<T*>(plugin);
    }

    template<typename T> QList<T*> pluginByDir(const QString& dir) {
        QList<T*> ls;
        QDir plugDir = QDir(dir);
        foreach(QString file, plugDir.entryList(QDir::Files)) {
            if(T* plugin = PluginLoader::pluginByName<T>(plugDir.absoluteFilePath(file)))
                ls.push_back(plugin); // On l'ajoute ? la liste si oui.
        }
        return ls;
    }
}

#endif
