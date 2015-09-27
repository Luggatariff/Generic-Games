#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    MainModel mw_model;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMenu * addMenu(QString title);

    QMenu * getGameMenu();

    QAction * addAction(QMenu * menu);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
