#ifndef GENERICPLUGIN_HPP
#define GENERICPLUGIN_HPP

#include <../parameter.hpp>
#include <QCheckBox>

class GenericPlugin: public QObject, public Parameter
{
    Q_OBJECT
    Q_INTERFACES(Parameter)

    QCheckBox * p_check_box;
    bool * p_value;

public:
    GenericPlugin(const QString & text = "", bool default_state = false);

    virtual ~GenericPlugin();
    virtual void * getValue();
    virtual QWidget * getWidget();
    virtual const char * getName();

private slots:
    void setValue(int value);
};

#endif // GENERICPLUGIN_HPP
