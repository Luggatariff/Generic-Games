/*!
 * \file Carlo.cpp
 * \brief Carlo code
 * \author Anaël Petit
 */

#include <Carlo/Carlo.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Carlo::Carlo(string name, unsigned int teamId, unsigned int maxSimulationNumber, bool displayTree){
	c_name = name;
	c_team = teamId;

	c_maxSimulationNumber = maxSimulationNumber;
	c_tree = NULL;
	c_displayTree = displayTree;
}

void Carlo::start(Game * game){
	if (c_tree != NULL){
		delete c_tree;
		c_tree = NULL;
	}
}

void Carlo::end(Game * game){
	if (c_tree != NULL){
		delete c_tree;
		c_tree = NULL;
	}
}

Coordinates Carlo::play(Game * game , vector<Coordinates> limit_choices){
	Coordinates result;

	if (c_tree != NULL){
		vector<Coordinates> game_last_moves = game->lastMoves();
		vector<Coordinates> each_player_last_move;
		for (unsigned int i_p = game_last_moves.size() - (game->players().size() - 1); i_p < game_last_moves.size(); i_p++){
			each_player_last_move.push_back(game_last_moves[i_p]);
		}
		c_tree = c_tree->changeRoot(each_player_last_move);
	}

	if (c_tree == NULL){
		c_tree = new CarloTree(game, c_team);
	}

	return c_tree->getBestMove(c_maxSimulationNumber);
}

string Carlo::getName(){
	return c_name;
}

unsigned int Carlo::getTeam(){
	return c_team;
}
