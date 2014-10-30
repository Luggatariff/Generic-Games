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
	 * \brief creates a new Score by copying an old one
	 */
	Score(Score * source);

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

	Score * it_score;					/*!<Game score, when it's computed*/
	bool it_definitive_score;			/*!<Indicates if the computed Score is subject to modifications*/
	map<Coordinates, IATree *> it_sons;	/*!<Map of sons, identified by the Coordinates of the move between this Game and the father's Game*/

	IATree * it_root;					/*!<Indicates IATree root*/
	unsigned int it_node_number;		/*!<Number of Nodes in the Tree*/
	vector<vector<vector<pair<Coordinates,IATree *> > > > it_level_stacks; /*!<IATree stacks by level, used to derecursify populate and compute*/

	/*!
	 * \brief adds a level to the choice Tree
	 */
	void populate_last_level();

	/*!
	 * \brief allows to estimate size of the new last level to be added
	 */
	unsigned int estimate_last_level_size();

	/*!
	 * \brief only recursive function, changes the nodes root
	 */
	void change_root(IATree * new_root);

	/*!
	 * \brief set current IATree as root
	 */
	void set_as_root();
public:
	/*!
	 * \brief creates a choice Tree from a given Game and Player
	 * \param game : given Game
	 * \param player : given Player
	 * \param root : indicates IATree root
	 */
	IATree(Game * game, Player * player, IATree * root = NULL);
	/*!
	 * \brief IATree destructor
	 */
	~IATree();
	/*!
	 * \brief populates (or re-populates) the choice Tree, initializing (or re-initializing) the computation
	 * \param min_level : min depth of the computation
	 * \param max_node_number : when min level is reached, populate function tries to reach a max node number
	 * \return True if the Tree was effectively populated
	 */
	bool populate(unsigned int min_level, unsigned int max_node_number = 0);
	/*!
	 * \brief computes the score for each node in the Tree in order to know the value of the Root Node
	 */
	void compute();

	/*!
	 * \brief clears not definitive Scores
	 */
	void clearScores();

	/*!
	 * \brief gets the IATree Score value
	 * \returns IATree Score value
	 */
	Score * getScore();
	/*!
	 * \brief returns node number
	 * \return node number
	 */
	unsigned int getNodeNumber();

	/*!
	 * \brief moves the root to one of the root's son
	 * \param coordinates : Coordinates of the chosen son
	 * \return IATree list of next possible choices
	 */
	map<Coordinates, IATree *> changeRoot(vector<Coordinates> coordinates);

	/*!
	 * \brief displays the Tree
	 */
	void display();
};

#endif
