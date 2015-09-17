#include "genericplugin.hpp"

GenericPlugin::GenericPlugin(const QString & text, bool default_state){
    p_check_box=new QCheckBox(text);
    p_check_box->setTristate(false);
    if (default_state)
        p_check_box->setCheckState(Qt::Checked);
    else
        p_check_box->setCheckState(Qt::Unchecked);

    QObject::connect(p_check_box, SIGNAL(stateChanged(int)), this, SLOT(setValue(int)));
}

GenericPlugin::~GenericPlugin(){
    delete p_check_box;
    delete p_value;
}

void * GenericPlugin::getValue(){
    return (void *)p_value;
}

QWidget * GenericPlugin::getWidget(){
    return (QWidget *)p_check_box;
}

const char * GenericPlugin::getName(){
    return "BooleanParameter";
}

void GenericPlugin::setValue(int value){
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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BooleanParameter, GenericPlugin)
#endif // QT_VERSION < 0x050000
