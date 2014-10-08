/*!
 * \file IA.cpp
 * \brief IA code
 * \author Anaël Petit
 */

#define IA_LEVEL 3

#include "IA.hpp"
#include "IATree.hpp"

Coordinates IA::play(Game * game){
	Coordinates result;

	IATree * iat = new IATree(game->copy(), this);

	iat->populate(IA_LEVEL);
	iat->compute();
	result = iat->bestSon();
	delete iat;

	return result;
}
