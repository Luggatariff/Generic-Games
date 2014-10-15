/*!
 * \file IATree.cpp
 * \brief IATree code
 * \author Anaël Petit
 */

#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "IATree.hpp"

IATree::IATree(Game * game, Player * player, IATree * root){
	this->it_game = game;
	this->it_next_player = game->nextPlayer();
	this->it_victory_score = game->victoryScore();
	this->it_player = player;
	this->it_score = NULL;

	if (root == NULL)
		this->it_root = this;
	else
		this->it_root = root;

	if (it_root == this){
		vector<pair<Coordinates, IATree *> > level_zero;
		level_zero.push_back(pair<Coordinates, IATree*>(Coordinates(), this));
		it_level_stacks.push_back(level_zero);
	}
}

IATree::~IATree(){
	delete it_score;
	delete it_game;
	for(vector<vector<pair<Coordinates,IATree *> > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		for (vector<pair<Coordinates,IATree *> >::iterator nodes_iterator = level_iterator->begin(); nodes_iterator != level_iterator->end(); ++nodes_iterator){
			if (nodes_iterator->second != this && nodes_iterator->second->it_root == this){
				delete nodes_iterator->second;
			}
		}
	}
}

void IATree::populate(unsigned int level){
	unsigned int populate_iterations = level - it_level_stacks.size() + 1;
	for (unsigned int iterations = 0; iterations < populate_iterations; iterations++)
		populate_last_level();
}

void IATree::populate_last_level(){
	vector<pair<Coordinates, IATree *> > last_level = it_level_stacks.back();

	vector<pair<Coordinates, IATree *> > new_level;
	for (vector<pair<Coordinates, IATree *> >::iterator last_level_iterator = last_level.begin(); last_level_iterator != last_level.end(); ++last_level_iterator){
		IATree * iatree_to_populate = last_level_iterator->second;

		if (iatree_to_populate->it_game != NULL){
			vector<Coordinates> playable_moves = iatree_to_populate->it_game->playableCoordinates();

			if (!playable_moves.empty()){
				delete iatree_to_populate->it_score;
				iatree_to_populate->it_score = NULL;

				for (unsigned int i_pm = 0; i_pm < playable_moves.size(); i_pm++){
					Game * son_game = iatree_to_populate->it_game->copy();
					son_game->play(playable_moves[i_pm]);

					if ((iatree_to_populate->it_next_player == iatree_to_populate->it_player) && son_game->isWinner(iatree_to_populate->it_player)){
						for(map<Coordinates, IATree *>::iterator sons_iterator = iatree_to_populate->it_sons.begin(); sons_iterator != iatree_to_populate->it_sons.end(); sons_iterator++){
							delete sons_iterator->second;
						}
						iatree_to_populate->it_sons.clear();
						iatree_to_populate->it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new IATree(son_game, iatree_to_populate->it_player, this)));
						break;
					}

					iatree_to_populate->it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new IATree(son_game, iatree_to_populate->it_player, this)));
				}
				for (map<Coordinates, IATree *>::iterator iter_sons = iatree_to_populate->it_sons.begin(); iter_sons != iatree_to_populate->it_sons.end(); iter_sons++)
					new_level.push_back(pair<Coordinates, IATree *>(iter_sons->first, iter_sons->second));
				delete iatree_to_populate->it_game;
				iatree_to_populate->it_game = NULL;
			}
		}
	}

	it_level_stacks.push_back(new_level);
}

Score * IATree::compute(){
	for(vector<vector<pair<Coordinates,IATree *> > >::reverse_iterator level_iterator = it_level_stacks.rbegin(); level_iterator != it_level_stacks.rend(); ++level_iterator){
		for (vector<pair<Coordinates,IATree *> >::iterator nodes_iterator = level_iterator->begin(); nodes_iterator != level_iterator->end(); ++nodes_iterator){
			IATree * iatree_to_compute = nodes_iterator->second;
			if (iatree_to_compute->it_score != NULL) continue;

			if (iatree_to_compute->it_sons.empty() && iatree_to_compute->it_game != NULL){
				iatree_to_compute->it_score = new Score(iatree_to_compute->it_game->score(it_player));
			}
			else if (!iatree_to_compute->it_sons.empty()){
				bool return_maximum = (iatree_to_compute->it_next_player == iatree_to_compute->it_player);
				map<Coordinates, IATree *>::iterator sons_iterator = iatree_to_compute->it_sons.begin();
				Score * best_son_score = sons_iterator->second->it_score;

				for(sons_iterator++; sons_iterator != iatree_to_compute->it_sons.end(); sons_iterator++){
					Score * res_score = sons_iterator->second->it_score;
					if (return_maximum){
						if ((res_score->value() >= best_son_score->value()) && ((res_score->value() > best_son_score->value()) || (res_score->value() > 0 && res_score->depth() < best_son_score->depth()) || (res_score->value() < 0 && res_score->depth() > best_son_score->depth()))){
							best_son_score = res_score;
							if (best_son_score->value() == it_victory_score)
								break;
						}
					}
					else{
						if ((res_score->value() <= best_son_score->value()) && ((res_score->value() < best_son_score->value()) || (res_score->value() >= 0 && res_score->depth() > best_son_score->depth()) || (res_score->value() < 0 && res_score->depth() < best_son_score->depth()))){
							best_son_score = res_score;
							if (best_son_score->value() == ((-1) * it_victory_score) && best_son_score->depth() == 0)
								break;
						}
					}
				}
				iatree_to_compute->it_score = new Score(best_son_score);
				iatree_to_compute->it_score->incDepth();
			}
		}
	}

	return this->it_score;
}

