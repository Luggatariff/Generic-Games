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
	this->it_game = game;
	this->it_next_player = game->nextPlayer();
	this->it_player = player;
	this->it_score = NULL;
	this->it_victory_score = game->victoryScore();
}

IATree::~IATree(){
	if (it_score != NULL)
		delete it_score;
	if (it_game != NULL)
		delete it_game;
	for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		delete sons_iterator->second;
	}
}

void IATree::populate(unsigned int level, bool fast_compute){
	private_populate(level, fast_compute, level);
}

void IATree::private_populate(unsigned int level, bool fast_compute, unsigned int highest_level){
	if (level == 0) return;

	if (it_game != NULL){
		bool populate_sons = true;

		if (fast_compute && (highest_level - level) >= it_game->players().size()){
			if (this->it_score != NULL){
				if (this->it_score->value() == 0){
					delete this->it_score;
					this->it_score = NULL;
				}
			}
			else{
				Score * score_peek = new Score(it_game->score(it_player));
				if (score_peek->value() != 0){
					this->it_score = score_peek;
				}
			}
			populate_sons = (this->it_score == NULL);
		}

		vector<Coordinates> playable_moves;
		if (populate_sons){
			playable_moves = it_game->playableCoordinates();
			populate_sons = !playable_moves.empty();
		}
		if (populate_sons){
			delete this->it_score;
			this->it_score = NULL;
			bool ending_son = false;
			for (unsigned int i_pm = 0; i_pm < playable_moves.size(); i_pm++){
				Game * son_game = it_game->copy();
				son_game->play(playable_moves[i_pm]);

				if (it_next_player == it_player && son_game->isWinner(it_next_player)){
					it_sons.clear();
					it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new IATree(son_game, it_player)));
					ending_son = true;
					break;
				}

				it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new IATree(son_game, it_player)));
			}
			if (!ending_son){
				for (map<Coordinates, IATree *>::iterator iter_sons = it_sons.begin(); iter_sons != it_sons.end(); iter_sons++)
					iter_sons->second->private_populate(level - 1, fast_compute, highest_level);
			}
			delete it_game;
			it_game = NULL;
		}
	}
	else{
		if (!it_sons.empty()){
			delete this->it_score;
			this->it_score = NULL;
			for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
				sons_iterator->second->private_populate(level - 1, fast_compute, highest_level);
			}
		}
	}
}

Score * IATree::compute(){
	if (this->it_score != NULL){
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
		this->it_bestsons.push_back(sons_iterator->first);
		for(sons_iterator++; sons_iterator != it_sons.end(); sons_iterator++){
			Score * res_score = sons_iterator->second->compute();
			if (return_maximum){
				if (res_score->value() >= result->value()){
					if ((res_score->value() > result->value()) || (res_score->value() > 0 && res_score->depth() < result->depth()) || (res_score->value() < 0 && res_score->depth() > result->depth())){
						result = res_score;

						this->it_bestsons.clear();
						this->it_bestsons.push_back(sons_iterator->first);
						if (result->value() == it_victory_score && result->depth() == 1)
							break;
					}
					else if ((res_score->value() == result->value())&& ((res_score->depth() == result->depth()) || res_score->value() == 0)){
						this->it_bestsons.push_back(sons_iterator->first);
					}
				}
			}
			else{
				if ((res_score->value() <= result->value()) && ((res_score->value() < result->value()) || (res_score->value() >= 0 && res_score->depth() > result->depth()) || (res_score->value() < 0 && res_score->depth() < result->depth()))){
					result = res_score;
					if (result->value() == (-1) * it_victory_score && result->depth() == 1)
						break;
				}
			}
		}
	}
	this->it_score = new Score(result);
	result->incDepth();
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

Score::Score(Score * source){
	s_value = source->value();
	s_depth = source->depth();
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
	unsigned int score_width = 3;
	char separator_between_nodes = '|';
	char separator_in_nodes = ',';
	unsigned int node_width = 4;

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
				cerr<<" ";
			cerr<<node.first[0];
			/*cerr<<separator_in_nodes;
			cerr<<node.first[1];*/
			cerr<<separator_in_nodes;
			if (node.second->it_score != NULL)
				cerr<<node.second->it_score->depth();
			else
				cerr<<9;
			cerr<<separator_between_nodes;
		}
		cerr<<endl;
		for (unsigned int i_ss = 0; i_ss < stacked_up_tree[i_s].size(); i_ss++){
			pair<Coordinates,IATree *> node = stacked_up_tree[i_s][i_ss];
			for (unsigned int i_spaces = 0; i_spaces < spaces_number; i_spaces++)
				cerr<<" ";
			cerr<<setw(score_width);
			if (node.second->it_score != NULL)
				cerr<<node.second->it_score->value();
			else
				cerr<<255;
			cerr<<separator_between_nodes;
		}
		cerr<<endl;
	}
}
