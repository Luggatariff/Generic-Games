#ifndef BOOLEANPARAMETER_HPP
#define BOOLEANPARAMETER_HPP

#include <QObject>
#include <parameter.hpp>
#include <QCheckBox>

class BooleanParameter : public QObject, public Parameter
{
    Q_OBJECT
    Q_INTERFACES(Parameter)

    QCheckBox * p_check_box;
    bool * p_value;

public:
    BooleanParameter(const QString & text, bool default_state);

    virtual ~BooleanParameter();
    virtual void * getValue() = 0;
    virtual QtWidget * getWidget() = 0;

private slots:
    void setValue(int value);
};

#endif // BOOLEANPARAMETER_HPP
