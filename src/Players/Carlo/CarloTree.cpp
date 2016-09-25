/*!
 * \file CarloTree.cpp
 * \brief CarloTree code
 * \author Anaël Petit
 */

#include <Carlo/CarloTree.hpp>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <algorithm>


CarloTree::CarloTree(Game * game, Player * player, CarloTree * root, CarloTree * father, Coordinates son_id){
	this->it_next_player = game->nextPlayer();
	this->it_player = player;
	this->it_score = NULL;
	this->it_definitive_score = false;
	this->it_level=0;

	if (root == NULL)
		this->it_root = this;
	else
		this->it_root = root;

	if (it_root == this){
		vector<pair<Coordinates, CarloTree *> > first_son_set;
		first_son_set.push_back(pair<Coordinates, CarloTree*>(Coordinates(), this));
		vector<vector<pair<Coordinates, CarloTree *> > > level_zero;
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

CarloTree::~CarloTree(){
	delete it_score;
	if (it_root == this)
		delete it_game;
	for(vector<vector<vector<pair<Coordinates,CarloTree *> > > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		for (vector<vector<pair<Coordinates,CarloTree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			for (vector<pair<Coordinates, CarloTree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
				if (nodes_iterator->second != this && nodes_iterator->second->it_root == this){
					delete nodes_iterator->second;
				}
			}
		}
	}
}

vector<unsigned int> CarloTree::pick_random_indexes(unsigned int number, unsigned int max){
	vector<unsigned int> result;
	unsigned int true_number=number;
	if (max < number){
		true_number=max;
	}
	srand (time(NULL));
	for (unsigned int index=0; index<true_number; index++){
		unsigned int random_index=0;
		random_index = rand() % (max);
		if (find(result.begin(), result.end(), random_index) == result.end()){
			result.push_back(random_index);
		}
		else{
			index--;
		}
	}
	return result;
}

vector<Coordinates> CarloTree::get_last_moves(){
	vector<Coordinates> result;
	if (it_root != this){
		result = it_father->get_last_moves();
		result.push_back(it_son_id);
	}
	return result;
}

Game * CarloTree::get_game_copy(){
	Game * root_game_copy = this->it_root->it_game->copy();
	vector<Coordinates> last_moves = this->get_last_moves();
	for (vector<Coordinates>::iterator last_moves_iterator = last_moves.begin(); last_moves_iterator != last_moves.end(); ++last_moves_iterator){
		root_game_copy->play(*last_moves_iterator);
	}
	return root_game_copy;
}

bool CarloTree::isDefinitve(){
	return it_definitive_score;
}

unsigned int CarloTree::level(){
	return it_level;
}

bool CarloTree::populate(unsigned int min_level, unsigned int free_choice_number){
	int populate_iterations = min_level - it_level;
	it_level=min_level;
	for (int iterations = 0; iterations < populate_iterations; iterations++){
		if (!populate_last_level(free_choice_number, (iterations == populate_iterations-1)))
			break;
	}
	return (populate_iterations>0);
}

bool CarloTree::populate_last_level(unsigned int free_choice_number, bool last_call){
	vector<vector<pair<Coordinates, CarloTree *> > > new_level;
	vector<vector<pair<Coordinates, CarloTree *> > >::iterator last_level_iterator;

	vector<pair<Coordinates, CarloTree *> > merged_last_level;
	for (last_level_iterator = it_level_stacks.back().begin(); last_level_iterator != it_level_stacks.back().end(); ++last_level_iterator){
		merged_last_level.insert(merged_last_level.end(), last_level_iterator->begin(), last_level_iterator->end());
	}
	map<vector<pair<Coordinates, CarloTree *> >::iterator, vector<pair<Coordinates, CarloTree *> > > new_son_sets;
	vector<pair<Coordinates, CarloTree *> > new_son_set;
	for (vector<pair<Coordinates, CarloTree *> >::iterator merged_last_level_iterator = merged_last_level.begin(); merged_last_level_iterator != merged_last_level.end(); ++merged_last_level_iterator)
		 new_son_sets.insert(pair<vector<pair<Coordinates, CarloTree *> >::iterator, vector<pair<Coordinates, CarloTree *> > >(merged_last_level_iterator, new_son_set));

	#pragma omp parallel for
	for (unsigned int omp_merged_last_level_iterator_index=0; omp_merged_last_level_iterator_index < merged_last_level.size(); omp_merged_last_level_iterator_index++){
		vector<pair<Coordinates, CarloTree *> >::iterator omp_merged_last_level_iterator= merged_last_level.begin();
		advance(omp_merged_last_level_iterator, omp_merged_last_level_iterator_index);

		CarloTree * iatree_to_populate = omp_merged_last_level_iterator->second;
		if (! iatree_to_populate->it_definitive_score){
			Game * iatree_to_populate_game = iatree_to_populate->get_game_copy();
			vector<Coordinates> playable_moves = iatree_to_populate_game->playableCoordinates();

			if (!playable_moves.empty()){
				bool next_move_is_ours = (iatree_to_populate_game->nextPlayer()->getTeam() == it_player->getTeam());

				vector<unsigned int> picked_indexes;
				if (free_choice_number!=0 && next_move_is_ours)
					picked_indexes=pick_random_indexes(free_choice_number, playable_moves.size());
				for (unsigned int i_pm = 0; i_pm < playable_moves.size(); i_pm++){
					Game * son_game = iatree_to_populate_game->copy();
					son_game->play(playable_moves[i_pm]);
					CarloTree * new_iatree = new CarloTree(son_game, it_player, this, iatree_to_populate, playable_moves[i_pm]);

					if (son_game->isEnded()){
						new_iatree->it_score = new CarloScore(son_game->score(it_player->getTeam()));
						new_iatree->it_definitive_score = true;

						bool is_winner = son_game->isWinner(it_player->getTeam());

						if (is_winner){
							for(map<Coordinates, CarloTree *>::iterator sons_iterator = iatree_to_populate->it_sons.begin(); sons_iterator != iatree_to_populate->it_sons.end(); sons_iterator++){
								delete sons_iterator->second;
							}
							iatree_to_populate->it_sons.clear();
							iatree_to_populate->it_sons.insert(pair<Coordinates, CarloTree *>(playable_moves[i_pm], new_iatree));
							break;
						}
						else{
							iatree_to_populate->it_sons.insert(pair<Coordinates, CarloTree *>(playable_moves[i_pm], new_iatree));
						}
					}
					else{
						if (last_call){
							new_iatree->it_score = new CarloScore(son_game->score(it_player->getTeam()));
						}
						if (iatree_to_populate_game->nextPlayer()->getTeam() != it_player->getTeam()){
							iatree_to_populate->it_sons.insert(pair<Coordinates, CarloTree *>(playable_moves[i_pm], new_iatree));
						}
						else if(free_choice_number==0 || find(picked_indexes.begin(), picked_indexes.end(), i_pm) != picked_indexes.end()){
							iatree_to_populate->it_sons.insert(pair<Coordinates, CarloTree *>(playable_moves[i_pm], new_iatree));
						}
						else{
							delete new_iatree;
						}
					}
					delete son_game;
				}
				for (map<Coordinates, CarloTree *>::iterator iter_sons = iatree_to_populate->it_sons.begin(); iter_sons != iatree_to_populate->it_sons.end(); iter_sons++){
					new_son_sets[omp_merged_last_level_iterator].push_back(pair<Coordinates, CarloTree *>(iter_sons->first, iter_sons->second));
				}
			}
			else{
				iatree_to_populate->it_score = new CarloScore(iatree_to_populate_game->score(it_player->getTeam()));
				iatree_to_populate->it_definitive_score = true;
			}
			delete iatree_to_populate_game;
		}
	}

	bool nodes_added=false;

	for (map<vector<pair<Coordinates, CarloTree *> >::iterator, vector<pair<Coordinates, CarloTree *> > >::iterator new_son_sets_iterator = new_son_sets.begin(); new_son_sets_iterator != new_son_sets.end(); ++new_son_sets_iterator){
		if (!new_son_sets_iterator->second.empty()){
			nodes_added=true;
			new_level.push_back(new_son_sets_iterator->second);
		}
	}

	if (nodes_added){
		it_level_stacks.push_back(new_level);
	}
	return nodes_added;
}

void CarloTree::compute(){
	vector<vector<vector<pair<Coordinates,CarloTree *> > > >::reverse_iterator level_iterator = it_level_stacks.rbegin();
	for(level_iterator++; level_iterator != it_level_stacks.rend(); ++level_iterator){
		vector<pair<Coordinates,CarloTree *> > merged_level;
		for (vector<vector<pair<Coordinates,CarloTree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			merged_level.insert(merged_level.end(), sons_sets_iterator->begin(), sons_sets_iterator->end());
		}

		#pragma omp parallel for
		for (unsigned int omp_merged_level_iterator_index=0; omp_merged_level_iterator_index < merged_level.size(); omp_merged_level_iterator_index++){
			vector<pair<Coordinates, CarloTree *> >::iterator omp_merged_level_iterator= merged_level.begin();
			advance(omp_merged_level_iterator, omp_merged_level_iterator_index);
			CarloTree * iatree_to_compute = omp_merged_level_iterator->second;

			if (!iatree_to_compute->it_sons.empty() && !iatree_to_compute->it_definitive_score){
				delete iatree_to_compute->it_score;
				iatree_to_compute->it_score = NULL;
				iatree_to_compute->it_definitive_score=true;

				map<Coordinates, CarloTree *>::iterator sons_iterator = iatree_to_compute->it_sons.begin();
				bool return_maximum = (iatree_to_compute->it_next_player->getTeam() == it_player->getTeam());
				CarloScore * best_son_score = sons_iterator->second->it_score;
				iatree_to_compute->it_definitive_score=(iatree_to_compute->it_definitive_score && sons_iterator->second->it_definitive_score);

				if (best_son_score != NULL){
					for(sons_iterator++; sons_iterator != iatree_to_compute->it_sons.end(); sons_iterator++){
						CarloScore * res_score = sons_iterator->second->it_score;
						iatree_to_compute->it_definitive_score=(iatree_to_compute->it_definitive_score && sons_iterator->second->it_definitive_score);

						if (res_score != NULL){
							int victory_score = this->it_root->it_game->victoryScore();

							if (res_score->compare(best_son_score, return_maximum)){
								best_son_score = res_score;
								if (return_maximum && (best_son_score->value() == victory_score)){
									iatree_to_compute->it_definitive_score=true;
									break;
								}
								else if (!return_maximum && (best_son_score->value() == ((-1) * victory_score) && best_son_score->depth() == 0)){
									break;
								}
							}
						}
					}
					iatree_to_compute->it_score = new CarloScore(best_son_score);
					iatree_to_compute->it_score->incDepth();
				}
			}
		}
	}
}

CarloScore * CarloTree::getScore(){
	return this->it_score;
}

void CarloTree::set_as_root(){
	this->it_game = this->get_game_copy();
	this->it_father = NULL;
	change_root(this);
}

void CarloTree::change_root(CarloTree * new_root){
	this->it_root = new_root;
	for (map<Coordinates, CarloTree *>::iterator sons_iterator = it_sons.begin(); sons_iterator != it_sons.end(); sons_iterator++){
		sons_iterator->second->change_root(new_root);
	}
}

map<Coordinates, CarloTree *> CarloTree::changeRoot(vector<Coordinates> coordinates){
	unsigned int final_level=it_level-coordinates.size();

	map<Coordinates, CarloTree *> new_roots;
	CarloTree * new_roots_father = this;
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
		for (map<Coordinates, CarloTree *>::iterator sons_iterator = new_roots_father->it_sons.begin(); sons_iterator != new_roots_father->it_sons.end(); sons_iterator++){
			sons_iterator->second->set_as_root();
			new_roots.insert(pair<Coordinates, CarloTree *>(sons_iterator->first, sons_iterator->second));
		}

		vector<vector<vector<pair<Coordinates,CarloTree *> > > > temp_level_stacks = it_level_stacks;
		for (map<Coordinates, CarloTree *>::iterator new_root = new_roots.begin(); new_root != new_roots.end(); new_root++){
			for(vector<vector<vector<pair<Coordinates,CarloTree *> > > >::iterator level_iterator = temp_level_stacks.begin(); level_iterator != temp_level_stacks.end(); ++level_iterator){
				bool new_level_added = false;
				for(vector<vector<pair<Coordinates,CarloTree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
					bool son_set_added = false;
					for (vector<pair<Coordinates,CarloTree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
						if (nodes_iterator->second->it_root == new_root->second){
							if (!son_set_added){
								if (!new_level_added){
									(new_root->second)->it_level_stacks.push_back(vector<vector<pair<Coordinates, CarloTree *> > >());
									new_level_added = true;
								}
								((new_root->second)->it_level_stacks.back()).push_back(vector<pair<Coordinates, CarloTree *> >());
								son_set_added = true;
							}
							(((new_root->second)->it_level_stacks.back()).back()).push_back(pair<Coordinates, CarloTree *>(nodes_iterator->first, nodes_iterator->second));
						}
					}
				}
			}
			new_root->second->it_level=final_level;
		}
	}

	delete this;

	return new_roots;
}

void CarloTree::display(){
	if (this->it_score != NULL){
		cerr<<"score value : "<<this->it_score->value()<<"\r";
		cerr<<"score depth : "<<this->it_score->depth()<<"\r";
	}
	cerr<<"tree depth : "<<this->it_level_stacks.size()<<"\r";
	unsigned int score_width = 5;
	char separator_between_nodes = '/';
	char separator_between_son_sets = '|';
	char separator_in_nodes = ',';

	unsigned int max_son_set_number = 0;
	unsigned int max_node_number = 0;
	for(vector<vector<vector<pair<Coordinates,CarloTree *> > > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		if (level_iterator->size() > max_son_set_number)
			max_son_set_number = level_iterator->size();
		for(vector<vector<pair<Coordinates,CarloTree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			if (sons_sets_iterator->size() > max_node_number)
				max_node_number = sons_sets_iterator->size();
		}
	}

	for(vector<vector<vector<pair<Coordinates,CarloTree *> > > >::iterator level_iterator = it_level_stacks.begin(); level_iterator != it_level_stacks.end(); ++level_iterator){
		for(vector<vector<pair<Coordinates,CarloTree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			for (vector<pair<Coordinates,CarloTree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
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
		for(vector<vector<pair<Coordinates,CarloTree *> > >::iterator sons_sets_iterator = level_iterator->begin(); sons_sets_iterator != level_iterator->end(); ++sons_sets_iterator){
			for (vector<pair<Coordinates,CarloTree *> >::iterator nodes_iterator = sons_sets_iterator->begin(); nodes_iterator != sons_sets_iterator->end(); ++nodes_iterator){
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
	}
	cerr<<endl;
}

CarloScore::CarloScore(int value){
	s_value = value;
	s_depth = 0;
}

CarloScore::Score(CarloScore * source){
	s_value = source->value();
	s_depth = source->depth();
}

void CarloScore::incDepth(){
	s_depth++;
}

int CarloScore::value(){
	return s_value;
}

unsigned int CarloScore::depth(){
	return s_depth;
}

bool CarloScore::compare(CarloScore * score, bool return_best){
	if (return_best){
		if ((this->value() >= score->value()) && ((this->value() > score->value()) || (this->value() > 0 && this->depth() < score->depth()) || (this->value() < 0 && this->depth() > score->depth()))){
			return true;
		}
	}
	else{
		if ((this->value() <= score->value()) && ((this->value() < score->value()) || (this->value() >= 0 && this->depth() > score->depth()) || (this->value() < 0 && this->depth() < score->depth()))){
			return true;
		}
	}
	return false;
}
