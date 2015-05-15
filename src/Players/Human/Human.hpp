/*!
 * \file Human.hpp
 * \brief Human description
 * \author Anaël Petit
 */

#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "Player.hpp"

/*!
 * \class Human
 * \brief this class represents a Human player
 */
class Human: public Player{
	string hu_name;			/*!<Player name*/
	unsigned int hu_team;	/*!<Team id*/
public:
	/*!
	 * \brief Human constructor
	 * \param name : Player name
	 */
	Human(string name, unsigned int team_id);

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());
	string getName();
	unsigned int getTeam();
};

#endif
