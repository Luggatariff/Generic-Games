/*!
 * \file IA.cpp
 * \brief IA code
 * \author Anaël Petit
 */

#include "IA.hpp"

IA::IA(unsigned int level, bool fast_compute, bool display_tree){
	ia_level = (level > 0) ? level : 1;
	ia_tree = NULL;
	ia_fast_compute = fast_compute;
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

	unsigned int true_level = ia_level * game->players().size();

	if (ia_tree == NULL){
		delete ia_tree;
		ia_tree = new IATree(game->copy(), this);
		ia_tree->populate(true_level, ia_fast_compute);
	}
	else{
		IATree * old_tree = ia_tree;
		ia_tree = ia_tree->changeRoot(ia_last_move);
		delete old_tree;
		vector<Coordinates> game_last_moves = game->lastMoves();
		vector<Coordinates> other_players_moves;
		for (unsigned int i_p = 0; i_p < game->players().size() - 1; i_p++){
			other_players_moves.push_back(game_last_moves[game_last_moves.size() - 1 - i_p]);
		}
		for (unsigned int i_o = 0; i_o < other_players_moves.size(); i_o++){
			old_tree = ia_tree;
			ia_tree = ia_tree->changeRoot(other_players_moves[i_o]);
			delete old_tree;
		}
		ia_tree->populate(true_level, ia_fast_compute);
	}

	Score * score = ia_tree->compute();
	if (ia_display_tree)
		ia_tree->display();
	result = ia_tree->bestSon();

	cout<<"IA chose:";
	for (unsigned int dim = 0; dim < game->dimension(); dim++)
		cout<<result[dim]<<" ";
	cout<<"("<<score->value()<<") "<<endl;

	ia_last_move = result;

	return result;
}
