/*!
 * \file Human.cpp
 * \brief Human code
 * \author Ana�l Petit
 */

#include "Human.hpp"

Human::Human(string name, unsigned int team_id){
	hu_name=name;
	hu_team=team_id;
}

void Human::start(Game * game){
	cout<<"Starting new Game..."<<endl;
}

void Human::end(Game * game){
	if (game->whoWon() == (int)this->getTeam()){
		cout<<"You won the Game"<<endl;
	}
}

Coordinates Human::play(Game * game){
	unsigned int dimension = game->dimension();
	Coordinates result(((size_t)dimension));
	do{
		cout<<"Choose a Square ("<<dimension<<" coordinates(s)):";
		for (unsigned int dim=0; dim<dimension; dim++)
			cin>>result[dim];
	}
	while(!game->isPlayable(result));

	return result;
}

string Human::getName(){
	return hu_name;
}

unsigned int Human::getTeam(){
	return hu_team;
}
