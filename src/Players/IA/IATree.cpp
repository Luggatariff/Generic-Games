/*!
 * \file IATree.cpp
 * \brief IATree code
 * \author Anaël Petit
 */

#include "IATree.hpp"

IATree::IATree(Game * game, Player * player){
	this->it_computed = false;
	this->it_game = game;
	this->it_victory_score = game->victoryScore();
	this->it_next_player = game->nextPlayer();
	this->it_player = player;
	this->it_score = 0;
}

IATree::~IATree(){
	if (it_game != NULL)
		delete it_game;
	for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		delete sons_iterator->second;
	}
}

void IATree::populate(unsigned int level){
	this->it_computed = false;
	if (level == 0) return;

	if (it_game != NULL){
		vector<Coordinates> playable_moves = it_game->playableCoordinates();
		if (!playable_moves.empty()){
			for (unsigned int i_pm = 0; i_pm < playable_moves.size(); i_pm++){
				Game * son_game = it_game->copy();
				son_game->play(playable_moves[i_pm]);

				it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new IATree(son_game, it_player)));
				it_sons[playable_moves[i_pm]]->populate(level - 1);
			}
			delete it_game;
			it_game = NULL;
		}
	}
	else{
		for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
			sons_iterator->second->populate(level - 1);
		}
	}
}

int IATree::compute(){
	if (this->it_computed){
		return this->it_score;
	}
	int result = 0;

	if (it_sons.empty() && it_game != NULL){
		result = it_game->score(it_player);
	}
	else{
		bool return_maximum = (it_next_player == it_player);
		if (!return_maximum){
			result = it_victory_score + 1;
		}
		else{
			result = -1 * (it_victory_score + 1);
		}
		for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
			int res_score = sons_iterator->second->compute();
			if (return_maximum){
				if (res_score > result){
					result = res_score;
					this->it_bestson = sons_iterator->first;
					if (result >= it_victory_score)
						break;
				}
			}
			else{
				if (res_score < result){
					result = res_score;
					this->it_bestson = sons_iterator->first;
					if (result <= (-1) * it_victory_score)
						break;
				}
			}
		}
	}
	this->it_computed = true;
	return result;
}

Coordinates IATree::bestSon(){
	return this->it_bestson;
}

map<Coordinates, IATree *> IATree::sons(){
	return this->it_sons;
}
