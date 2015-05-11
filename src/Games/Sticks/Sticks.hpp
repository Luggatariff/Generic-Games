/*!
 * \file Sticks.hpp
 * \brief Sticks description
 * \author Anaël Petit
 */

#ifndef STICKS_HPP
#define STICKS_HPP

#define STICKS_MAX_AT_ONCE	4	/*!<Maximum number of Sticks that can be taken at once*/
#define STICKS_MAX_SCORE	100	/*!<Sticks Maximal score (victory)*/

#include "Game.hpp"
#include "Board.hpp"

/*!
 * \brief Attributes than can be found in Sticks Board Squares
 */
enum Sticks_Attributes{
	STICKS_NO_STICK,
	STICKS_STICK,
};

/*!
 * \class Sticks
 * \brief class representing a Sticks Game
 */
class Sticks: public Game{
	Board<Sticks_Attributes> * t_board;	/*!<Sticks Game Board*/
	vector<Player *> t_players;			/*!<Sticks Players */
	Player * t_next_player;				/*!<next Player*/
	vector<Coordinates> t_last_moves;	/*!<last moves played*/

	unsigned int t_max_sticks_number;	/*!<Number of sticks at the beginning*/
	unsigned int t_sticks_remaining;	/*!<Number of Sticks remaining. When equal to zero, Game is finished*/
public:
	/*!
	 * \brief Sticks constructor
	 * \param player_one : First Player
	 * \param player_two : Second Player
	 * \param sticks_number : Number of sticks
	 */
	Sticks(Player * player_one, Player * player_two, unsigned int sticks_number);
	~Sticks();
	Game * copy();

	vector<Player *> players();

	bool isEnded();
	bool isWon();
	Player * whoWon();
	bool isWinner(Player * player);

	int score(Player * player);
	int victoryScore();

	void start();
	unsigned int dimension();
	bool isPlayable(Coordinates coordinates);
	vector<Coordinates> playableCoordinates();

	Player * nextPlayer();
	void play(Coordinates coordinates);
	vector<Coordinates> lastMoves();

	void display(std::ostream & out);
	string getName();
};

#endif
