/*!
 * \file Human.cpp
 * \brief Human code
 * \author Anaël Petit
 */

#include "Human.hpp"
#include <algorithm>

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

Coordinates Human::play(Game * game, vector<Coordinates> limit_choices){
	unsigned int dimension = game->dimension();
	Coordinates result(((size_t)dimension));
	do{
		if (limit_choices.size() > 1){
			cout<<"Choose one of these Coordinates: ";
			for (vector<Coordinates>::iterator limit_choices_it = limit_choices.begin(); limit_choices_it != limit_choices.end(); limit_choices_it++){
				cout<<"("<<limit_choices_it->operator [](0);
				for (unsigned int dim = 1; dim < limit_choices_it->dimension(); dim++)
					cout<<" "<<limit_choices_it->operator [](dim);
				cout<<")";
			}
			cout<<endl;
		}
		cout<<"Choose a Square ("<<dimension<<" coordinates(s)):";
		for (unsigned int dim=0; dim<dimension; dim++)
			cin>>result[dim];
	}
	while(!game->isPlayable(result) || (limit_choices.size() > 1 && find(limit_choices.begin(), limit_choices.end(), result) == limit_choices.end()));

	return result;
}

string Human::getName(){
	return hu_name;
}

unsigned int Human::getTeam(){
	return hu_team;
}
