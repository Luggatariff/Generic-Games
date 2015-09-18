#include "booloeanparameter.hpp"

BooleanParameter::BooleanParameter(const QString & text, bool default_state){
    p_check_box=new QCheckBox(text);
    p_check_box->setTristate(false);
    if (default_state)
        p_check_box->setCheckState(Qt::Checked);
    else
        p_check_box->setCheckState(Qt::Unchecked);

    Parameter::connect(p_check_box, SIGNAL(stateChanged(int)), this, SLOT(setValue(int)));
}

BooleanParameter::~BooleanParameter(){
    delete p_check_box;
    delete p_value;
}

void * BooleanParameter::getValue(){
    return (void *)p_value;
}

QWidget * BooleanParameter::getWidget(){
    return (QWidget *)p_check_box;
}

const char * BooleanParameter::getName(){
    return "BooleanParameter";
}

void BooleanParameter::setValue(int value){
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

