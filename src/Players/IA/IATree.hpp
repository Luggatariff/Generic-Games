/*!
 * \file IATree.hpp
 * \brief IATree description
 * \author Anaël Petit
 */

#ifndef IATREE_HPP
#define IATREE_HPP

#include "Game.hpp"
#include "Player.hpp"
#include "Coordinates.hpp"
#include "map"

using namespace std;

/*!
 * \class IATree
 * \brief Tree used to compute IA choices
 */
class IATree{
	Game * it_game;						/*!<Game associated to the node*/
	int it_victory_score;				/*!<Game victory score*/
	Player * it_next_player;			/*!<Game last Player*/
	Player * it_player;					/*!<Player considered in computations*/
	bool it_computed;					/*!<Indicates if the score has been computed*/
	int it_score;						/*!<Game score, when it's computed*/
	Coordinates it_bestson;				/*!<Best son coordinates*/
	map<Coordinates, IATree *> it_sons;	/*!<Map of sons, identified by the Coordinates of the move between this Game and the father's Game*/
public:
	/*!
	 * \brief creates a choice Tree from a given Game and Player
	 * \param game : given Game
	 * \param player : given Player
	 */
	IATree(Game * game, Player * player);
	/*!
	 * \brief IATree destructor
	 */
	~IATree();
	/*!
	 * \brief populates (or re-populates) the choice Tree, initializing (or re-initializing) the computation
	 * \param level : depth of the computation
	 */
	void populate(unsigned int level);
	/*!
	 * \brief computes the score for each node in the Tree in order to know the value of the Root Node
	 * \return computed score
	 */
	int compute();

	/*!
	 * \brief gets the best son's Coordinates
	 * \return best son's Coordinates
	 */
	Coordinates bestSon();
	/*!
	 * \brief gets the map of sons
	 * \return maps of sons
	 */
	map<Coordinates, IATree *> sons();
};

#endif
