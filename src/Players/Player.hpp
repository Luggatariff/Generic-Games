/*!
 * \file Player.hpp
 * \brief Player description
 * \author Anaël Petit
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Coordinates.hpp"

/*!
 * \brief see Game.hpp
 */
class Game;

/*!
 * \class Player
 * \brief class describing a Game Player (either Artificial Intelligence or Human Player)
 */
class Player{
public:
	/*!
	 * \brief virtual method allowing a Player to choose a Coordinate to play
	 * \param game : pointer to the Game to play
	 * \return Coordinates of the choosen Square
	 */
	virtual Coordinates play(Game * game) = 0;

	/*!
	 * \brief virtual destructor for Player
	 */
	virtual ~Player() = 0;
};

#include "Game.hpp"

#endif
