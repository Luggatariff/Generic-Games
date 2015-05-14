/*!
 * \file IATree.cpp
 * \brief IATree code
 * \author Anaël Petit
 */

#include <iomanip>

#include "IATree.hpp"

IATree::IATree(Game * game, Player * player, IATree * root, IATree * father, Coordinates son_id){
	this->it_next_player = game->nextPlayer();
	this->it_player = player;
	this->it_score = NULL;
	this->it_definitive_score = false;

	if (root == NULL)
		this->it_root = this;
	else
		this->it_root = root;

	if (it_root == this){
		vector<pair<Coordinates, IATree *> > first_son_set;
		first_son_set.push_back(pair<Coordinates, IATree*>(Coordinates(), this));
		vector<vector<pair<Coordinates, IATree *> > > level_zero;
		level_zero.push_back(first_son_set);
		it_level_stacks.push_back(level_zero);

		this->it_father = NULL;
		this->it_game = game;
	}
	else{
		this->it_father = father;
		this->it_son_id = son_id;
		this->it_game = NULL;
	}
}

IATree::~IATree(){
	delete it_score;
	if (it_root == this)
		delete it_game;
	for(vector<vector<vector<pair<Coordinates,IATree *> > > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		for (vector<vector<pair<Coordinates,IATree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			for (vector<pair<Coordinates, IATree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
				if (nodes_iterator->second != this && nodes_iterator->second->it_root == this){
					delete nodes_iterator->second;
				}
			}
		}
	}
}

vector<Coordinates> IATree::get_last_moves(){
	vector<Coordinates> result;
	if (it_root != this){
		result = it_father->get_last_moves();
		result.push_back(it_son_id);
	}
	return result;
}

Game * IATree::get_game_copy(){
	Game * root_game_copy = this->it_root->it_game->copy();
	vector<Coordinates> last_moves = this->get_last_moves();
	for (vector<Coordinates>::iterator last_moves_iterator = last_moves.begin(); last_moves_iterator != last_moves.end(); ++last_moves_iterator){
		root_game_copy->play(*last_moves_iterator);
	}
	return root_game_copy;
}

bool IATree::populate(unsigned int min_level){
	int populate_iterations = min_level - it_level_stacks.size() + 1;
	for (int iterations = 0; iterations < populate_iterations; iterations++)
		populate_last_level();
	return (populate_iterations>0);
}

void IATree::populate_last_level(unsigned int max_node_number){
	vector<vector<pair<Coordinates, IATree *> > > new_level;
	vector<vector<pair<Coordinates, IATree *> > >::iterator last_level_iterator;

	vector<pair<Coordinates, IATree *> > merged_last_level;
	for (last_level_iterator = it_level_stacks.back().begin(); last_level_iterator != it_level_stacks.back().end(); ++last_level_iterator){
		merged_last_level.insert(merged_last_level.end(), last_level_iterator->begin(), last_level_iterator->end());
	}
	map<vector<pair<Coordinates, IATree *> >::iterator, vector<pair<Coordinates, IATree *> > > new_son_sets;
	vector<pair<Coordinates, IATree *> > new_son_set;
	for (vector<pair<Coordinates, IATree *> >::iterator merged_last_level_iterator = merged_last_level.begin(); merged_last_level_iterator != merged_last_level.end(); ++merged_last_level_iterator)
		 new_son_sets.insert(pair<vector<pair<Coordinates, IATree *> >::iterator, vector<pair<Coordinates, IATree *> > >(merged_last_level_iterator, new_son_set));

	#pragma omp parallel for
	for (unsigned int omp_merged_last_level_iterator_index=0; omp_merged_last_level_iterator_index < merged_last_level.size(); omp_merged_last_level_iterator_index++){
		vector<pair<Coordinates, IATree *> >::iterator omp_merged_last_level_iterator= merged_last_level.begin();
		advance(omp_merged_last_level_iterator, omp_merged_last_level_iterator_index);

		IATree * iatree_to_populate = omp_merged_last_level_iterator->second;
		if (! iatree_to_populate->it_definitive_score){
			Game * iatree_to_populate_game = iatree_to_populate->get_game_copy();
			vector<Coordinates> playable_moves = iatree_to_populate_game->playableCoordinates();

			if (!playable_moves.empty()){
					for (unsigned int i_pm = 0; i_pm < playable_moves.size(); i_pm++){
						Game * son_game = iatree_to_populate_game->copy();
						son_game->play(playable_moves[i_pm]);

						if (son_game->isEnded()){
							IATree * new_iatree = new IATree(son_game, it_player, this, iatree_to_populate, playable_moves[i_pm]);
							new_iatree->it_score = new Score(son_game->score(it_player->getTeam()));
							new_iatree->it_definitive_score = true;
							bool is_winner = son_game->isWinner(it_player->getTeam());
							if (is_winner && iatree_to_populate_game->nextPlayer() == it_player){
								for(map<Coordinates, IATree *>::iterator sons_iterator = iatree_to_populate->it_sons.begin(); sons_iterator != iatree_to_populate->it_sons.end(); sons_iterator++){
									delete sons_iterator->second;
								}
								iatree_to_populate->it_sons.clear();
								iatree_to_populate->it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new_iatree));
								break;
							}
							else{
								iatree_to_populate->it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new_iatree));
							}
						}
						else
							iatree_to_populate->it_sons.insert(pair<Coordinates, IATree *>(playable_moves[i_pm], new IATree(son_game, it_player, this, iatree_to_populate, playable_moves[i_pm])));
						delete son_game;
					}
					for (map<Coordinates, IATree *>::iterator iter_sons = iatree_to_populate->it_sons.begin(); iter_sons != iatree_to_populate->it_sons.end(); iter_sons++){
						new_son_sets[omp_merged_last_level_iterator].push_back(pair<Coordinates, IATree *>(iter_sons->first, iter_sons->second));
					}
			}
			delete iatree_to_populate_game;
		}
	}

	for (map<vector<pair<Coordinates, IATree *> >::iterator, vector<pair<Coordinates, IATree *> > >::iterator new_son_sets_iterator = new_son_sets.begin(); new_son_sets_iterator != new_son_sets.end(); ++new_son_sets_iterator){
		if (!new_son_sets_iterator->second.empty()) new_level.push_back(new_son_sets_iterator->second);
	}

	it_level_stacks.push_back(new_level);
}

