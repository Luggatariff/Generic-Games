/*!
 * \file TicTacToe.cpp
 * \brief TicTacToe code
 * \author Anaël Petit
 */

#include "TicTacToe.hpp"

TicTacToe::TicTacToe(Player * player_one, Player * player_two){
	Coordinates board_size(TICTACTOE_DIMENSION);
	for (unsigned int dim = 0; dim<TICTACTOE_DIMENSION; dim++)
		board_size[dim] = TICTACTOE_WIDTH;
	this->t_board = new Board<TicTacToe_Attributes>(board_size);

	this->t_players.push_back(pair<Player *, TicTacToe_Attributes>(player_one, TICTACTOE_CROSS));
	this->t_players.push_back(pair<Player *, TicTacToe_Attributes>(player_two, TICTACTOE_ROUND));
	this->t_next_player = player_one;
}

TicTacToe::~TicTacToe(){
	delete t_board;
}

Game * TicTacToe::copy(){
	TicTacToe * result = new TicTacToe(this->t_players[0].first, this->t_players[1].first);
	delete result->t_board;
	result->t_board = this->t_board->copy();
	result->t_next_player = this->t_next_player;
	return (Game *)result;
}

vector<Player *> TicTacToe::players(){
	vector<Player *> result;
	for (unsigned int i_p = 0; i_p < t_players.size(); i_p++)
		result.push_back(t_players[i_p].first);
	return result;
}

bool TicTacToe::isEnded(){
	if (isWon()) return true;

	TicTacToe_Empty * pattern_empty = new TicTacToe_Empty();
	bool result = (t_board->checkPattern(pattern_empty)==0);
	delete pattern_empty;
	return result;
}
bool TicTacToe::isWon(){
	return (whoWon() != NULL);
}
Player * TicTacToe::whoWon(){
	if (isWinner(t_players[0].first)) return t_players[0].first;
	if (isWinner(t_players[1].first)) return t_players[1].first;
	return NULL;
}
bool TicTacToe::isWinner(Player * player){
	bool result = false;

	TicTacToe_Attributes p_player;
	if (player == t_players[0].first)
		p_player = t_players[0].second;
	else
		p_player = t_players[1].second;

	TicTacToe_Line * pattern_line = new TicTacToe_Line(p_player);
	result = (t_board->checkPattern(pattern_line) > 0);
	delete pattern_line;

	if (!result){
		TicTacToe_Column * pattern_column = new TicTacToe_Column(p_player);
		result = (t_board->checkPattern(pattern_column) > 0);
		delete pattern_column;
	}
	if (!result){
		TicTacToe_FirstDiag * pattern_firstdiag = new TicTacToe_FirstDiag(p_player);
		result = (t_board->checkPattern(pattern_firstdiag) > 0);
		delete pattern_firstdiag;
	}
	if (!result){
		TicTacToe_SecondDiag * pattern_seconddiag = new TicTacToe_SecondDiag(p_player);
		result = (t_board->checkPattern(pattern_seconddiag) > 0);
		delete pattern_seconddiag;
	}
	return result;
}
unsigned int TicTacToe::countIncompleteVictoryPatterns(Player * player){
	unsigned int result = 0;

	TicTacToe_Attributes p_player;
	if (player == t_players[0].first)
		p_player = t_players[0].second;
	else
		p_player = t_players[1].second;

	TicTacToe_IncompleteLine * pattern_line = new TicTacToe_IncompleteLine(p_player);
	result += t_board->checkPattern(pattern_line);
	delete pattern_line;
	TicTacToe_IncompleteColumn * pattern_column = new TicTacToe_IncompleteColumn(p_player);
	result += t_board->checkPattern(pattern_column);
	delete pattern_column;
	TicTacToe_IncompleteFirstDiag * pattern_firstdiag = new TicTacToe_IncompleteFirstDiag(p_player);
	result += t_board->checkPattern(pattern_firstdiag);
	delete pattern_firstdiag;
	TicTacToe_IncompleteSecondDiag * pattern_seconddiag = new TicTacToe_IncompleteSecondDiag(p_player);
	result += t_board->checkPattern(pattern_seconddiag);
	delete pattern_seconddiag;

	return result;
}

