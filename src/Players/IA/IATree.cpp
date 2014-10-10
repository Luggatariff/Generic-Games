/*!
 * \file IATree.cpp
 * \brief IATree code
 * \author Anaël Petit
 */

#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "IATree.hpp"

IATree::IATree(Game * game, Player * player){
	this->it_computed = false;
	this->it_game = game;
	this->it_next_player = game->nextPlayer();
	this->it_player = player;
	this->it_score = NULL;
	this->it_victory_score = game->victoryScore();
}

IATree::~IATree(){
	if (it_score != NULL && it_sons.empty())
		delete it_score;
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

Score * IATree::compute(){
	if (this->it_computed){
		return this->it_score;
	}
	Score * result;

	if (it_sons.empty() && it_game != NULL){
		result = new Score(it_game->score(it_player));
	}
	else if (!it_sons.empty()){
		bool return_maximum = (it_next_player == it_player);
		map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin();
		result = sons_iterator->second->compute();
		result->incDepth();
		this->it_bestsons.push_back(sons_iterator->first);
		for(sons_iterator++; sons_iterator != it_sons.end(); sons_iterator++){
			Score * res_score = sons_iterator->second->compute();
			if (return_maximum){
				if (res_score->value() >= result->value()){
					if ((res_score->value() > result->value()) || (res_score->value() > 0 && res_score->depth() < result->depth()) || (res_score->value() < 0 && res_score->depth() > result->depth())){
						result = res_score;
						result->incDepth();

						this->it_bestsons.clear();
						this->it_bestsons.push_back(sons_iterator->first);
					}
					else if ((res_score->value() == result->value())&& ((res_score->depth() == result->depth()) || res_score->value() == 0)){
						this->it_bestsons.push_back(sons_iterator->first);
					}
				}
			}
			else{
				if ((res_score->value() <= result->value()) && ((res_score->value() < result->value()) || (res_score->value() >= 0 && res_score->depth() > result->depth()) || (res_score->value() < 0 && res_score->depth() < result->depth()))){
					result = res_score;
					result->incDepth();

					if (result->value() == (-1) * it_victory_score && result->depth() == 0)
						break;
				}
			}
		}
	}
	this->it_computed = true;
	if (this->it_score != NULL && this->it_sons.empty())
		delete this->it_score;
	this->it_score = result;
	return result;
}

Coordinates IATree::bestSon(){
	if (this->it_bestsons.size() == 1) return this->it_bestsons[0];
	srand ( time(NULL) );
	unsigned int r;
	r =  rand() % (this->it_bestsons.size()-1);
	return this->it_bestsons[r];
}

IATree * IATree::changeRoot(Coordinates coordinates){
	IATree * result = this->it_sons[coordinates];
	this->it_sons.erase(coordinates);

	return result;
}

Score::Score(int value){
	s_value = value;
	s_depth = 0;
}

void Score::incDepth(){
	s_depth++;
}

int Score::value(){
	return s_value;
}

unsigned int Score::depth(){
	return s_depth;
}

void IATree::stackByLevel(vector<vector<pair<Coordinates,IATree *> > > &result, unsigned int level){
	if (result.size() < level + 1)
		result.push_back(vector<pair<Coordinates,IATree *> >());
	for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		result[level].push_back(pair<Coordinates, IATree*>(sons_iterator->first, sons_iterator->second));
		sons_iterator->second->stackByLevel(result, level+1);
	}
}

void IATree::display(){
	vector<vector<pair<Coordinates,IATree *> > > stacked_up_tree;
	stackByLevel(stacked_up_tree);
	unsigned int score_width = 5;
	char separator_between_nodes = '|';
	char separator_in_nodes = ',';
	unsigned int node_width = 6;

	unsigned int max_level_node_number = 0;
	for (unsigned int i_s = 0; i_s< stacked_up_tree.size(); i_s++){
		if (stacked_up_tree[i_s].size() > max_level_node_number)
			max_level_node_number = stacked_up_tree[i_s].size();

	}
	unsigned int max_level_width = max_level_node_number * node_width;
	for (unsigned int i_s = 0; i_s< stacked_up_tree.size(); i_s++){
		unsigned int spaces_number = (max_level_width - node_width * stacked_up_tree[i_s].size()) / (stacked_up_tree[i_s].size() + 1);
		for (unsigned int i_ss = 0; i_ss < stacked_up_tree[i_s].size(); i_ss++){
			pair<Coordinates,IATree *> node = stacked_up_tree[i_s][i_ss];
			for (unsigned int i_spaces = 0; i_spaces < spaces_number; i_spaces++)
				cout<<" ";
			cout<<node.first[0];
			cout<<separator_in_nodes;
			cout<<node.first[1];
			cout<<separator_in_nodes;
			cout<<node.second->it_score->depth();
			cout<<separator_between_nodes;
		}
		cout<<endl;
		for (unsigned int i_ss = 0; i_ss < stacked_up_tree[i_s].size(); i_ss++){
			pair<Coordinates,IATree *> node = stacked_up_tree[i_s][i_ss];
			for (unsigned int i_spaces = 0; i_spaces < spaces_number; i_spaces++)
				cout<<" ";
			cout<<setw(score_width);
			cout<<node.second->it_score->value();
			cout<<separator_between_nodes;
		}
		cout<<endl;
	}
}
