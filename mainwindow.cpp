#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::displayBlockingParameterWindow(QString title, QFrame * parameter_frame){
    if (parameter_frame != NULL){
        parameter_frame->setWindowIconText(title);
        parameter_frame->show();
        parameter_frame->grabMouse();
    }
}

inline void MainWindow::update_dynamic_parameter_pane(){
    QLayoutItem * item = ui->dynamicParameterFramesAreaLayout->takeAt(0);
    while (item != NULL){
        ui->dynamicParameterFramesAreaLayout->removeItem(item);
        item = ui->dynamicParameterFramesAreaLayout->takeAt(0);
    }
    for (unsigned int dpfa_index=0; dpfa_index<mw_dynamic_parameter_frame_list.size(); ++dpfa_index){
        if (mw_dynamic_parameter_frame_list[dpfa_index] != NULL){
            ui->dynamicParameterFramesAreaLayout->addWidget(mw_dynamic_parameter_frame_list[dpfa_index]);
        }
    }
}

void MainWindow::addDynamicParameterFrame(unsigned int index, QString title, QFrame * parameter_frame){
    for (unsigned int dpfa_index=mw_dynamic_parameter_frame_list.size(); dpfa_index<=index; ++dpfa_index){
        mw_dynamic_parameter_frame_list.push_back(NULL);
    }
    if (mw_dynamic_parameter_frame_list[index] != NULL){
        removeDynamicParameterFrame(index);
    }
    parameter_frame->setWindowIconText(title);
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
    QLayoutItem * item = ui->mainAreaLayout->takeAt(0);
    while (item != NULL){
        ui->mainAreaLayout->removeItem(item);
        item = ui->mainAreaLayout->takeAt(0);
    }
    ui->mainAreaLayout->addWidget(frame);
}


