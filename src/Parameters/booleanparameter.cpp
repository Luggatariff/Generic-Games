#include "booleanparameter.hpp"

BooleanParameter::BooleanParameter(const QString & text, bool default_state){
    p_check_box=new QtCheckBox(text);
    p_check_box->setTristate(false);
    if (default_state)
        p_check_box->setCheckState(Qt::Checked);
    else
        p_check_box->setCheckState(Qt::Unchecked);

    QObject::connect(p_check_box, SIGNAL(stateChanged(int)), this, SLOT(setValue(int)));
}

virtual BooleanParameter::~BooleanParameter(){
    delete p_check_box;
    delete p_value;
}

virtual void * BooleanParameter::getValue(){
    return (void *)p_value;
}

virtual QtWidget * BooleanParameter::getWidget(){
    return p_check_box;
}

void setValue(int value){
    Qt::CheckState state=(Qt::CheckState)value;
    switch (state){
    case Qt::Checked:{
        *p_value=true;
        break;
    }
    case Qt::Unchecked:{
        *p_value=false;
        break;
    }
    default:
        break;
    }
}

Q_EXPORT_PLUGIN2(plugin_boolean_parameter, BooleanParameter);
