/*!
 * \file FourInALine.cpp
 * \brief FourInALine code
 * \author Anaël Petit
 */

#include <FourInALine/FourInALine.hpp>

FourInALine::FourInALine(Player * player_one, Player * player_two){
	Coordinates board_size(2);
	board_size[0] = FOURINALINE_HEIGHT;
	board_size[1] = FOURINALINE_WIDTH;
	this->t_board = new Board<FourInALine_Attributes>(board_size);

	this->t_players.push_back(pair<Player *, FourInALine_Attributes>(player_one, FOURINALINE_CROSS));
	this->t_players.push_back(pair<Player *, FourInALine_Attributes>(player_two, FOURINALINE_ROUND));
	this->t_next_player = player_one;
}

FourInALine::~FourInALine(){
	delete t_board;
}

Game * FourInALine::copy(){
	FourInALine * result = new FourInALine(this->t_players[0].first, this->t_players[1].first);
	delete result->t_board;
	result->t_board = this->t_board->copy();
	result->t_next_player = this->t_next_player;
	return (Game *)result;
}

vector<Player *> FourInALine::players(){
	vector<Player *> result;
	for (unsigned int i_p = 0; i_p < t_players.size(); i_p++)
		result.push_back(t_players[i_p].first);
	return result;
}

bool FourInALine::isEnded(){
	if (isWon()) return true;

	FourInALine_Empty * pattern_empty = new FourInALine_Empty();
	bool result = (t_board->checkPattern(pattern_empty)==0);
	delete pattern_empty;
	return result;
}
bool FourInALine::isWon(){
	return (whoWon() != NULL);
}
Player * FourInALine::whoWon(){
	if (isWinner(t_players[0].first)) return t_players[0].first;
	if (isWinner(t_players[1].first)) return t_players[1].first;
	return NULL;
}
bool FourInALine::isWinner(Player * player){
	bool result = false;

	FourInALine_Attributes p_player;
	if (player == t_players[0].first)
		p_player = t_players[0].second;
	else
		p_player = t_players[1].second;

	FourInALine_Line * pattern_line = new FourInALine_Line(p_player);
	result = (t_board->checkPattern(pattern_line) > 0);
	delete pattern_line;

	if (!result){
		FourInALine_Column * pattern_column = new FourInALine_Column(p_player);
		result = (t_board->checkPattern(pattern_column) > 0);
		delete pattern_column;
	}
	if (!result){
		FourInALine_FirstDiag * pattern_firstdiag = new FourInALine_FirstDiag(p_player);
		result = (t_board->checkPattern(pattern_firstdiag) > 0);
		delete pattern_firstdiag;
	}
	if (!result){
		FourInALine_SecondDiag * pattern_seconddiag = new FourInALine_SecondDiag(p_player);
		result = (t_board->checkPattern(pattern_seconddiag) > 0);
		delete pattern_seconddiag;
	}
	return result;
}
unsigned int FourInALine::countIncompleteVictoryPatterns(Player * player){
	unsigned int result = 0;

	FourInALine_Attributes p_player;
	if (player == t_players[0].first)
		p_player = t_players[0].second;
	else
		p_player = t_players[1].second;

	FourInALine_IncompleteLine * pattern_line = new FourInALine_IncompleteLine(p_player);
	result += t_board->checkPattern(pattern_line);
	delete pattern_line;
	FourInALine_IncompleteFirstDiag * pattern_firstdiag = new FourInALine_IncompleteFirstDiag(p_player);
	result += t_board->checkPattern(pattern_firstdiag);
	delete pattern_firstdiag;
	FourInALine_IncompleteSecondDiag * pattern_seconddiag = new FourInALine_IncompleteSecondDiag(p_player);
	result += t_board->checkPattern(pattern_seconddiag);
	delete pattern_seconddiag;

	return result;
}

int FourInALine::score(Player * player){
	Player * winner = whoWon();
	if (winner == player){
		return victoryScore();
	}
	if (winner == NULL){
		int incomplete_patterns_count = 0;
		for (unsigned int i_p = 0; i_p < t_players.size(); i_p++){
			unsigned int temp_count = countIncompleteVictoryPatterns(t_players[i_p].first);
			if (player == t_players[i_p].first){
				incomplete_patterns_count += temp_count;
			}
			else{
				incomplete_patterns_count -= temp_count;
			}
		}
		return incomplete_patterns_count;
	}
	return -victoryScore();
}
int FourInALine::victoryScore(){
	return FOURINALINE_MAX_SCORE;
}

