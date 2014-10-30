/*!
 * \file IA.cpp
 * \brief IA code
 * \author Anaël Petit
 */

#include "IA.hpp"

#include <cstdlib>
#include <ctime>

IA::IA(unsigned int level, bool display_tree){
	ia_level = (level > 0) ? level : 1;
	ia_tree = NULL;
	ia_display_tree = display_tree;
}

void IA::start(Game * game){
	if (ia_tree != NULL){
		delete ia_tree;
		ia_tree = NULL;
	}
}

Coordinates IA::play(Game * game){
	Coordinates result;

	map<Coordinates, IATree *> choices;

	if (ia_tree != NULL){
		vector<Coordinates> game_last_moves = game->lastMoves();
		vector<Coordinates> each_player_last_move;
		for (unsigned int i_p = game_last_moves.size() - (game->players().size() - 1); i_p < game_last_moves.size(); i_p++){
			each_player_last_move.push_back(game_last_moves[i_p]);
		}
		choices = ia_tree->changeRoot(each_player_last_move);
	}

	if (choices.empty()){
		vector<Coordinates> playable_moves = game->playableCoordinates();
		for (vector<Coordinates>::iterator move = playable_moves.begin(); move != playable_moves.end(); move++){
			Game * new_game = game->copy();
			new_game->play(*move);
			choices.insert(pair<Coordinates, IATree *>((*move), new IATree(new_game, this)));
		}
	}

	cout<<"Computing best choice.."<<endl;

	unsigned int temp_level = 1;
	unsigned int player_number = game->players().size();
	int victory_score = game->victoryScore();
	while(choices.size() > 1 && temp_level <= ia_level){
		cout<<"Thinking Deep.."<<endl;
		unsigned int true_level = temp_level * player_number - 1;

		map<Coordinates, IATree *>::iterator choices_iterator = choices.begin();

		if (choices_iterator->second->populate(true_level)){
			choices_iterator->second->compute();
		}

		Score * best_choice_score = choices_iterator->second->getScore();

		if (best_choice_score->value() != victory_score){
			for(choices_iterator++; choices_iterator != choices.end(); choices_iterator++){
				if (choices_iterator->second->populate(true_level))
					choices_iterator->second->compute();
				Score * res_score = choices_iterator->second->getScore();
				if ((res_score->value() >= best_choice_score->value()) && ((res_score->value() > best_choice_score->value()) || (res_score->value() > 0 && res_score->depth() < best_choice_score->depth()) || (res_score->value() < 0 && res_score->depth() > best_choice_score->depth()))){
					best_choice_score = res_score;
					if (best_choice_score->value() == victory_score)
						break;
				}
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

		if (best_choice_score->value() == victory_score || best_choice_score->value() == -victory_score)
			break;

		temp_level++;
	}

	cout<<"Done!"<<endl;

	map<Coordinates, IATree *>::iterator choice = choices.begin();
	if (choices.size() > 1){
		unsigned int i_choice = 0;
		srand (time(NULL));
		i_choice = rand() % (choices.size() - 1);
		advance(choice, i_choice);
	}

	ia_tree = choice->second;
	result = choice->first;
	choices.erase(choice);

	for (map<Coordinates, IATree *>::iterator choices_iterator = choices.begin(); choices_iterator != choices.end(); choices_iterator++){
		delete choices_iterator->second;
	}

	cout<<"IA chose:";
	for (unsigned int dim = 0; dim < game->dimension(); dim++)
		cout<<result[dim]<<" ";
	cout<<endl;

	if (ia_display_tree)
		ia_tree->display();

	return result;
}
