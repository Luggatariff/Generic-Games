/*!
 * \file TicTacToe.hpp
 * \brief TicTacToe description
 * \author Anaël Petit
 */

#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#define TICTACTOE_WIDTH 3
#define TICTACTOE_DIMENSION 2 //Not adaptable
#define TICTACTOE_MAX_SCORE 100

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

template class Board<TicTacToe_Attributes>;
template class Square<TicTacToe_Attributes>;

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
	TicTacToe_Line(TicTacToe_Attributes player){
		p_player = player;
	}
	vector<VarPattern> getVariables(){
		vector<VarPattern> result;

		VarPattern line;
		line.v_begin = 0;
		line.v_end = TICTACTOE_WIDTH;
		line.v_step = 1;
		result.push_back(line);

		return result;
	}
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values){
		vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
		Square<TicTacToe_Attributes> marked_square;
		marked_square.addAttribute(p_player);

		int line = variable_values[0];
		for (unsigned int column = 0; column<TICTACTOE_WIDTH; column++){
			Coordinates coordinates(TICTACTOE_DIMENSION);
			coordinates[0] = line; coordinates[1] = column;
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		}

		return result;
	}
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
	TicTacToe_Column(TicTacToe_Attributes player){
		p_player = player;
	}
	vector<VarPattern> getVariables(){
		vector<VarPattern> result;

		VarPattern column;
		column.v_begin = 0;
		column.v_end = TICTACTOE_WIDTH;
		column.v_step = 1;
		result.push_back(column);

		return result;
	}
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values){
		vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
		Square<TicTacToe_Attributes> marked_square;
		marked_square.addAttribute(p_player);

		int column = variable_values[0];
		for (unsigned int line = 0; line<TICTACTOE_WIDTH; line++){
			Coordinates coordinates(TICTACTOE_DIMENSION);
			coordinates[0] = line; coordinates[1] = column;
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		}

		return result;
	}
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
	TicTacToe_FirstDiag(TicTacToe_Attributes player){
		p_player = player;
	}
	vector<VarPattern> getVariables(){
		vector<VarPattern> result;
		return result;
	}
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values){
		vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
		Square<TicTacToe_Attributes> marked_square;
		marked_square.addAttribute(p_player);

		for (unsigned int d = 0; d<TICTACTOE_WIDTH; d++){
			Coordinates coordinates(TICTACTOE_DIMENSION);
			coordinates[0] = d; coordinates[1] = d;
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		}

		return result;
	}
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
	TicTacToe_SecondDiag(TicTacToe_Attributes player){
		p_player = player;
	}
	vector<VarPattern> getVariables(){
		vector<VarPattern> result;
		return result;
	}
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values){
		vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
		Square<TicTacToe_Attributes> marked_square;
		marked_square.addAttribute(p_player);

		for (unsigned int d = 0; d<TICTACTOE_WIDTH; d++){
			Coordinates coordinates(TICTACTOE_DIMENSION);
			coordinates[0] = (TICTACTOE_WIDTH - 1)-d; coordinates[1] = d;
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		}

		return result;
	}
};

/*!
 * \class TicTacToe_Empty
 * \brief TicTacToe empty square Pattern
 */
class TicTacToe_Empty : public Pattern<TicTacToe_Attributes>{
public:
	vector<VarPattern> getVariables(){
		vector<VarPattern> result;

		VarPattern line;
		line.v_begin = 0;
		line.v_end = TICTACTOE_WIDTH;
		line.v_step = 1;
		result.push_back(line);

		VarPattern column;
		column.v_begin = 0;
		column.v_end = TICTACTOE_WIDTH;
		column.v_step = 1;
		result.push_back(column);

		return result;
	}
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > getSquares(vector<int> variable_values){
		vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
		Square<TicTacToe_Attributes> marked_square;
		marked_square.addAttribute(EMPTY);

		int line = variable_values[0];
		int column = variable_values[1];
		Coordinates coordinates(TICTACTOE_DIMENSION);
		coordinates[0] = line; coordinates[1] = column;
		result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));

		return result;
	}
};

/*!
 * \class TicTacToe
 * \brief class representing a TicTacToe Game
 */
class TicTacToe: public Game{
	Board<TicTacToe_Attributes> * t_board;	/*!<TicTacToe Game Board*/
	vector<Player *> t_players;				/*!<TicTacToe Players */

public:
	/*!
	 * \brief TicTacToe constructor
	 * \param player_one : First Player
	 * \param player_two : Second Player
	 */
	TicTacToe(Player * player_one, Player * player_two){
		Coordinates board_size(TICTACTOE_DIMENSION);
		for (unsigned int dim = 0; dim<TICTACTOE_DIMENSION; dim++)
			board_size[dim] = TICTACTOE_WIDTH;
		this->t_board = new Board<TicTacToe_Attributes>(board_size);

		this->t_players.push_back(player_one);
		this->t_players.push_back(player_two);
	}

