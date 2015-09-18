#ifndef BooleanParameter_HPP
#define BooleanParameter_HPP

#include <../parameter.hpp>
#include <QCheckBox>

class BooleanParameter: public Parameter
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Generic-Games.Parameter" FILE "")
    Q_INTERFACES(Parameter)

    QCheckBox * p_check_box;
    bool * p_value;

public:
    BooleanParameter(const QString & text = "", bool default_state = false);

    virtual ~BooleanParameter();
    virtual void * getValue();
    virtual QWidget * getWidget();
    virtual const char * getName();

private slots:
    void setValue(int value);
};

#endif // BooleanParameter_HPP
