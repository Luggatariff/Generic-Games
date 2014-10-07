/*!
 * \file Game.hpp
 * \brief Game description
 * \author Ana�l Petit
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "Player.hpp"

/*!
 * \class Game
 * \brief class representing a Game, its number of Player, Coordinates to play, display and ways to know if it ended
 */
class Game{
public:
	/*!
	 * \brief gets the Players list
	 * \return a list of Players and their number
	 */
	virtual vector<Player *> players() = 0;

	/*!
	 * \brief allows to know if the Game has ended
	 * \return True if the Game has ended, False otherwise
	 */
	virtual bool isEnded() = 0;
	/*!
	 * \brief allows to know if the Game has been won by someone
	 * \return True if the Game has been won, False otherwise
	 */
	virtual bool isWon() = 0;
	/*!
	 * \brief gets a pointer to the Player who won (if any)
	 * \return a pointer to the Player who won (NULL if no one won)
	 */
	virtual Player * whoWon() = 0;
	/*!
	 * \brief indicates a arbitrary score as an information about the state of the Game for the given Player
	 * \param player : Player for which the score is computed
	 * \return a value representing the Player's score
	 */
	virtual int score(Player * player) = 0;

	/*!
	 * \brief starts the Game
	 */
	virtual void start() = 0;
	/*!
	 * \brief indicates if a Square at the given Coordinates is playable
	 * \return True if the Square at the given Coordinates is playable, False otherwise
	 */
	virtual bool isPlayable(Coordinates coordinates) = 0;
	/*!
	 * \brief makes the Player actually play the Square at the given Coordinates
	 * \param player : pointer to the Player who plays
	 * \param coordinates : coordinates of the Square to play
	 */
	virtual void play(Player * player, Coordinates coordinates) = 0;

	/*!
	 * \brief displays the Game on the given stream
	 * \param out : stream where the Game has to be displayed
	 */
	virtual void display(std::ostream & out = std::cout) = 0;

	/*!
	 * \brief virtual Game destructor
	 */
	virtual ~Game() = 0;
};

#endif
