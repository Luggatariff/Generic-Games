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
    unsigned int 	s_simulationNumber;				/*!<Number of simulations made for this node and its children*/
	unsigned int	s_winNumber;					/*!<Number of wins for this nodes and its children*/
	unsigned int	s_drawNumber;					/*!<Number of draws for this nodes and its children*/
	unsigned int	s_defeatNumber;					/*!<Number of defeats for this nodes and its children*/
	unsigned int	s_unfinishedNumber;				/*!<Number of unfinished games for this node and its children*/
	int				s_explorationParameter;			/*!<Coefficient giving the importance of expanding the tree*/

    int             s_gameScore;                    /*!<Score given by the Game instance*/

    int             s_winValue;                     /*!<Value of a victory in the score computation*/
    int             s_defeatValue;                  /*!<Value of a defeat in the score computation*/
    int             s_drawValue;                    /*!<Value of a draw game in the score computation*/
    int             s_unfinishedValue;              /*!<Value of a unfinished game in the score computation*/
public:
	/*!
	 * \brief Score constructor, from an exploration parameter
	 * \param explorationParameter : Score exploration parameter
     * \param winValue : Value of a victory in the score computation
     * \param defeatValue : Value of a defeat in the score computation
     * \param drawValue : Value of a draw game in the score computation
     * \param unfinishedValue : Value of a unfinished game in the score computation
	 */
    CarloScore(int explorationParameter, int winValue, int defeatValue, int drawValue, int unfinishedValue);

	/*!
	 * \brief gets score computed value
	 */
    double computeScore(int totalSimulations);

	/*!
	 * \brief compute the final score without exploration parameter
	 */
    virtual double computeFinalScore();

	/*!
	 * \brief increases SimulationNumber
	 */
	void newSimulation();

	/*!
	 * \brief signals a win
	 */
	void newWin();

	/*!
	 * \brief signals a draw
	 */
	void newDraw();

	/*!
	 * \brief signals a defeat
	 */
	void newDefeat();

	/*!
	 * \brief signals an unfinished
	 */
	void newUnfinished();

	/*!
	 * \brief gets the simulation Number
	 * \return simulation number
	 */
	unsigned int getSimulationNumber();

	/*!
	 * \brief gets the win Number
	 * \return win number
	 */
	unsigned int getWinNumber();

	/*!
	 * \brief gets the win Number
	 * \return win number
	 */
	unsigned int getDrawNumber();

	/*!
	 * \brief gets the defeat Number
	 * \return defeat number
	 */
	unsigned int getDefeatNumber();

	/*!
	 * \brief gets the unfinished Number
	 * \return unfinished number
	 */
	unsigned int getUnfinishedNumber();


	/*!
	 * \brief cancels the given number of wins and replaces it by defeats
	 */
	void cancelWins(unsigned int winsToCancel);

	/*!
	 * \brief cancels the given number of draws and replaces it by wins or defeats, depending on the given boolean
	 */
	void cancelDraws(unsigned int drawsToCancel, bool replaceWithWins);

	/*!
	 * \brief cancels the given number of unfinished and replaces it by wins or defeats, depending on the given boolean
	 */
	void cancelUnfinished(unsigned int unfinishedToCancel, bool replaceWithWins);

	/*!
	 * \brief cancels the given number of defeats and replaces it by wins
	 */
	void cancelDefeats(unsigned int defeatsToCancel);

    /*!
     * \brief sets the Game Score (score given par the Game instance)
     */
    void setGameScore(int gameScore);

    /*!
     * \brief gets the Game Score (score given par the Game instance)
     */
    int getGameScore();
};

/*!
 * \class CarloTree
 * \brief Tree used to compute Carlo choices
 */
class CarloTree{
private:
	Game * 							ct_game;				/*!<Game associated to the node*/

	unsigned int 					ct_team;				/*!<Team for which the CarloTree is playing*/

	map<Coordinates, CarloTree *>	ct_sons;				/*!<Map of sons, identified by the Coordinates of the move between this Game and the father's Game*/

	CarloTree * 					ct_father;				/*!<Indicates CarloTree father node*/
	Coordinates 					ct_sonId;				/*!<Indactes one CarloTree son id for its father*/

	bool							ct_isExpandable;		/*!<Indicates if a node and all its sons can be expandable*/

    int                             ct_winValue;            /*!<Value of a victory in the score computation*/
    int                             ct_defeatValue;         /*!<Value of a defeat in the score computation*/
    int                             ct_drawValue;           /*!<Value of a draw game in the score computation*/
    int                             ct_unfinishedValue;     /*!<Value of a unfinished game in the score computation*/

	/*!
	 * \brief selects the best leaf to expand
	 * \return the best leaf node
	 */
	CarloTree * selectionAndExpansion();

	/**
	 * \brief simulates the game for the selected leaf
	 */
	void simulation();

	/**
	 * \brief backpropagates the score
	 */
	void backPropagation();

	/*!
	 * \brief set current CarloTree as root
	 */
	void setAsRoot();

	/*!
         * \brief get the last played moves and random events by getting up the tree
	 * \return list of Coordinates
	 */
        vector<Coordinates> getLastMovesAndRandomEvents();

	/*!
	 * \brief this function returns a copy of the Game corresponding to its node
	 * \return a Game pointer (don't forget to delete it)
	 */
	Game * getGameCopy();

	/*!
	 * \brief picks a given number of different random indexes
	 * \param number : Random indexes number
	 * \param max : maximum value for the indexes
	 * \return a vector of number size filled with indexes from 0 to max
	 */
	static vector<unsigned int> pickRandomIndexes(unsigned int number, unsigned int max);

	/**
	 * \brief gets all the descendants of a node
	 * \return vector containing all the descendants of a node
	 */
	vector<CarloTree *> getAllDescendants();

	/**
	 * \brief picks a move beneath the sons
	 * \param finalComputation : indicates if the score final computation is used
	 * \return chosen move
	 */
        Coordinates pickAMoveOrARandomEvent(bool finalComputation);

	/**
	 * \brief gets unplayed moves for a node
	 * \return unplayed moves
	 */
    vector<Coordinates> getUnplayedMovesOrRandomEvents();

protected:
    CarloScore * 		ct_score;                           /*!<CarloTree node Score*/
    int					ct_explorationParameter;            /*!<Exploration parameter for score computation*/

    CarloTree * 		ct_root;                            /*!<Indicates CarloTree root*/
    vector<Coordinates> ct_playableMovesOrRandomEvents;		/*!<Playable moves or random events*/

    /*!
     * \brief fills the list of random events or playable moves
     * \param game : given Game
     */
    virtual void fillPlayableMovesOrRandomEvents(Game * game);

    /*!
     * \brief creates a choice Tree from a given Game and Team (used to create sons)
     * \param game : given Game
     * \param team : given team
     * \param root : indicates CarloTree root
     */
    virtual CarloTree * createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue);

    virtual void customSimulation(Game * game);

    virtual void customBackPropagation(CarloTree * ancestor);

public:
	/*!
	 * \brief creates a choice Tree from a given Game and Team
	 * \param game : given Game
	 * \param team : given team
	 * \param root : indicates CarloTree root
	 */
    CarloTree(Game * game, unsigned int team, CarloTree * root = NULL, CarloTree * father = NULL, Coordinates sonId = Coordinates(0), int winValue = 10, int defeatValue = 0, int drawValue = 2, int unfinishedValue = 1);
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
	 * \return new CarloTree root
	 */
	CarloTree * changeRoot(vector<Coordinates> coordinates);

	/*!
	 * \brief displays the Tree
	 */
	void display();

    CarloScore * getScore();
};

#endif
