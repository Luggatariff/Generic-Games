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
public:
	Coordinates play(Game * game);
};

#endif
