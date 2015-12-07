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
    }
    connect (c_signalMapper, SIGNAL(mapped(QString)), this, SLOT(gameActionClicked(QString))) ;
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
        unsigned int player_index;
        for (player_index = 0; player_index < c_model.getPlayerNumber(); player_index++){
            QMenu * player_menu = c_window.addMenu(player_index, QString("Player %1").arg(player_index + 1));
            QList<QString> player_types = c_model.getAvailablePlayerTypes();

            for (unsigned int player_types_index = 0; (int)player_types_index < player_types.size(); player_types_index++){
                QAction * player_action = c_window.addAction(player_types.at(player_types_index), player_menu);
                QPair<unsigned int, QString> * playerInformation = new QPair<unsigned int, QString>();
                playerInformation->first = player_index;
                playerInformation->second = player_types.at(player_types_index);
                connect (player_action, SIGNAL(triggered()), c_signalMapper, SLOT(map())) ;
                c_signalMapper->setMapping (player_action, (QObject *)playerInformation) ;
            }
        }
        connect (c_signalMapper, SIGNAL(mapped(QObject *)), this, SLOT(playerActionClicked(QObject *))) ;
        while (player_index < c_window.getMenuNumber()){
           c_window.removeMenu(player_index);
           player_index++;
        }

        c_window.changeMainDisplay(c_model.getCurrentGameDisplay());
    }
}

void MainController::playerActionClicked(QObject * playerInformationObject){
    QPair<unsigned int, QString> * playerInformation = (QPair<unsigned int, QString> *)playerInformationObject;
    c_model.changePlayer(playerInformation->first, playerInformation->second);
    QList<QPair<QLabel *, QWidget *> > widgets_list = c_model.getPlayerParameterWidgetList(playerInformation->first);
    c_window.addDynamicParameterFrame(playerInformation->first, QString("Player %1 - ").arg(playerInformation->first + 1) + playerInformation->second, widgets_list);
}

