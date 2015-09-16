#ifndef GENERICPLUGIN_HPP
#define GENERICPLUGIN_HPP

#include <QGenericPlugin>


class GenericPlugin : public QGenericPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "BooleanParameter.json")
#endif // QT_VERSION >= 0x050000

public:
    GenericPlugin(QObject *parent = 0);
};

#endif // GENERICPLUGIN_HPP
