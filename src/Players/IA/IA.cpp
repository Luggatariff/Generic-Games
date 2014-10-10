/*!
 * \file IA.cpp
 * \brief IA code
 * \author Anaël Petit
 */

#include "IA.hpp"

IA::IA(unsigned int level){
	ia_level = level;
	ia_tree = NULL;
}

void IA::start(Game * game){
	if (ia_tree != NULL){
		delete ia_tree;
		ia_tree = NULL;
	}
}

Coordinates IA::play(Game * game){
	Coordinates result;

	if (ia_tree == NULL){
		ia_tree = new IATree(game->copy(), this);
		ia_tree->populate(ia_level);
	}
	else{
		ia_tree = ia_tree->changeRoot(ia_last_move);
		ia_tree->populate(ia_level);
		vector<Coordinates> game_last_moves = game->lastMoves();
		vector<Coordinates> other_players_moves;
		for (unsigned int i_m = game_last_moves.size() - 1; i_m >= 0; i_m--){
			if (game_last_moves[i_m] == ia_last_move)
				break;
			else
				other_players_moves.push_back(game_last_moves[i_m]);
		}
		for (unsigned int i_o = 0; i_o < other_players_moves.size(); i_o++){
			IATree * old_tree = ia_tree;
			ia_tree = ia_tree->changeRoot(other_players_moves[i_o]);
			delete old_tree;
			ia_tree->populate(ia_level);
		}
	}

	ia_tree->compute();
	result = ia_tree->bestSon();

	cout<<"IA chose:"<<result[0]<<" "<<result[1]<<endl;

	ia_last_move = result;

	return result;
}
