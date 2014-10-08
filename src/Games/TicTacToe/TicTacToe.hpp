/*!
 * \file TicTacToe.hpp
 * \brief TicTacToe description
 * \author Anaël Petit
 */

#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#define TICTACTOE_WIDTH 3		/*!<TicTacToe side size*/
#define TICTACTOE_DIMENSION 2 	/*!<TicTacToe dimension (code not adaptable yet)*/
#define TICTACTOE_MAX_SCORE 100	/*!<TicTacToe Maximal score (victory)*/

#include "Game.hpp"
#include "Board.hpp"

/*!
 * \brief Attributes than can be found in TicTacToe Board Squares
 */
enum TicTacToe_Attributes{
	EMPTY,
	CROSS,
	ROUND
};

/*!
 * \class TicTacToe
 * \brief class representing a TicTacToe Game
 */
class TicTacToe: public Game{
	Board<TicTacToe_Attributes> * t_board;					/*!<TicTacToe Game Board*/
	vector<pair<Player *, TicTacToe_Attributes> > t_players;/*!<TicTacToe Players */
	Player * t_next_player;									/*!<next Player*/
	Player * t_last_player;									/*!<last Player*/
public:
	/*!
	 * \brief TicTacToe constructor
	 * \param player_one : First Player
	 * \param player_two : Second Player
	 */
	TicTacToe(Player * player_one, Player * player_two);
	~TicTacToe();
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
	Player * lastPlayer();
	void play(Coordinates coordinates);

	void display(std::ostream & out);
};

/*!
 * \class TicTacToe_Line
 * \brief TicTacToe line victory Pattern
 */
class TicTacToe_Line : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief Line Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_Line(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_Column
 * \brief TicTacToe column victory Pattern
 */
class TicTacToe_Column : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief Column Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_Column(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_FirstDiag
 * \brief TicTacToe first diagonal victory Pattern
 */
class TicTacToe_FirstDiag : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief First Diag Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_FirstDiag(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_SecondDiag
 * \brief TicTacToe second diagonal victory Pattern
 */
class TicTacToe_SecondDiag : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief Second Diag Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_SecondDiag(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_Empty
 * \brief TicTacToe empty square Pattern
 */
class TicTacToe_Empty : public Pattern<TicTacToe_Attributes>{
public:
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

#endif
