#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <QtPlugin>
#include <QtWidgets>

class Parameter
{
public:
    virtual ~Parameter() { }

    virtual void * getValue() = 0;
    virtual QtWidget * getWidget() = 0;
};

Q_DECLARE_INTERFACE(Parameter, "Generic-Games.Parameter")

#endif // PARAMETER_HPP