Coordinates IATree::bestSon(){
	vector<Coordinates> it_bestsons;
	for(map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		if (sons_iterator->second->it_score->value() == this->it_score->value() && sons_iterator->second->it_score->depth() == (this->it_score->depth() - 1)){
			it_bestsons.push_back(sons_iterator->first);
		}
	}
	Coordinates bestson_coordinates;
	if (it_bestsons.size() == 1)
		bestson_coordinates = it_bestsons[0];
	else{
		srand ( time(NULL) );
		unsigned int random_uint;
		random_uint =  rand() % (it_bestsons.size()-1);
		bestson_coordinates = it_bestsons[random_uint];
	}
	return bestson_coordinates;
}

void IATree::set_as_root(){
	change_root(this);
}

void IATree::change_root(IATree * new_root){
	this->it_root = new_root;
	for (map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		sons_iterator->second->change_root(new_root);
	}
}

IATree * IATree::changeRoot(vector<Coordinates> coordinates){
	IATree * new_root = this;
	for (vector<Coordinates>::iterator coordinates_iterator = coordinates.begin(); coordinates_iterator != coordinates.end(); coordinates_iterator++){
		new_root = new_root->it_sons[*coordinates_iterator];
	}

	new_root->set_as_root();
	vector<vector<pair<Coordinates,IATree *> > > temp_level_stacks = it_level_stacks;
	for(vector<vector<pair<Coordinates,IATree *> > >::iterator level_iterator = temp_level_stacks.begin(); level_iterator != temp_level_stacks.end(); ++level_iterator){
		bool new_level_added = false;
		for (vector<pair<Coordinates,IATree *> >::iterator nodes_iterator = level_iterator->begin(); nodes_iterator != level_iterator->end(); ++nodes_iterator){
			if (nodes_iterator->second->it_root == new_root){
				if (!new_level_added){
					new_root->it_level_stacks.push_back(vector<pair<Coordinates, IATree *> >());
					new_level_added = true;
				}
				(new_root->it_level_stacks.back()).push_back(pair<Coordinates, IATree *>(nodes_iterator->first, nodes_iterator->second));
			}
		}
	}

	delete this;

	return new_root;
}

void IATree::display(){
	unsigned int score_width = 5;
	char separator_between_nodes = '|';
	char separator_in_nodes = ',';
	unsigned int node_width = 6;

	unsigned int max_level_node_number = 0;
	for (unsigned int i_s = 0; i_s< it_level_stacks.size(); i_s++){
		if (it_level_stacks[i_s].size() > max_level_node_number)
			max_level_node_number = it_level_stacks[i_s].size();

	}
	unsigned int max_level_width = max_level_node_number * node_width;
	for (unsigned int i_s = 0; i_s< it_level_stacks.size(); i_s++){
		unsigned int spaces_number = (max_level_width - node_width * it_level_stacks[i_s].size()) / (it_level_stacks[i_s].size() + 1);
		for (unsigned int i_ss = 0; i_ss < it_level_stacks[i_s].size(); i_ss++){
			pair<Coordinates,IATree *> node = it_level_stacks[i_s][i_ss];
			for (unsigned int i_spaces = 0; i_spaces < spaces_number; i_spaces++)
				cerr<<" ";
			cerr<<node.first[0];
			cerr<<separator_in_nodes;
			cerr<<node.first[1];
			cerr<<separator_in_nodes;
			if (node.second->it_score != NULL)
				cerr<<node.second->it_score->depth();
			else
				cerr<<9;
			cerr<<separator_between_nodes;
		}
		cerr<<endl;
		for (unsigned int i_ss = 0; i_ss < it_level_stacks[i_s].size(); i_ss++){
			pair<Coordinates,IATree *> node = it_level_stacks[i_s][i_ss];
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
