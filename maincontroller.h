#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "mainwindow.h"
#include "mainmodel.h"

class MainController
{
    MainModel c_model;
    MainWindow c_window;
public:
    MainController();

    /*!
     * \brief controller initialization
     */
    void init();
signals:

public slots:
};

#endif // MAINCONTROLLER_H
