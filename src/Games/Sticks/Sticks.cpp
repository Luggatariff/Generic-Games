/*!
 * \file Sticks.cpp
 * \brief Sticks code
 * \author Anaël Petit
 */

#include <iomanip>
#include <Sticks/Sticks.hpp>

Sticks::Sticks(Player * player_one, Player * player_two, unsigned int sticks_number){
	Coordinates board_size(1);
	t_max_sticks_number = sticks_number;
	board_size[0] = t_max_sticks_number;
	this->t_board = new Board<Sticks_Attributes>(board_size);
	this->t_sticks_remaining = t_max_sticks_number;

	this->t_players.push_back(player_one);
	this->t_players.push_back(player_two);
	this->t_next_player = player_one;
}

Sticks::~Sticks(){
	delete t_board;
}

Game * Sticks::copy(){
	Sticks * result = new Sticks(this->t_players[0], this->t_players[1], this->t_max_sticks_number);
	delete result->t_board;
	result->t_sticks_remaining = this->t_sticks_remaining;
	result->t_board = this->t_board->copy();
	result->t_next_player = this->t_next_player;
	return (Game *)result;
}

vector<Player *> Sticks::players(){
	return t_players;
}

bool Sticks::isEnded(){
	return (t_sticks_remaining == 0) || (t_sticks_remaining == 1);
}
bool Sticks::isWon(){
	return isEnded();
}
Player * Sticks::whoWon(){
	if (isWinner(t_players[0])) return t_players[0];
	if (isWinner(t_players[1])) return t_players[1];
	return NULL;
}
bool Sticks::isWinner(Player * player){
	if (isEnded()){
		if ((t_sticks_remaining == 0 && t_next_player == player) || (t_sticks_remaining == 1 && t_next_player != player)) return true;
		else return false;
	}
	return false;
}

int Sticks::score(Player * player){
	Player * winner = whoWon();
	if (winner == player)
		return victoryScore();
	if (winner == NULL)
		return 0;
	return -victoryScore();
}
int Sticks::victoryScore(){
	return STICKS_MAX_SCORE;
}

void Sticks::start(){
	t_last_moves.clear();
	Coordinates coordinates(1);
	for (coordinates[0] = 0; coordinates[0] < t_max_sticks_number; coordinates[0]++){
		t_board->getSquare(coordinates)->delAttribute(STICKS_NO_STICK);
		t_board->getSquare(coordinates)->addAttribute(STICKS_STICK);
	}
	t_sticks_remaining = t_max_sticks_number;
}
unsigned int Sticks::dimension(){
	return 2;
}

bool Sticks::isPlayable(Coordinates coordinates){
	if (isEnded()) return false;

	if (coordinates[0] < 0 || (coordinates[0] + (coordinates[1] -1)) >= t_max_sticks_number  || coordinates[1] <= 0 || coordinates[2] > STICKS_MAX_AT_ONCE)
		return false;

	Coordinates stick(1);
	bool playable = true;
	for (stick[0] = coordinates[0]; stick[0] < coordinates[0] + coordinates[1]; stick[0]++){
		if (t_board->getSquare(stick)->isAttribute(STICKS_NO_STICK)){
			playable = false;
			break;
		}
	}

	return playable;
}
vector<Coordinates> Sticks::playableCoordinates(){
	vector<Coordinates> result;
	if (isEnded()) return result;

	Coordinates coordinates(2);
	for (coordinates[0] = 0; coordinates[0] < t_max_sticks_number; coordinates[0]++){
		for (coordinates[1] = 1; coordinates[1] <= STICKS_MAX_AT_ONCE; coordinates[1]++){
			if ((coordinates[0] + (coordinates[1] -1)) >= t_max_sticks_number) break;

			Coordinates stick(1);
			stick[0] = coordinates[0] + coordinates[1] - 1;
			if (t_board->getSquare(stick)->isAttribute(STICKS_STICK)){
				result.push_back(coordinates);
			}
			else
				break;
		}
	}

	return result;
}

Player * Sticks::nextPlayer(){
	return t_next_player;
}
void Sticks::play(Coordinates coordinates){
	if (isPlayable(coordinates)){
		t_last_moves.push_back(coordinates);
		if (t_next_player == t_players[0]){
			t_next_player = t_players[1];
		}
		else{
			t_next_player = t_players[0];
		}

		Coordinates stick(1);
		for (stick[0] = coordinates[0]; stick[0] < coordinates[0] + coordinates[1]; stick[0]++){
			t_board->getSquare(stick)->delAttribute(STICKS_STICK);
			t_board->getSquare(stick)->addAttribute(STICKS_NO_STICK);
			t_sticks_remaining--;
		}

	}
}
vector<Coordinates> Sticks::lastMoves(){
	return this->t_last_moves;
}

void Sticks::display(std::ostream & out){
	Coordinates stick(1);
	for (stick[0] = 0; stick[0] < t_max_sticks_number; stick[0]++){
		cout<<' ';
		if(t_board->getSquare(stick)->isAttribute(STICKS_STICK))
			cout<<'|';
		else
			cout<<' ';
		cout<<' ';
	}
	cout<<endl;
	for (stick[0] = 0; stick[0] < t_max_sticks_number; stick[0]++){
		cout<<setw(2);
		cout<<stick[0];
		cout<<' ';
	}
	out<<std::endl;
	out<<"Remaining Sticks : "<<t_sticks_remaining<<std::endl;
}