void IATree::compute(){
	for(vector<vector<vector<pair<Coordinates,IATree *> > > >::reverse_iterator level_iterator = it_level_stacks.rbegin(); level_iterator != it_level_stacks.rend(); ++level_iterator){
		vector<pair<Coordinates,IATree *> > merged_level;
		for (vector<vector<pair<Coordinates,IATree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			merged_level.insert(merged_level.end(), sons_sets_iterator->begin(), sons_sets_iterator->end());
		}

		#pragma omp parallel for
		for (unsigned int omp_merged_level_iterator_index=0; omp_merged_level_iterator_index < merged_level.size(); omp_merged_level_iterator_index++){
			vector<pair<Coordinates, IATree *> >::iterator omp_merged_level_iterator= merged_level.begin();
			advance(omp_merged_level_iterator, omp_merged_level_iterator_index);
			IATree * iatree_to_compute = omp_merged_level_iterator->second;

			if (!iatree_to_compute->it_definitive_score){
				delete iatree_to_compute->it_score;
				iatree_to_compute->it_score = NULL;
			}

			if (iatree_to_compute->it_score == NULL){

				if (iatree_to_compute->it_sons.empty() && ! iatree_to_compute->it_definitive_score){
					Game * iatree_to_compute_game = iatree_to_compute->get_game_copy();
					iatree_to_compute->it_score = new Score(iatree_to_compute_game->score(it_player->getTeam()));
					delete iatree_to_compute_game;
				}
				else if (!iatree_to_compute->it_sons.empty()){
					map<Coordinates, IATree *>::iterator sons_iterator = iatree_to_compute->it_sons.begin();
					bool return_maximum = (iatree_to_compute->it_next_player == it_player);
					Score * best_son_score = sons_iterator->second->it_score;

					if (best_son_score != NULL){

						for(sons_iterator++; sons_iterator != iatree_to_compute->it_sons.end(); sons_iterator++){
							Score * res_score = sons_iterator->second->it_score;

							if (res_score != NULL){
								int victory_score = this->it_root->it_game->victoryScore();

								if (return_maximum){
									if ((res_score->value() >= best_son_score->value()) && ((res_score->value() > best_son_score->value()) || (res_score->value() > 0 && res_score->depth() < best_son_score->depth()) || (res_score->value() < 0 && res_score->depth() > best_son_score->depth()))){
										best_son_score = res_score;
										if (best_son_score->value() == victory_score)
											break;
									}
								}
								else{
									if ((res_score->value() <= best_son_score->value()) && ((res_score->value() < best_son_score->value()) || (res_score->value() >= 0 && res_score->depth() > best_son_score->depth()) || (res_score->value() < 0 && res_score->depth() < best_son_score->depth()))){
										best_son_score = res_score;
										if (best_son_score->value() == ((-1) * victory_score) && best_son_score->depth() == 0)
											break;
									}
								}
							}
						}
						iatree_to_compute->it_score = new Score(best_son_score);
						iatree_to_compute->it_score->incDepth();
					}
				}
			}
		}
	}
}