void FourInALine::start(){
	t_last_moves.clear();
	Coordinates coordinates(2);
	coordinates[0] = 0;
	for (coordinates[1] = 0; coordinates[1] < FOURINALINE_WIDTH; coordinates[1]++){
		t_board->getSquare(coordinates)->delAttribute(FOURINALINE_CROSS);
		t_board->getSquare(coordinates)->delAttribute(FOURINALINE_ROUND);
		t_board->getSquare(coordinates)->delAttribute(FOURINALINE_EMPTY);
		t_board->getSquare(coordinates)->addAttribute(FOURINALINE_PLAYABLE);
	}
	for (coordinates[0] = 1; coordinates[0] < FOURINALINE_HEIGHT; coordinates[0]++){
		for (coordinates[1] = 0; coordinates[1] < FOURINALINE_WIDTH; coordinates[1]++){
			t_board->getSquare(coordinates)->delAttribute(FOURINALINE_CROSS);
			t_board->getSquare(coordinates)->delAttribute(FOURINALINE_ROUND);
			t_board->getSquare(coordinates)->delAttribute(FOURINALINE_PLAYABLE);
			t_board->getSquare(coordinates)->addAttribute(FOURINALINE_EMPTY);
		}
	}
}
unsigned int FourInALine::dimension(){
	return 1;
}

bool FourInALine::isPlayable(Coordinates coordinates){
	if (isEnded()) return false;

	if (coordinates[0] < 0 && coordinates[0] >= FOURINALINE_WIDTH)
		return false;

	Coordinates highest_square(2);
	highest_square[1] = coordinates[0];
	highest_square[0] = FOURINALINE_HEIGHT - 1;
	return (t_board->getSquare(highest_square)->isAttribute(FOURINALINE_PLAYABLE) || t_board->getSquare(highest_square)->isAttribute(FOURINALINE_EMPTY));
}
vector<Coordinates> FourInALine::playableCoordinates(){
	vector<Coordinates> result;
	if (isEnded()) return result;

	for (unsigned int column = 0; column < FOURINALINE_WIDTH; column++){
		Coordinates coordinates = Coordinates(1);
		coordinates[0] = column;

		Coordinates highest_square(2);
		highest_square[1] = coordinates[0];
		highest_square[0] = FOURINALINE_HEIGHT - 1;

		if (t_board->getSquare(highest_square)->isAttribute(FOURINALINE_PLAYABLE) || t_board->getSquare(highest_square)->isAttribute(FOURINALINE_EMPTY))
			result.push_back(coordinates);
	}

	return result;
}

Player * FourInALine::nextPlayer(){
	return t_next_player;
}
void FourInALine::play(Coordinates coordinates){
	if (isPlayable(coordinates)){
		t_last_moves.push_back(coordinates);
		FourInALine_Attributes p_player;
		if (t_next_player == t_players[0].first){
			p_player = t_players[0].second;
			t_next_player = t_players[1].first;
		}
		else{
			p_player = t_players[1].second;
			t_next_player = t_players[0].first;
		}
		Coordinates highest_free_square(2);
		highest_free_square[1] = coordinates[0];
		for (highest_free_square[0] = 0; highest_free_square[0] < FOURINALINE_HEIGHT; highest_free_square[0]++){
			if(t_board->getSquare(highest_free_square)->isAttribute(FOURINALINE_PLAYABLE))
				break;
		}
		t_board->getSquare(highest_free_square)->delAttribute(FOURINALINE_PLAYABLE);
		t_board->getSquare(highest_free_square)->addAttribute(p_player);
		highest_free_square[0]++;
		if (highest_free_square[0] < FOURINALINE_HEIGHT){
			t_board->getSquare(highest_free_square)->delAttribute(FOURINALINE_EMPTY);
			t_board->getSquare(highest_free_square)->addAttribute(FOURINALINE_PLAYABLE);
		}
	}
}
vector<Coordinates> FourInALine::lastMoves(){
	return this->t_last_moves;
}

void FourInALine::display(std::ostream & out){
	out<<" ";
	for(int column=0; column<FOURINALINE_WIDTH; column++)
		out<<"|"<<column;
	out<<"|"<<std::endl;
	Coordinates coordinates(2);
	for(int line = FOURINALINE_HEIGHT - 1; line>=0; line--){
		out<<" ";
		for(coordinates[1]=0; coordinates[1]<FOURINALINE_WIDTH; coordinates[1]++){
			coordinates[0] = (unsigned int) line;

			out<<"|";
			if (t_board->getSquare(coordinates)->isAttribute(FOURINALINE_CROSS))
				out<<'X';
			else if (t_board->getSquare(coordinates)->isAttribute(FOURINALINE_ROUND))
				out<<'O';
			else
				out<<" ";
		}
		out<<"|"<<std::endl;
	}
	out<<" ";
	for(int column=0; column<FOURINALINE_WIDTH; column++)
		out<<"|-";
	out<<"|"<<std::endl;
}

string FourInALine::getName(){
	return string("FourInALine");
}

/*****************************************************Patterns****************************************************************/

vector<VarPattern> FourInALine_Empty::getVariables(){
	vector<VarPattern> result;

	VarPattern column;
	column.v_begin = 0;
	column.v_end = FOURINALINE_WIDTH;
	column.v_step = 1;
	result.push_back(column);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_Empty::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(FOURINALINE_EMPTY);

	int column = variable_values[0];
	Coordinates coordinates(2);
	coordinates[0] = FOURINALINE_HEIGHT-1; coordinates[1] = column;
	result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));

	return result;
}

