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
#include "vector"

using namespace std;

/*!
 * \class Score
 * \brief representing a Score in the IATree
 */
class Score{
	int s_value;			/*!<Score value*/
	unsigned int s_depth;	/*!<Score depth, high depths makes Score less important*/
public:
	/*!
	 * \brief Score constructor, from a value
	 * \param value : Score value
	 */
	Score(int value = 0);
	/*!
	 * \brief gets value
	 */
	int value();
	/*!
	 * \brief gets depth
	 */
	unsigned int depth();
	/*!
	 * \brief increases Score depth
	 */
	void incDepth();
};

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
	Score * it_score;					/*!<Game score, when it's computed*/
	vector<Coordinates> it_bestsons;	/*!<Best son coordinates*/
	map<Coordinates, IATree *> it_sons;	/*!<Map of sons, identified by the Coordinates of the move between this Game and the father's Game*/

	/*!
	 * \brief stacks IATree nodes by level, used for display
	 */
	void stackByLevel(vector<vector<pair<Coordinates,IATree *> > > &result, unsigned int level = 0);
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
	Score * compute();

	/*!
	 * \brief gets the best son's Coordinates
	 * \return best son's Coordinates
	 */
	Coordinates bestSon();
	/*!
	 * \brief moves the root to one of the root's son
	 * \param coordinates : Coordinates of the chose son
	 * \return IATree pointer to the new Root
	 */
	IATree * changeRoot(Coordinates coordinates);

	/*!
	 * \brief displays the Tree
	 */
	void display();
};

#endif
