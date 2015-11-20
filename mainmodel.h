#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QMap>
#include <src/Players/Player.hpp>
#include <QApplication>

#define UNDEFINED_TYPE "UNDEFINED"

class MainModel
{
    QMap<QString, Player *> m_player_instances;   /*!<Loaded Player instances*/
    QMap<QString, Game *> m_game_instances;       /*!<Loaded Game instances*/
    Game * m_current_game;                        /*!<Current selected Game*/
    vector<Player *> m_current_players;           /*!<Current selected Players*/
public:
    MainModel();

    /*!
     * \brief gets a Game type Parameter frame
     * \param type : Game type
     * \return Parameter Frame
     */
    QFrame * getGameParameterFrame(QString type);

    /*!
     * \brief gets current Game display Frame
     * \return Game Display Frame
     */
    QFrame * getCurrentGameDisplay();

    /*!
     * \brief gets available Games Types
     * \return Game Types list
     */
    QList<QString> getAvailableGameTypes();

    /*!
     * \brief gets a Player Parameter Frame
     * \param index : Player Number
     * \return Parameter Frame
     */
    QFrame * getPlayerParameterFrame(unsigned int index);

    /*!
     * \brief gets a Player type
     * \param index : Player Number
     * \return Player type
     */
    QString getPlayerType(unsigned int index);

    /*!
     * \brief gets available Player Types
     * \return Player Types list
     */
    QList<QString> getAvailablePlayerTypes();

    /*!
     * \brief gets the Total Number of Players
     * \return Player Quantity
     */
    unsigned int getPlayerNumber();

public slots:
    /*!
     * \brief called when the user wants to change Player type
     * \param index : Player number
     * \param type : Player type
     */
    void changePlayer(unsigned int index, QString type);

    /*!
     * \brief called when the user wants to change Game type
     * \param type : Game type
     */
    void changeGame(QString type);
};

#endif // MAINMODEL_H