/*****************************************************Victory Patterns****************************************************************/

FourInALine_Line::FourInALine_Line(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_Line::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT;
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = 0;
	column.v_end = FOURINALINE_WIDTH - (FOURINALINE_LINE - 1);
	column.v_step = 1;
	result.push_back(column);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_Line::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);

	for (unsigned int d = 0; d < FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0]; coordinates[1] = variable_values[1] + d;
		result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}

FourInALine_Column::FourInALine_Column(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_Column::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT - (FOURINALINE_LINE - 1);
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = 0;
	column.v_end = FOURINALINE_WIDTH;
	column.v_step = 1;
	result.push_back(column);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_Column::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);

	for (unsigned int d = 0; d < FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0] + d; coordinates[1] = variable_values[1];
		result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}

FourInALine_FirstDiag::FourInALine_FirstDiag(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_FirstDiag::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT - (FOURINALINE_LINE - 1);
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = 0;
	column.v_end = FOURINALINE_WIDTH - (FOURINALINE_LINE - 1);
	column.v_step = 1;
	result.push_back(column);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_FirstDiag::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);

	for (unsigned int d = 0; d<FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0] + d; coordinates[1] = variable_values[1] + d;
		result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}

FourInALine_SecondDiag::FourInALine_SecondDiag(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_SecondDiag::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT - (FOURINALINE_LINE - 1);
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = FOURINALINE_LINE - 1;
	column.v_end = FOURINALINE_WIDTH;
	column.v_step = 1;
	result.push_back(column);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_SecondDiag::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);

	for (unsigned int d = 0; d<FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0] + d; coordinates[1] = variable_values[1] - d;
		result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}

/*****************************************************Incomplete Victory Patterns****************************************************************/

FourInALine_IncompleteLine::FourInALine_IncompleteLine(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_IncompleteLine::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT;
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = 0;
	column.v_end = FOURINALINE_WIDTH - (FOURINALINE_LINE - 1);
	column.v_step = 1;
	result.push_back(column);

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = FOURINALINE_LINE;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_IncompleteLine::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<FourInALine_Attributes> empty_square;
	empty_square.addAttribute(FOURINALINE_EMPTY);

	int empty_square_index = variable_values[2];
	for (unsigned int d = 0; d < FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0]; coordinates[1] = variable_values[1] + d;
		if (d == (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, empty_square));
		else
			result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}

FourInALine_IncompleteFirstDiag::FourInALine_IncompleteFirstDiag(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_IncompleteFirstDiag::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT - (FOURINALINE_LINE - 1);
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = 0;
	column.v_end = FOURINALINE_WIDTH - (FOURINALINE_LINE - 1);
	column.v_step = 1;
	result.push_back(column);

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = FOURINALINE_LINE;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_IncompleteFirstDiag::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<FourInALine_Attributes> empty_square;
	empty_square.addAttribute(FOURINALINE_EMPTY);

	int empty_square_index = variable_values[2];
	for (unsigned int d = 0; d<FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0] + d; coordinates[1] = variable_values[1] + d;
		if (d == (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, empty_square));
		else
			result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}

FourInALine_IncompleteSecondDiag::FourInALine_IncompleteSecondDiag(FourInALine_Attributes player){
	p_player = player;
}
vector<VarPattern> FourInALine_IncompleteSecondDiag::getVariables(){
	vector<VarPattern> result;

	VarPattern line;
	line.v_begin = 0;
	line.v_end = FOURINALINE_HEIGHT - (FOURINALINE_LINE - 1);
	line.v_step = 1;
	result.push_back(line);

	VarPattern column;
	column.v_begin = FOURINALINE_LINE - 1;
	column.v_end = FOURINALINE_WIDTH;
	column.v_step = 1;
	result.push_back(column);

	VarPattern empty_square;
	empty_square.v_begin = 0;
	empty_square.v_end = FOURINALINE_LINE;
	empty_square.v_step = 1;
	result.push_back(empty_square);

	return result;
}
vector<pair<Coordinates, Square<FourInALine_Attributes> > > FourInALine_IncompleteSecondDiag::getSquares(vector<int> variable_values){
	vector<pair<Coordinates, Square<FourInALine_Attributes> > > result;
	Square<FourInALine_Attributes> marked_square;
	marked_square.addAttribute(p_player);
	Square<FourInALine_Attributes> empty_square;
	empty_square.addAttribute(FOURINALINE_EMPTY);

	int empty_square_index = variable_values[2];
	for (unsigned int d = 0; d<FOURINALINE_LINE; d++){
		Coordinates coordinates(2);
		coordinates[0] = variable_values[0] + d; coordinates[1] = variable_values[1] - d;
		if (d == (unsigned int)empty_square_index)
			result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, empty_square));
		else
			result.push_back(pair<Coordinates, Square<FourInALine_Attributes> >(coordinates, marked_square));
	}

	return result;
}


