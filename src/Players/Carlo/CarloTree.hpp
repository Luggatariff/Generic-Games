/*!
 * \file CarloTree.hpp
 * \brief CarloTree description
 * \author Anaël Petit
 */

#ifndef CARLOTREE_HPP
#define CARLOTREE_HPP

#include "Game.hpp"
#include "Player.hpp"
#include "Coordinates.hpp"
#include "map"
#include "vector"

using namespace std;

/*!
 * \class Score
 * \brief representing a Score in the CarloTree
 */
class CarloScore{
	unsigned int 	s_simulationNumber 		= 0;	/*!<Number of simulations made for this node and its children*/
	unsigned int	s_winNumber				= 0;	/*!<Number of wins for this nodes and its children*/
	int				s_explorationParameter;			/*!<Coefficient giving the importance of expanding the tree*/
public:
	/*!
	 * \brief Score constructor, from an exploration parameter
	 * \param explorationParameter : Score exploration parameter
	 */
	CarloScore(int explorationParameter);

	/*!
	 * \brief gets score computed value
	 */
	float computeScore();

	/*!
	 * \brief increases SimulationNumber
	 */
	void incSimulationNumber();

	/*!
	 * \brief increases WinNumber
	 */
	void incWinNumber();
};

/*!
 * \class CarloTree
 * \brief Tree used to compute Carlo choices
 */
class CarloTree{
	Game * 							ct_game;				/*!<Game associated to the node*/
	unsigned int 					ct_team;				/*!<Team for which the CarloTree is playing*/

	CarloScore * 					ct_score;				/*!<CarloTree node Score*/

	map<Coordinates, CarloTree *>	ct_sons;				/*!<Map of sons, identified by the Coordinates of the move between this Game and the father's Game*/

	CarloTree * 					ct_root;				/*!<Indicates CarloTree root*/
	CarloTree * 					ct_father;				/*!<Indicates CarloTree father node*/
	Coordinates 					ct_sonId;				/*!<Indactes one CarloTree son id for its father*/

	unsigned int					ct_maxSimulationNumber;	/*!<Indicates the maximum of simulation number authorized*/

	bool							ct_isExpandable;		/*!<Indicates if a node and all its sons can be expandable*/

	/*!
	 * \brief selects the best leaf to expand
	 * \return the best leaf node
	 */
	CarloTree selection();

	/**
	 * \brief expands the selected leaf
	 */
	void expansion();

	/**
	 * \brief simulates the game for the selected leaf
	 */
	void simulation();

	/**
	 * \brief backpropagates the score
	 */
	void backPropagation();

	/*!
	 * \brief only recursive function, changes the nodes root
	 */
	void changeRoot(CarloTree * new_root);

	/*!
	 * \brief set current CarloTree as root
	 */
	void setAsRoot();

	/*!
	 * \brief get the last played moves by getting up the tree
	 * \return list of Coordinates
	 */
	vector<Coordinates> getLastMoves();

	/*!
	 * \brief this function returns a copy of the Game corresponding to its node
	 * \return a Game pointer (don't forget to delete it)
	 */
	Game * getGameCopy();

	/*!
	 * \return True if the CarloTree can not be expanded anymore
	 */
	bool isExpandable();

	/*!
	 * \brief picks a given number of different random indexes
	 * \param number : Random indexes number
	 * \param max : maximum value for the indexes
	 * \return a vector of number size filled with indexes from 0 to max
	 */
	static vector<unsigned int> pickRandomIndexes(unsigned int number, unsigned int max);
public:
	/*!
	 * \brief creates a choice Tree from a given Game and Team
	 * \param game : given Game
	 * \param team : given team
	 * \param root : indicates CarloTree root
	 */
	CarloTree(Game * game, unsigned int team, CarloTree * root = NULL, CarloTree * father = NULL, Coordinates son_id = Coordinates(0));
	/*!
	 * \brief CarloTree destructor
	 */
	~CarloTree();
	/*!
	 * \brief computes the best move
	 * \param maxSimulationNumber : maximum number of simulations
	 * \return the Coordinates of the best move
	 */
	Coordinates getBestMove(unsigned int maxSimulationNumber);

	/*!
	 * \brief moves the root to one of the root's son
	 * \param coordinates : Coordinates of the chosen son
	 * \return IATree list of next possible choices
	 */
	map<Coordinates, CarloTree *> changeRoot(vector<Coordinates> coordinates);

	/*!
	 * \brief displays the Tree
	 */
	void display();
};

#endif
