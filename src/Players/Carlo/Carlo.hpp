/*!
 * \file Carlo.hpp
 * \brief Carlo description
 * \author Anaël Petit
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
    unsigned int c_team;                            /*!<Team id*/

    unsigned int c_maxSimulationNumber;             /*!<Maximum simulation number for each play*/

    CarloTree * c_tree;                             /*!<Carlo Choice Tree*/
    vector<Coordinates> c_lastMovesAndRandomEvents; /*!<Last moves and random events in the Game*/
    bool c_displayTree;                             /*!<indicates if Carlo must display the choice Tree (debug)*/

    int c_winValue;
    int c_defeatValue;
    int c_drawValue;
    int c_unfinishedValue;
public:
	/*!
	 * \brief Carlo constructor
	 */
    Carlo(string name, unsigned int teamId, unsigned int maxSimulationNumber, int winValue = 10, int defeatValue = 0, int drawValue = 2, int unfinishedValue = 1, bool displayTree = false);

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());
	string getName();
	unsigned int getTeam();
};

#endif
