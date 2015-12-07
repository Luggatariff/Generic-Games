#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_parameterdialog.h"
#include "maincontroller.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTextCharFormat>

ParameterDialog::ParameterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

ParameterDialog::~ParameterDialog()
{
    delete ui;
}

Ui::Dialog *ParameterDialog::getUi(){
    return ui;
}

MainWindow::MainWindow(QWidget *parent, MainController *controller) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mw_parameter_dialog(this)
{
    mw_parameter_dialog.hide();
    ui->setupUi(this);
    if (controller != NULL){
        QObject::connect(mw_parameter_dialog.getUi()->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), controller, SLOT(parameterWindowValided()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear_layout(QLayout * layout){
    QLayoutItem * item = layout->takeAt(0);
    while (item != NULL){
        layout->removeItem(item);
        item = layout->takeAt(0);
    }
}

inline void MainWindow::update_menu_bar(){
    ui->menuBar->clear();
    ui->menuBar->addMenu(ui->menuGeneric_Games);
    for (unsigned int menu_index=0; menu_index<mw_menu_list.size(); ++menu_index){
        if (mw_menu_list[menu_index] != NULL){
            ui->menuBar->addMenu(mw_menu_list[menu_index]);
        }
    }
}

QMenu * MainWindow::addMenu(unsigned int index, QString title){
    QMenu * new_menu=new QMenu(title, ui->menuBar);
    for (unsigned int menu_index=mw_menu_list.size(); menu_index<=index; ++menu_index){
        mw_menu_list.push_back(NULL);
    }
    if (mw_menu_list[index] != NULL){
        removeMenu(index);
    }
    mw_menu_list[index]=new_menu;
    update_menu_bar();
    return new_menu;
}

void MainWindow::removeMenu(unsigned int menu_index){
   if (menu_index < mw_menu_list.size()){
       delete mw_menu_list[menu_index];
       mw_menu_list[menu_index]=NULL;
       update_menu_bar();
   }
}

unsigned int MainWindow::getMenuNumber(){
    return mw_menu_list.size();
}

QMenu * MainWindow::getGameMenu(){
    return ui->menuGeneric_Games;
}

QAction * MainWindow::addAction(QString title, QMenu * menu){
    if (menu != NULL){
        QAction * new_action=new QAction(title, menu);
        new_action->setCheckable(false);
        menu->addAction(new_action);
        return new_action;
    }
    return NULL;
}

void MainWindow::displayBlockingParameterWindow(QString title, QList<QPair<QLabel *, QWidget *> > parameter_widgets){
    if (! parameter_widgets.empty()){
        mw_parameter_dialog.setWindowTitle(title);
        clear_layout(mw_parameter_dialog.getUi()->formLayout);
        for (QList<QPair<QLabel *, QWidget *> >::Iterator pw_it = parameter_widgets.begin(); pw_it != parameter_widgets.end(); pw_it++){
            mw_parameter_dialog.getUi()->formLayout->addWidget(pw_it->first);
            mw_parameter_dialog.getUi()->formLayout->addWidget(pw_it->second);
        }
        mw_parameter_dialog.show();
        mw_parameter_dialog.grabMouse();
        mw_parameter_dialog.setFocus();
    }
}

inline void MainWindow::update_dynamic_parameter_pane(){
    clear_layout(ui->dynamicParameterFramesAreaLayout);
    for (unsigned int dpfa_index=0; dpfa_index<mw_dynamic_parameter_frame_list.size(); ++dpfa_index){
        if (mw_dynamic_parameter_frame_list[dpfa_index] != NULL){
            ui->dynamicParameterFramesAreaLayout->addWidget(mw_dynamic_parameter_frame_list[dpfa_index]);
        }
    }
}

void MainWindow::addDynamicParameterFrame(unsigned int index, QString title, QList<QPair<QLabel *, QWidget *> > parameter_widgets){
    for (unsigned int dpfa_index=mw_dynamic_parameter_frame_list.size(); dpfa_index<=index; ++dpfa_index){
        mw_dynamic_parameter_frame_list.push_back(NULL);
    }
    QFrame * parameter_frame = mw_dynamic_parameter_frame_list[index];
    if (parameter_frame == NULL){
        parameter_frame = new QFrame(ui->dynamicParameterFramesArea);
        parameter_frame->setLayout(new QFormLayout(parameter_frame));
    }
    clear_layout(parameter_frame->layout());

    QLabel * title_label = new QLabel(title);
    parameter_frame->layout()->addWidget(title_label);

    for (QList<QPair<QLabel *, QWidget *> >::Iterator pw_it = parameter_widgets.begin(); pw_it != parameter_widgets.end(); pw_it++){
        parameter_frame->layout()->addWidget(pw_it->first);
        parameter_frame->layout()->addWidget(pw_it->second);
    }

    mw_dynamic_parameter_frame_list[index]=parameter_frame;
    update_dynamic_parameter_pane();
}

void MainWindow::removeDynamicParameterFrame(unsigned int index){
    if (index < mw_dynamic_parameter_frame_list.size()){
        delete mw_dynamic_parameter_frame_list[index];
        mw_dynamic_parameter_frame_list[index]=NULL;
        update_dynamic_parameter_pane();
    }
}

unsigned int MainWindow::getDynamicParameterFrameNumber(){
    return mw_dynamic_parameter_frame_list.size();
}

void MainWindow::changeMainDisplay(QFrame * frame){
    clear_layout(ui->mainAreaLayout);
    ui->mainAreaLayout->addWidget(frame);
}


