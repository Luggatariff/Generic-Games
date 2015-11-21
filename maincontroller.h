#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "mainwindow.h"
#include "mainmodel.h"
#include <QObject>
#include <QSignalMapper>

class MainController : public QObject
{
    Q_OBJECT

    MainModel c_model;
    MainWindow c_window;
    QSignalMapper * c_signalMapper;
    QString c_lastChoseGameType;
public:
    MainController(QObject * parent = 0);

    /*!
     * \brief controller initialization
     */
    void init();

public slots:
    void gameActionClicked(QString gameType);
    void parameterWindowValided();
};

#endif // MAINCONTROLLER_H
