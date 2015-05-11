/*!
 * \file FourInALine.hpp
 * \brief FourInALine description
 * \author Anaël Petit
 */

#ifndef FOURINALINE_HPP
#define FOURINALINE_HPP

#define FOURINALINE_WIDTH 	7		/*!<FourInALine board width*/
#define FOURINALINE_HEIGHT 	6		/*!<FourInALine board height*/
#define FOURINALINE_LINE	4		/*!<FourInALine victorious line size*/
#define FOURINALINE_MAX_SCORE 50	/*!<FourInALine Maximal score (victory)*/

#include "Game.hpp"
#include "Board.hpp"

/*!
 * \brief Attributes than can be found in FourInALine Board Squares
 */
enum FourInALine_Attributes{
	FOURINALINE_EMPTY,
	FOURINALINE_CROSS,
	FOURINALINE_ROUND,
	FOURINALINE_PLAYABLE,
};

/*!
 * \class FourInALine
 * \brief class representing a FourInALine Game
 */
class FourInALine: public Game{
	Board<FourInALine_Attributes> * t_board;					/*!<FourInALine Game Board*/
	vector<pair<Player *, FourInALine_Attributes> > t_players;	/*!<FourInALine Players */
	Player * t_next_player;										/*!<next Player*/
	vector<Coordinates> t_last_moves;							/*!<last moves played*/

	/*!
	 * \brief count the Incomplete victory Patterns for the given Player.
	 */
	unsigned int countIncompleteVictoryPatterns(Player * player);
public:
	/*!
	 * \brief FourInALine constructor
	 * \param player_one : First Player
	 * \param player_two : Second Player
	 */
	FourInALine(Player * player_one, Player * player_two);
	~FourInALine();
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

/*!
 * \class FourInALine_Line
 * \brief FourInALine line victory Pattern
 */
class FourInALine_Line : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief Line Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_Line(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_Column
 * \brief FourInALine column victory Pattern
 */
class FourInALine_Column : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief Column Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_Column(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_FirstDiag
 * \brief FourInALine first diagonal victory Pattern
 */
class FourInALine_FirstDiag : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief First Diag Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_FirstDiag(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_SecondDiag
 * \brief FourInALine second diagonal victory Pattern
 */
class FourInALine_SecondDiag : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief Second Diag Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_SecondDiag(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_Empty
 * \brief FourInALine empty square Pattern
 */
class FourInALine_Empty : public Pattern<FourInALine_Attributes>{
public:
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_IncompleteLine
 * \brief FourInALine incomplete line Pattern
 */
class FourInALine_IncompleteLine : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief Line Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_IncompleteLine(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_IncompleteFirstDiag
 * \brief FourInALine incomplete first diagonal Pattern
 */
class FourInALine_IncompleteFirstDiag : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief First Diag Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_IncompleteFirstDiag(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class FourInALine_IncompleteSecondDiag
 * \brief FourInALine incomplete second diagonal Pattern
 */
class FourInALine_IncompleteSecondDiag : public Pattern<FourInALine_Attributes>{
	FourInALine_Attributes p_player;
public:
	/*!
	 * \brief Second Diag Pattern constructor
	 * \param player : considered Player
	 */
	FourInALine_IncompleteSecondDiag(FourInALine_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > getSquares(vector<int> variable_values);
};

#endif
