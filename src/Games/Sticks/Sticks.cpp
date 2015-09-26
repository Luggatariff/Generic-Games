/*!
 * \file Sticks.cpp
 * \brief Sticks code
 * \author Anaël Petit
 */

#include <iomanip>
#include <sstream>
#include "Sticks.hpp"

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
int Sticks::whoWon(){
	if (isWinner(t_players[0]->getTeam())) return t_players[0]->getTeam();
	if (isWinner(t_players[1]->getTeam())) return t_players[1]->getTeam();
	return -1;
}
bool Sticks::isWinner(unsigned int team_id){
	if (isEnded()){
		if ((t_sticks_remaining == 0 && t_next_player->getTeam() == team_id) || (t_sticks_remaining == 1 && t_next_player->getTeam() != team_id)) return true;
		else return false;
	}
	return false;
}

int Sticks::score(unsigned int team_id){
	int winner = whoWon();
	if (winner == (int)team_id)
		return victoryScore();
	if (winner == -1)
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

    if ((coordinates[0] + (coordinates[1] -1)) >= t_max_sticks_number  || coordinates[1] <= 0 || coordinates[2] > STICKS_MAX_AT_ONCE)
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
	cout<<'\r';
	for (stick[0] = 0; stick[0] < t_max_sticks_number; stick[0]++){
		cout<<setw(2);
		cout<<stick[0];
		cout<<' ';
	}
	out<<'\r';
	out<<"Remaining Sticks : "<<t_sticks_remaining<<std::endl;
}

string Sticks::getName(){
	ostringstream name_stream;
	name_stream << "Sticks" << t_max_sticks_number;
	return name_stream.str();
}
