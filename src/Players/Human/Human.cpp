/*!
 * \file Human.cpp
 * \brief Human code
 * \author Anaël Petit
 */

#include "Human.hpp"

Coordinates Human::play(Game * game){
	unsigned int dimension = game->dimension();
	Coordinates result(((size_t)dimension));
	do{
		cout<<"Choisissez une case("<<dimension<<" coordonnee(s)):";
		for (unsigned int dim=0; dim<dimension; dim++)
			cin>>result[dim];
	}
	while(!game->isPlayable(result));

	return result;
}