	~TicTacToe(){
		delete t_board;
	}

	vector<Player *> players(){
		return t_players;
	}

	bool isEnded(){
		TicTacToe_Empty * pattern_empty = new TicTacToe_Empty();
		bool result = (t_board->checkPattern(pattern_empty)==0);
		delete pattern_empty;
		return result;
	}
	bool isWon(){
		return (whoWon() != NULL);
	}
	Player * whoWon(){
		Player * result = NULL;

		TicTacToe_Line * pattern_line = new TicTacToe_Line(CROSS);
		TicTacToe_Column * pattern_column = new TicTacToe_Column(CROSS);
		TicTacToe_FirstDiag * pattern_firstdiag = new TicTacToe_FirstDiag(CROSS);
		TicTacToe_SecondDiag * pattern_seconddiag = new TicTacToe_SecondDiag(CROSS);
		if ((t_board->checkPattern(pattern_line) +
			t_board->checkPattern(pattern_column) +
			t_board->checkPattern(pattern_firstdiag) +
			t_board->checkPattern(pattern_seconddiag)) > 0){
			result = t_players[0];
		}
		else{
			delete pattern_line;
			delete pattern_column;
			delete pattern_firstdiag;
			delete pattern_seconddiag;
			TicTacToe_Line * pattern_line = new TicTacToe_Line(ROUND);
			TicTacToe_Column * pattern_column = new TicTacToe_Column(ROUND);
			TicTacToe_FirstDiag * pattern_firstdiag = new TicTacToe_FirstDiag(ROUND);
			TicTacToe_SecondDiag * pattern_seconddiag = new TicTacToe_SecondDiag(ROUND);

			if ((t_board->checkPattern(pattern_line) +
				t_board->checkPattern(pattern_column) +
				t_board->checkPattern(pattern_firstdiag) +
				t_board->checkPattern(pattern_seconddiag)) > 0){
				result = t_players[1];
			}
		}
		delete pattern_line;
		delete pattern_column;
		delete pattern_firstdiag;
		delete pattern_seconddiag;

		return result;
	}

	int score(Player * player){
		Player * winner = whoWon();
		if (winner == player) return TICTACTOE_MAX_SCORE;
		if (winner == NULL) return 0;
		return -TICTACTOE_MAX_SCORE;
	}

	void start(){
		Player * swap = t_players[0];
		t_players[0] = t_players[1];
		t_players[1] = swap;

		Coordinates coordinates(2);
		for (coordinates[0] = 0; coordinates[0] < TICTACTOE_WIDTH; coordinates[0]++){
			for (coordinates[1] = 0; coordinates[1] < TICTACTOE_WIDTH; coordinates[1]++){
				t_board->getSquare(coordinates)->delAttribute(CROSS);
				t_board->getSquare(coordinates)->delAttribute(ROUND);
				t_board->getSquare(coordinates)->addAttribute(EMPTY);
			}
		}
	}
	unsigned int dimension(){
		return TICTACTOE_DIMENSION;
	}
	bool isPlayable(Coordinates coordinates){
		return (t_board->getSquare(coordinates)->isAttribute(EMPTY));
	}
	void play(Player * player, Coordinates coordinates){
		if (isPlayable(coordinates)){
			TicTacToe_Attributes p_player;
			if (player == t_players[0])
				p_player = CROSS;
			else
				p_player = ROUND;
			t_board->getSquare(coordinates)->delAttribute(EMPTY);
			t_board->getSquare(coordinates)->addAttribute(p_player);
		}
	}

	void display(std::ostream & out){
		out<<" ";
		for(int column=0; column<TICTACTOE_WIDTH; column++)
			out<<"|"<<column;
		out<<"|"<<std::endl;
		Coordinates coordinates(TICTACTOE_DIMENSION);
		for(coordinates[0]=0; coordinates[0]<TICTACTOE_WIDTH; coordinates[0]++){
			out<<coordinates[0];
			for(coordinates[1]=0; coordinates[1]<TICTACTOE_WIDTH; coordinates[1]++){
				out<<"|";
				if (t_board->getSquare(coordinates)->isAttribute(CROSS))
					out<<'X';
				else if (t_board->getSquare(coordinates)->isAttribute(ROUND))
					out<<'O';
				else
					out<<" ";
			}
			out<<"|"<<std::endl;
		}
		out<<" ";
		for(int column=0; column<TICTACTOE_WIDTH; column++)
			out<<"|-";
		out<<"|"<<std::endl;
	}
};

#endif
