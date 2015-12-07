#include "mainmodel.h"

#include "src/pluginloader.hpp"

MainModel::MainModel()
{
    this->m_current_game=NULL;

    QList<Game *> game_list = PluginLoader::pluginByDir<Game>(qApp->applicationDirPath() + "/plugins/");
    for (QList<Game *>::iterator game_list_it=game_list.begin(); game_list_it!=game_list.end(); game_list_it++){
        this->m_game_instances.insert((*game_list_it)->getType(),(*game_list_it));
    }

    QList<Player *> player_list = PluginLoader::pluginByDir<Player>(qApp->applicationDirPath() + "/plugins/");
    for (QList<Player *>::iterator player_list_it=player_list.begin(); player_list_it!=player_list.end(); player_list_it++){
        this->m_player_instances.insert((*player_list_it)->getType(),(*player_list_it));
    }
}

QList<QPair<QLabel *, QWidget *> > MainModel::getGameParameterWidgetList(QString type){
    return m_game_instances[type]->getParameterWidgets();
}

QFrame * MainModel::getCurrentGameDisplay(){
    if (m_current_game != NULL)
        return m_current_game->display();
    return NULL;
}

QList<QString> MainModel::getAvailableGameTypes(){
    return m_game_instances.keys();
}

QList<QPair<QLabel *, QWidget *> > MainModel::getPlayerParameterWidgetList(unsigned int index){
    if (index < m_current_players.size())
        return m_current_players[index]->getParameterWidgets();
    return QList<QPair<QLabel *, QWidget *> >();
}

QString MainModel::getPlayerType(unsigned int index){
    if (index < m_current_players.size())
        return m_current_players[index]->getType();
    return UNDEFINED_TYPE;
}

QList<QString> MainModel::getAvailablePlayerTypes(){
    return m_player_instances.keys();
}

unsigned int MainModel::getPlayerNumber(){
    return m_current_players.size();
}

void MainModel::changePlayer(unsigned int index, QString type){
    if (index < m_current_players.size()){
        delete m_current_players[index];
        m_current_players[index]=m_player_instances[type]->createInstance();
        if (m_current_game != NULL){
            QObject::connect(m_current_game, SIGNAL(clickedMove(Coordinates)), m_current_players[index], SLOT(clickedMove(Coordinates)));
        }
    }
}

void MainModel::changeGame(QString type){
    delete m_current_game;
    m_current_game=m_game_instances[type]->copy();
    unsigned int player_number=m_current_game->playersNumber();
    if (player_number > m_current_players.size()){
        m_current_players.resize(player_number, NULL);
    }
    else if (player_number < m_current_players.size()){
        for (unsigned int player_index=player_number; player_index < m_current_players.size(); ++player_index){
            delete m_current_players[player_index];
            m_current_players[player_index]=NULL;
        }
        m_current_players.resize(player_number, NULL);
    }
    if (m_current_game != NULL){
        for (unsigned int player_index=0; player_index < m_current_players.size(); ++player_index){
           if (m_current_players[player_index]!=NULL){
               QObject::connect(m_current_game, SIGNAL(clickedMove(Coordinates)), m_current_players[player_index], SLOT(clickedMove(Coordinates)));
           }
        }
    }
}
