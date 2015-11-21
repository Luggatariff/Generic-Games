#include "maincontroller.h"

#include <QSignalMapper>
#include <QAction>

MainController::MainController(QObject * parent):
    QObject(parent),
    c_window(0, this)
{
    c_lastChoseGameType = QString("");
    c_signalMapper = new QSignalMapper (this);
}

MainController::~MainController(){
    delete c_signalMapper;
}

void MainController::init(){
    c_window.show();
    QList<QString> gameTypesList = c_model.getAvailableGameTypes();
    for (QList<QString>::Iterator gameTypesIterator = gameTypesList.begin(); gameTypesIterator != gameTypesList.end(); gameTypesIterator++){
        QAction * game_action = c_window.addAction(*gameTypesIterator, c_window.getGameMenu());

        connect (game_action, SIGNAL(triggered()), c_signalMapper, SLOT(map())) ;
        c_signalMapper->setMapping (game_action, *gameTypesIterator) ;
        connect (c_signalMapper, SIGNAL(mapped(QString)), this, SLOT(gameActionClicked(QString))) ;
    }
}

void MainController::gameActionClicked(QString gameType){
    QList<QPair<QLabel *, QWidget *> > widgets_list = c_model.getGameParameterWidgetList(gameType);
    c_lastChoseGameType = gameType;
    if (!widgets_list.empty()){
        c_window.displayBlockingParameterWindow(gameType, widgets_list);
    }
    else{
        parameterWindowValided();
    }
}

void MainController::parameterWindowValided(){
    if (!c_lastChoseGameType.isEmpty()){
        c_model.changeGame(c_lastChoseGameType);
        c_window.changeMainDisplay(c_model.getCurrentGameDisplay());
    }
}

