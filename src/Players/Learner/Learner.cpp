/*!
 * \file Learner.cpp
 * \brief Learner code
 * \author Anaël Petit
 */

#include "Learner.hpp"

Learner::Learner(string name, Player * teacher){
	lea_name=name;
	lea_teacher=teacher;
}

void Learner::start(Game * game){
	lea_teacher->start(game);
}

Coordinates Learner::play(Game * game){
	return lea_teacher->play(game);
}

string Learner::getName(){
	return lea_name;
}
