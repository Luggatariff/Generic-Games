/*!
 * \file Carlo.hpp
 * \brief Carlo description
 * \author Anaël Petit
 */

#ifndef CARLO_HPP
#define CARLO_HPP

#include <Carlo/CarloTree.hpp>
#include "Player.hpp"

/*!
 * \class Carlo
 * \brief this class represents a Monte Caro IA Player
 */
class Carlo: public Player{
	string c_name;							/*!<Player name*/
	unsigned int c_team;					/*!<Team id*/
	unsigned int c_max_free_choices;		/*!<Define the number of free choices let to the Carlo*/
	CarloTree * c_tree;						/*!<Carlo Choice Tree*/
	bool ia_display_tree;					/*!<indicates if Carlo must display the choice Tree (debug)*/
	bool ia_display_messages;				/*!<indicates if Carlo must display flags*/
public:
	/*!
	 * \brief Carlo constructor
	 * \param display_tree : indicates Carlo must display the choice Tree (debug)
	 */
	Carlo(string name, unsigned int team_id, unsigned int max_free_choices=0, bool display_messages=true, bool display_tree = false);

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());
	string getName();
	unsigned int getTeam();
};

#endif
