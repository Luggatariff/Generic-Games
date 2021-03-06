/*!
 * \file Carlo.hpp
 * \brief Carlo description
 * \author Ana�l Petit
 */

#ifndef CARLO_HPP
#define CARLO_HPP

#include <Carlo/CarloTree.hpp>
#include "Player.hpp"

/*!
 * \class Carlo
 * \brief this class represents a Monte Caro IA Player
 */
class Carlo: public Player{
    string c_name;                                  /*!<Player name*/

    unsigned int c_maxSimulationNumber;             /*!<Maximum simulation number for each play*/

    vector<Coordinates> c_lastMovesAndRandomEvents; /*!<Last moves and random events in the Game*/
    bool c_displayTree;                             /*!<indicates if Carlo must display the choice Tree (debug)*/

protected:
    unsigned int c_team;                            /*!<Team id*/
    CarloTree * c_tree;                             /*!<Carlo Choice Tree*/

    int c_winValue;                                 /*!<Value of a victory in the score computation*/
    int c_defeatValue;                              /*!<Value of a defeat in the score computation*/
    int c_drawValue;                                /*!<Value of a draw game in the score computation*/
    int c_unfinishedValue;                          /*!<Value of a unfinished game in the score computation*/

    virtual void newCarloTree(Game * game);
public:
	/*!
	 * \brief Carlo constructor
	 */
    Carlo(string name, unsigned int teamId, unsigned int maxSimulationNumber, int winValue = 10, int defeatValue = 0, int drawValue = 2, int unfinishedValue = 1, bool displayTree = false);

    void setMaxSimulationNumber(unsigned int maxSimulationNumber);
    void setWinValue(int winValue);
    void setDefeatValue(int defeatValue);
    void setDrawValue(int drawValue);
    void setUnfinishedValue(int unfinishedValue);

    void start(Game * game);
    void end(Game * game);
    Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());

	string getName();
	unsigned int getTeam();
};

#endif
