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

	bool compare(Score * score, bool return_best);
};

/*!
 * \class IATree
 * \brief Tree used to compute IA choices
 */
class IATree{
	Game * it_game;						/*!<Game associated to the node*/
	Player * it_next_player;			/*!<Game last Player*/
	Player * it_player;					/*!<Player considered in computations*/

	Score * it_score;					/*!<Game score, when it's computed*/
	bool it_definitive_score;			/*!<Indicates if the computed Score is subject to modifications*/
	map<Coordinates, IATree *> it_sons;	/*!<Map of sons, identified by the Coordinates of the move between this Game and the father's Game*/

	IATree * it_root;					/*!<Indicates IATree root*/
	IATree * it_father;					/*!<Indicates IATree father node*/
	Coordinates it_son_id;				/*!<Indactes one IATree son id for its father*/
	vector<vector<vector<pair<Coordinates,IATree *> > > > it_level_stacks; /*!<IATree stacks by level, used to derecursify populate and compute*/

	unsigned int it_level;				/*!<Population level*/

	/*!
	 * \brief adds a level to the choice Tree
	 * \return true if nodes was actually added
	 */
	bool populate_last_level(unsigned int free_choice_number=0, bool last_call=false);

	/*!
	 * \brief only recursive function, changes the nodes root
	 */
	void change_root(IATree * new_root);

	/*!
	 * \brief set current IATree as root
	 */
	void set_as_root();

	/*!
	 * \brief get the last played moves by getting up the tree
	 * \return list of Coordinates
	 */
	vector<Coordinates> get_last_moves();

	/*!
	 * \brief this function returns a copy of the Game corresponding to its node
	 * \return a Game pointer (don't forget to delete it)
	 */
	Game * get_game_copy();

	/*!
	 * \brief picks a given number of different random indexes
	 * \param number : Random indexes number
	 * \param max : maximum value for the indexes
	 * \return a vector of number size filled with indexes from 0 to max
	 */
	static vector<unsigned int> pick_random_indexes(unsigned int number, unsigned int max);
public:
	/*!
	 * \brief creates a choice Tree from a given Game and Player
	 * \param game : given Game
	 * \param player : given Player
	 * \param root : indicates IATree root
	 */
	IATree(Game * game, Player * player, IATree * root = NULL, IATree * father = NULL, Coordinates son_id = Coordinates(0));
	/*!
	 * \brief IATree destructor
	 */
	~IATree();
	/*!
	 * \brief populates (or re-populates) the choice Tree, initializing (or re-initializing) the computation
	 * \param min_level : min depth of the computation
	 * \return True if the Tree was effectively populated
	 */
	bool populate(unsigned int min_level, unsigned int free_choice_number=0);
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
	 * \brief moves the root to one of the root's son
	 * \param coordinates : Coordinates of the chosen son
	 * \return IATree list of next possible choices
	 */
	map<Coordinates, IATree *> changeRoot(vector<Coordinates> coordinates);

	/*!
	 * \return the IaTree population level
	 */
	unsigned int level();

	/*!
	 * \return True if the IaTree doesn't need to be populated nor computed anymore
	 */
	bool isDefinitve();

	/*!
	 * \brief displays the Tree
	 */
	void display();
};

#endif
