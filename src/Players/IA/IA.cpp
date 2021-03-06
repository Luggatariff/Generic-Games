/*!
 * \file IA.cpp
 * \brief IA code
 * \author Ana�l Petit
 */

#include "IA.hpp"

#include <cstdlib>
#include <ctime>
#include <algorithm>

IA::IA(string name, unsigned int team_id, unsigned int level, unsigned int max_free_choices, bool display_messages, bool display_tree){
	ia_name = name;
	ia_team = team_id;

	ia_level = level;
	ia_max_free_choices = max_free_choices;
	ia_tree = NULL;
	ia_display_tree = display_tree;
	ia_display_messages = display_messages;
}

void IA::start(Game * game){
	if (ia_tree != NULL){
		delete ia_tree;
		ia_tree = NULL;
	}
}

void IA::end(Game * game){
	if (ia_tree != NULL){
		delete ia_tree;
		ia_tree = NULL;
	}
}

Coordinates IA::play(Game * game , vector<Coordinates> limit_choices){
	Coordinates result;

	map<Coordinates, IATree *> choices;

	if (ia_tree != NULL && ia_max_free_choices == 0){
		vector<Coordinates> game_last_moves = game->lastMoves();
		vector<Coordinates> each_player_last_move;
		for (unsigned int i_p = game_last_moves.size() - (game->players().size() - 1); i_p < game_last_moves.size(); i_p++){
			each_player_last_move.push_back(game_last_moves[i_p]);
		}
		choices = ia_tree->changeRoot(each_player_last_move);
	}
	else if (ia_tree != NULL){
		delete ia_tree;
	}

	if (limit_choices.size() > 1){
		map<Coordinates, IATree *>::iterator choices_it = choices.begin();
		while (choices_it != choices.end()){
			if (find(limit_choices.begin(), limit_choices.end(), choices_it->first) == limit_choices.end()){
				delete choices_it->second;
				choices.erase(choices_it++);
			}
			else
				++choices_it;
		}
	}

	if (choices.empty()){
		vector<Coordinates> playable_moves = game->playableCoordinates();
		for (vector<Coordinates>::iterator move = playable_moves.begin(); move != playable_moves.end(); move++){
			Game * new_game = game->copy();
			new_game->play(*move);
			choices.insert(pair<Coordinates, IATree *>((*move), new IATree(new_game, this)));
		}
	}

	unsigned int level=choices.begin()->second->level()+1;
	unsigned int max_level = ia_level;
	int victory_score = game->victoryScore();

	if (ia_display_messages)
		cout<<"Thinking.."<<endl;
	while(choices.size() > 1 && level <= max_level){
		bool compute_occured=false;

		vector<unsigned int> random_indexes = IATree::pick_random_indexes(choices.size(), choices.size());

		vector<unsigned int>::iterator random_indexes_iterator = random_indexes.begin();
		map<Coordinates, IATree *>::iterator choices_iterator = choices.begin();
		advance(choices_iterator, *random_indexes_iterator);

		if (!choices_iterator->second->isDefinitve() && choices_iterator->second->populate(level, ia_max_free_choices)){
			compute_occured=true;
			choices_iterator->second->compute();
		}
		Score * best_choice_score = choices_iterator->second->getScore();

		if ( (best_choice_score->value() < victory_score && level <= max_level) || (best_choice_score->value() < 0 && level == max_level) ) {
			for(random_indexes_iterator++; random_indexes_iterator != random_indexes.end(); random_indexes_iterator++){
				choices_iterator = choices.begin();
				advance(choices_iterator, *random_indexes_iterator);

				if (!choices_iterator->second->isDefinitve() && choices_iterator->second->populate(level, ia_max_free_choices)){
					compute_occured=true;
					choices_iterator->second->compute();
				}
				Score * res_score = choices_iterator->second->getScore();
				if (res_score->compare(best_choice_score, true)){
					best_choice_score = res_score;
				}

				if ((best_choice_score->value() == victory_score && level <= max_level) || (best_choice_score->value() >= 0 && level == max_level && compute_occured))
					break;
			}
		}

		choices_iterator = choices.begin();

		while (choices_iterator != choices.end()){
			Score * choice_score = choices_iterator->second->getScore();
			if (choice_score != NULL && choice_score->depth() == best_choice_score->depth() && choice_score->value() == best_choice_score->value()){
				++choices_iterator;
			}
			else{
				delete choices_iterator->second;
				choices.erase(choices_iterator++);
			}
		}

		if (!compute_occured)
			break;

		if (ia_display_messages)
			cout<<level<<" Step(s) forward."<<endl;

		if (best_choice_score->value() == victory_score || best_choice_score->value() == -victory_score)
			break;

		level++;
	}
	if (ia_display_messages)
		cout<<"Done!"<<endl;

	map<Coordinates, IATree *>::iterator choice = choices.begin();
	if (choices.size() > 1){
		unsigned int i_choice = 0;
		srand (time(NULL));
		i_choice = rand() % (choices.size());
		advance(choice, i_choice);
	}

	ia_tree = choice->second;
	result = choice->first;
	choices.erase(choice);

	for (map<Coordinates, IATree *>::iterator choices_iterator = choices.begin(); choices_iterator != choices.end(); choices_iterator++){
		delete choices_iterator->second;
	}

	if (ia_display_messages){
		cout<<"IA chose:";
		for (unsigned int dim = 0; dim < game->dimension(); dim++)
			cout<<result[dim]<<" ";
		cout<<endl;
	}

	if (ia_display_tree){
		ia_tree->display();
	}

	return result;
}

string IA::getName(){
	return ia_name;
}

unsigned int IA::getTeam(){
	return ia_team;
}