int TicTacToe::score(Player * player){
	Player * winner = whoWon();
	if (winner == player) return victoryScore();
	if (winner == NULL){
		int incomplete_patterns_count = 0;
		for (unsigned int i_p = 0; i_p < t_players.size(); i_p++){
			unsigned int temp_count = countIncompleteVictoryPatterns(t_players[i_p].first);
			if (player == t_players[i_p].first){
				if (temp_count > 0 && player == this->nextPlayer())
					return victoryScore();
				incomplete_patterns_count += temp_count;
			}
			else{
				if (temp_count > 0 && player != this->nextPlayer())
					return -victoryScore();
				incomplete_patterns_count -= temp_count;
			}
		}
		if (incomplete_patterns_count > 1)
			return victoryScore()/2;
		if (incomplete_patterns_count < -1)
			return -victoryScore()/2;
		return 0;
	}
	return -victoryScore();
}
int TicTacToe::victoryScore(){
	return TICTACTOE_MAX_SCORE;
}

void TicTacToe::start(){
	t_last_moves.clear();
	Coordinates coordinates(((size_t)TICTACTOE_DIMENSION));
	for (coordinates[0] = 0; coordinates[0] < TICTACTOE_WIDTH; coordinates[0]++){
		for (coordinates[1] = 0; coordinates[1] < TICTACTOE_WIDTH; coordinates[1]++){
			t_board->getSquare(coordinates)->delAttribute(TICTACTOE_CROSS);
			t_board->getSquare(coordinates)->delAttribute(TICTACTOE_ROUND);
			t_board->getSquare(coordinates)->addAttribute(TICTACTOE_EMPTY);
		}
	}
}
unsigned int TicTacToe::dimension(){
	return TICTACTOE_DIMENSION;
}
bool TicTacToe::isPlayable(Coordinates coordinates){
	if (isEnded()) return false;

	bool is_in_board = true;
	for (unsigned int dim = 0; dim < this->dimension(); dim++){
		is_in_board = (coordinates[dim] >= 0 && coordinates[dim] < TICTACTOE_WIDTH);
		if (!is_in_board)
			return false;
	}
	return (t_board->getSquare(coordinates)->isAttribute(TICTACTOE_EMPTY));
}
vector<Coordinates> TicTacToe::playableCoordinates(){
	vector<Coordinates> result;
	if (isEnded()) return result;

	for (unsigned int line = 0; line < TICTACTOE_WIDTH; line++){
		for (unsigned int column = 0; column < TICTACTOE_WIDTH; column++){
			Coordinates coordinates = Coordinates(TICTACTOE_DIMENSION);
			coordinates[0] = line; coordinates[1] = column;
			if (t_board->getSquare(coordinates)->isAttribute(TICTACTOE_EMPTY))
				result.push_back(coordinates);
		}
	}
	return result;
}

Player * TicTacToe::nextPlayer(){
	return t_next_player;
}
void TicTacToe::play(Coordinates coordinates){
	if (isPlayable(coordinates)){
		t_last_moves.push_back(coordinates);
		TicTacToe_Attributes p_player;
		if (t_next_player == t_players[0].first){
			p_player = t_players[0].second;
			t_next_player = t_players[1].first;
		}
		else{
			p_player = t_players[1].second;
			t_next_player = t_players[0].first;
		}
		t_board->getSquare(coordinates)->delAttribute(TICTACTOE_EMPTY);
		t_board->getSquare(coordinates)->addAttribute(p_player);
	}
}
vector<Coordinates> TicTacToe::lastMoves(){
	return this->t_last_moves;
}

