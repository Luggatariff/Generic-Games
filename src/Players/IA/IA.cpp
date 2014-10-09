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

Coordinates IA::play(Game * game){
	Coordinates result;

	ia_tree = new IATree(game->copy(), this);

	ia_tree->populate(ia_level);
	ia_tree->compute();
	result = ia_tree->bestSon();
	delete ia_tree;

	cout<<"IA chose:"<<result[0]<<" "<<result[1]<<endl;

	return result;
}