Score * IATree::getScore(){
	return this->it_score;
}

void IATree::set_as_root(){
	this->it_game = this->get_game_copy();
	this->it_father = NULL;
	change_root(this);
}

void IATree::change_root(IATree * new_root){
	this->it_root = new_root;
	for (map<Coordinates, IATree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		sons_iterator->second->change_root(new_root);
	}
}

map<Coordinates, IATree *> IATree::changeRoot(vector<Coordinates> coordinates){
	map<Coordinates, IATree *> new_roots;
	IATree * new_roots_father = this;
	for (vector<Coordinates>::iterator coordinates_iterator = coordinates.begin(); coordinates_iterator != coordinates.end(); coordinates_iterator++){
		if (new_roots_father->it_sons.empty()) break;
		try{
			new_roots_father = new_roots_father->it_sons.at(*coordinates_iterator);
		}
		catch(...){
			new_roots_father = NULL;
		}
	}
	if (new_roots_father != NULL){
		for (map<Coordinates, IATree *>::iterator sons_iterator = new_roots_father->it_sons.begin(); sons_iterator != new_roots_father->it_sons.end(); sons_iterator++){
			sons_iterator->second->set_as_root();
			new_roots.insert(pair<Coordinates, IATree *>(sons_iterator->first, sons_iterator->second));
		}

		vector<vector<vector<pair<Coordinates,IATree *> > > > temp_level_stacks = it_level_stacks;
		for (map<Coordinates, IATree *>::iterator new_root = new_roots.begin(); new_root != new_roots.end(); new_root++){
			for(vector<vector<vector<pair<Coordinates,IATree *> > > >::iterator level_iterator = temp_level_stacks.begin(); level_iterator != temp_level_stacks.end(); ++level_iterator){
				bool new_level_added = false;
				for(vector<vector<pair<Coordinates,IATree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
					bool son_set_added = false;
					for (vector<pair<Coordinates,IATree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
						if (nodes_iterator->second->it_root == new_root->second){
							if (!son_set_added){
								if (!new_level_added){
									(new_root->second)->it_level_stacks.push_back(vector<vector<pair<Coordinates, IATree *> > >());
									new_level_added = true;
								}
								((new_root->second)->it_level_stacks.back()).push_back(vector<pair<Coordinates, IATree *> >());
								son_set_added = true;
							}
							(((new_root->second)->it_level_stacks.back()).back()).push_back(pair<Coordinates, IATree *>(nodes_iterator->first, nodes_iterator->second));
						}
					}
				}
			}
		}
	}

	delete this;

	return new_roots;
}

void IATree::display(){
	if (this->it_score != NULL){
		cerr<<"score value : "<<this->it_score->value()<<"\r";
		cerr<<"score depth : "<<this->it_score->depth()<<"\r";
	}
	cerr<<"tree depth : "<<this->it_level_stacks.size()<<"\r";
	/*unsigned int score_width = 5;
	char separator_between_nodes = '/';
	char separator_between_son_sets = '|';
	char separator_in_nodes = ',';

	unsigned int max_son_set_number = 0;
	unsigned int max_node_number = 0;
	for(vector<vector<vector<pair<Coordinates,IATree *> > > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		if (level_iterator->size() > max_son_set_number)
			max_son_set_number = level_iterator->size();
		for(vector<vector<pair<Coordinates,IATree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			if (sons_sets_iterator->size() > max_node_number)
				max_node_number = sons_sets_iterator->size();
		}
	}

	for(vector<vector<vector<pair<Coordinates,IATree *> > > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		for(vector<vector<pair<Coordinates,IATree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			for (vector<pair<Coordinates,IATree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
				cerr<<nodes_iterator->first[0];
				cerr<<separator_in_nodes;
				cerr<<nodes_iterator->first[1];
				cerr<<separator_in_nodes;
				if (nodes_iterator->second->it_score != NULL)
					cerr<<nodes_iterator->second->it_score->depth();
				else
					cerr<<9;
				cerr<<separator_between_nodes;
			}
			cerr<<separator_between_son_sets;
		}
		cerr<<"\r";
		for(vector<vector<pair<Coordinates,IATree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			for (vector<pair<Coordinates,IATree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
				cerr<<setw(score_width);
				if (nodes_iterator->second->it_score != NULL)
					cerr<<nodes_iterator->second->it_score->value();
				else
					cerr<<255;
				cerr<<separator_between_nodes;
			}
			cerr<<separator_between_son_sets;
		}
		cerr<<"\r";
	}*/
	cerr<<endl;
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
