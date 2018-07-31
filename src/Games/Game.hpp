/*!
 * \file Game.hpp
 * \brief Game description
 * \author Anaël Petit
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
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
	 * \brief gets the tem_id of the Player who won (if any)
	 * \return the team id of the Team who won (-1 if no one won)
	 */
	virtual int whoWon() = 0;
	/*!
	 * \brief indicates if a given Team won the game
	 * \param team_id : considered team
	 * \return True is the Team won, false otherwise
	 */
	virtual bool isWinner(unsigned int team_id) = 0;

	/*!
	 * \brief indicates a arbitrary score as an information about the state of the Game for the given Team
	 * \param team_id : Team id for which the score is computed
	 * \return a value representing the Player's score
	 */
	virtual int score(unsigned int team_id) = 0;
	/*!
	 * \brief gets the maximal score, corresponding to the victory
	 * \return maximal score value
	 */
	virtual int victoryScore() = 0;

	/*!
	 * \brief starts the Game
	 */
	virtual void start() = 0;
	/*!
	 * \brief gets the number of Coordinates used to play
     * \return dimension of the used Coordinates for moves
	 */
    virtual unsigned int moveDimension() = 0;
	/*!
	 * \brief indicates if a Square at the given Coordinates is playable
	 * \return True if the Square at the given Coordinates is playable, False otherwise
	 */
    virtual bool isPlayableMove(Coordinates coordinates) = 0;
	/*!
     * \brief gets the list of playable Moves
     * \return list of playable Moves
	 */
    virtual vector<Coordinates> playableMoves() = 0;
	/*!
	 * \brief gets the next Player who has to play
	 * \return pointer to the next Player
	 */
	virtual Player * nextPlayer() = 0;
	/*!
	 * \brief makes the Player actually play the Square at the given Coordinates
	 * \param coordinates : coordinates of the Square to play
     * \param launchRandomEvents : launches the random events (false if they are to be forced)
	 */
    virtual void playMove(Coordinates coordinates, bool launchRandomEvents) = 0;

    /*!
     * \brief indicates if the Game is waiting for random events to be forced
     * \return true if the Game is waiting for random events to be forced
     */
    virtual bool waitingForRandomEvents() = 0;
    /*!
     * \brief gets the number of Coordinates used in random game events
     * \return dimension of the used Coordinates for random events
     */
    virtual unsigned int randomDimension() = 0;
    /*!
     * \brief gets the list of random events Coordinates
     * \return list of random events Coordinates
     */
    virtual vector<Coordinates> randomCoordinates() = 0;
    /*!
     * \brief force a random event to the given coordinates
     * \param coordinates : coordinates of the random event
     */
    virtual void forceRandom(Coordinates coordinates) = 0;

    /*!
     * \brief returns last moves and random events, from the oldest to the newest
     * \return list of last played Coordinates
     */
    virtual vector<Coordinates> lastMovesAndRandomEvents() = 0;

	/*!
	 * \brief displays the Game on the given stream
	 * \param out : stream where the Game has to be displayed
	 */
	virtual void display(std::ostream & out = std::cout) = 0;

	/*!
	 * \brief virtual function returning the name of the Game (can be dependant of its contructor parameters)
	 * \return Game name
	 */
	virtual string getName() = 0;

	/*!
	 * \brief deep copy for a Game
	 * \return new pointer with source Game copy
	 */
	virtual Game * copy() = 0;
	/*!
	 * \brief virtual Game destructor
	 */
	virtual ~Game(){};
};

#endif