void TicTacToe::display(std::ostream & out){
	out<<" ";
	for(int column=0; column<TICTACTOE_WIDTH; column++)
		out<<"|"<<column;
	out<<"|"<<std::endl;
	Coordinates coordinates(TICTACTOE_DIMENSION);
	for(coordinates[0]=0; coordinates[0]<TICTACTOE_WIDTH; coordinates[0]++){
		out<<coordinates[0];
		for(coordinates[1]=0; coordinates[1]<TICTACTOE_WIDTH; coordinates[1]++){
			out<<"|";
			if (t_board->getSquare(coordinates)->isAttribute(TICTACTOE_CROSS))
				out<<'X';
			else if (t_board->getSquare(coordinates)->isAttribute(TICTACTOE_ROUND))
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

/*****************************************************Patterns****************************************************************/

vector<VarPattern> TicTacToe_Empty::getVariables(){
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
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_Empty::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
	Square<TicTacToe_Attributes> marked_square;
	marked_square.addAttribute(TICTACTOE_EMPTY);

	int line = variable_values[0];
	int column = variable_values[1];
	Coordinates coordinates(TICTACTOE_DIMENSION);
	coordinates[0] = line; coordinates[1] = column;
	result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));

	return result;
}

/*****************************************************Victory Patterns****************************************************************/

TicTacToe_Line::TicTacToe_Line(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_Line::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = TICTACTOE_WIDTH;
	line.v_step = 1;
	result.push_back(line);

	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_Line::getSquares(vector<int> variable_values){
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

TicTacToe_Column::TicTacToe_Column(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_Column::getVariables(){
	vector<VarPattern> result;

	VarPattern column;
	column.v_begin = 0;
	column.v_end = TICTACTOE_WIDTH;
	column.v_step = 1;
	result.push_back(column);

	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_Column::getSquares(vector<int> variable_values){
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

TicTacToe_FirstDiag::TicTacToe_FirstDiag(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_FirstDiag::getVariables(){
	vector<VarPattern> result;
	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_FirstDiag::getSquares(vector<int> variable_values){
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

TicTacToe_SecondDiag::TicTacToe_SecondDiag(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_SecondDiag::getVariables(){
	vector<VarPattern> result;
	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_SecondDiag::getSquares(vector<int> variable_values){
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

/*****************************************************Incomplete Victory Patterns****************************************************************/

TicTacToe_IncompleteLine::TicTacToe_IncompleteLine(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_IncompleteLine::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = TICTACTOE_WIDTH;
	line.v_step = 1;
	result.push_back(line);

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = 3;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_IncompleteLine::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
	Square<TicTacToe_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<TicTacToe_Attributes> empty_square;
	empty_square.addAttribute(TICTACTOE_EMPTY);

	int line = variable_values[0];
	int empty_square_index = variable_values[1];
	for (unsigned int column = 0; column<TICTACTOE_WIDTH; column++){
		Coordinates coordinates(TICTACTOE_DIMENSION);
		coordinates[0] = line; coordinates[1] = column;
		if (column != (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		else
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, empty_square));
	}

	return result;
}

TicTacToe_IncompleteColumn::TicTacToe_IncompleteColumn(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_IncompleteColumn::getVariables(){
	vector<VarPattern> result;

	VarPattern column;
	column.v_begin = 0;
	column.v_end = TICTACTOE_WIDTH;
	column.v_step = 1;
	result.push_back(column);

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = 3;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_IncompleteColumn::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
	Square<TicTacToe_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<TicTacToe_Attributes> empty_square;
	empty_square.addAttribute(TICTACTOE_EMPTY);

	int column = variable_values[0];
	int empty_square_index = variable_values[1];
	for (unsigned int line = 0; line<TICTACTOE_WIDTH; line++){
		Coordinates coordinates(TICTACTOE_DIMENSION);
		coordinates[0] = line; coordinates[1] = column;
		result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		if (line != (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		else
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, empty_square));
	}

	return result;
}

TicTacToe_IncompleteFirstDiag::TicTacToe_IncompleteFirstDiag(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_IncompleteFirstDiag::getVariables(){
	vector<VarPattern> result;

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = 3;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_IncompleteFirstDiag::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
	Square<TicTacToe_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<TicTacToe_Attributes> empty_square;
	empty_square.addAttribute(TICTACTOE_EMPTY);

	int empty_square_index = variable_values[0];
	for (unsigned int d = 0; d<TICTACTOE_WIDTH; d++){
		Coordinates coordinates(TICTACTOE_DIMENSION);
		coordinates[0] = d; coordinates[1] = d;
		if (d != (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		else
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, empty_square));
	}

	return result;
}

TicTacToe_IncompleteSecondDiag::TicTacToe_IncompleteSecondDiag(TicTacToe_Attributes player){
	p_player = player;
}
vector<VarPattern> TicTacToe_IncompleteSecondDiag::getVariables(){
	vector<VarPattern> result;

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = 3;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<TicTacToe_Attributes> > > TicTacToe_IncompleteSecondDiag::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<TicTacToe_Attributes> > > result;
	Square<TicTacToe_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<TicTacToe_Attributes> empty_square;
	empty_square.addAttribute(TICTACTOE_EMPTY);

	int empty_square_index = variable_values[0];
	for (unsigned int d = 0; d<TICTACTOE_WIDTH; d++){
		Coordinates coordinates(TICTACTOE_DIMENSION);
		coordinates[0] = (TICTACTOE_WIDTH - 1)-d; coordinates[1] = d;
		if (d != (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, marked_square));
		else
			result.push_back(pair<Coordinates, Square<TicTacToe_Attributes> >(coordinates, empty_square));
	}

	return result;
}


