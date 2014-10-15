/*!
 * \file IA.cpp
 * \brief IA code
 * \author Anaël Petit
 */

#include "IA.hpp"

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

	unsigned int true_level = ia_level * game->players().size();

	cout<<"Generating choice Tree.."<<endl;

	if (ia_tree == NULL){
		delete ia_tree;
		ia_tree = new IATree(game->copy(), this);
		ia_tree->populate(true_level);
	}
	else{
		vector<Coordinates> game_last_moves = game->lastMoves();
		vector<Coordinates> each_player_last_move;
		for (unsigned int i_p = game_last_moves.size() - game->players().size(); i_p < game_last_moves.size(); i_p++){
			each_player_last_move.push_back(game_last_moves[i_p]);
		}
		ia_tree = ia_tree->changeRoot(each_player_last_move);

		ia_tree->populate(true_level);
	}

	cout<<"Computing best choice.."<<endl;

	Score * score = ia_tree->compute();

	if (ia_display_tree)
		ia_tree->display();

	result = ia_tree->bestSon();

	cout<<"IA chose:";
	for (unsigned int dim = 0; dim < game->dimension(); dim++)
		cout<<result[dim]<<" ";
	cout<<"("<<score->value()<<") "<<endl;

	return result;
}
