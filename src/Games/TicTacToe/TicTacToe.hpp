/*!
 * \file TicTacToe.hpp
 * \brief TicTacToe description
 * \author Ana�l Petit
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
	TICTACTOE_EMPTY,
	TICTACTOE_CROSS,
	TICTACTOE_ROUND
};

/*!
 * \class TicTacToe
 * \brief class representing a TicTacToe Game
 */
class TicTacToe: public Game{
	Board<TicTacToe_Attributes> * t_board;					/*!<TicTacToe Game Board*/
	vector<pair<Player *, TicTacToe_Attributes> > t_players;/*!<TicTacToe Players */
	Player * t_next_player;									/*!<next Player*/
	vector<Coordinates> t_last_moves;							/*!<last moves played*/

	/*!
	 * \brief count the Incomplete victory Patterns for the given Player. With two of them, victory is almost guaranted.
	 */
	unsigned int countIncompleteVictoryPatterns(Player * player);
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
	int whoWon();
	bool isWinner(unsigned int team_id);

	int score(unsigned int team_id);
	int victoryScore();

	void start();
    unsigned int moveDimension();
    bool isPlayableMove(Coordinates coordinates);
    vector<Coordinates> playableMoves();
    void playMove(Coordinates coordinates, bool launchRandomEvents);

    bool waitingForRandomEvents();
    unsigned int randomDimension();
    vector<Coordinates> randomCoordinates();
    void forceRandom(Coordinates coordinates);

	Player * nextPlayer();
    vector<Coordinates> lastMovesAndRandomEvents();

	void display(std::ostream & out);

	string getName();
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

/*!
 * \class TicTacToe_IncompleteLine
 * \brief TicTacToe incomplete line Pattern
 */
class TicTacToe_IncompleteLine : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief Line Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_IncompleteLine(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_IncompleteColumn
 * \brief TicTacToe incomplete column Pattern
 */
class TicTacToe_IncompleteColumn : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief Column Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_IncompleteColumn(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_IncompleteFirstDiag
 * \brief TicTacToe incomplete first diagonal Pattern
 */
class TicTacToe_IncompleteFirstDiag : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief First Diag Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_IncompleteFirstDiag(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

/*!
 * \class TicTacToe_IncompleteSecondDiag
 * \brief TicTacToe incomplete second diagonal Pattern
 */
class TicTacToe_IncompleteSecondDiag : public Pattern<TicTacToe_Attributes>{
	TicTacToe_Attributes p_player;
public:
	/*!
	 * \brief Second Diag Pattern constructor
	 * \param player : considered Player
	 */
	TicTacToe_IncompleteSecondDiag(TicTacToe_Attributes player);
	vector<VarPattern> getVariables();
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values);
};

#endif
