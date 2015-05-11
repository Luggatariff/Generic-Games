/*!
 * \file Learner.hpp
 * \brief Learner description
 * \author Anaël Petit
 */

#ifndef LEARNER_HPP
#define LEARNER_HPP

#include "Player.hpp"

/*!
 * \class Learner
 * \brief this class represents a Learner player
 */
class Learner: public Player{
	string lea_name;		/*!<Player name*/
	Player * lea_teacher; 	/*!<Player to learn from*/
public:
	/*!
	 * \brief Learner constructor
	 * \param name : Player name
	 */
	Learner(string name, Player * teacher);

	void start(Game * game);
	Coordinates play(Game * game);
	string getName();
};

#endif
