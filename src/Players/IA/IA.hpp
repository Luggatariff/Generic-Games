/*!
 * \file IA.hpp
 * \brief IA description
 * \author Anaël Petit
 */

#ifndef IA_HPP
#define IA_HPP

#include "Player.hpp"
#include "IATree.hpp"

/*!
 * \class IA
 * \brief this class represents a Artificial Intelligence player
 */
class IA: public Player{
	string ia_name;							/*!<Player name*/
	unsigned int ia_team;					/*!<Team id*/
	unsigned int ia_level;					/*!<IA level of thinking*/
	unsigned int ia_max_free_choices;		/*!<Define the number of free choices let to the IA*/
	IATree * ia_tree;						/*!<IA Choice Tree*/
	bool ia_display_tree;					/*!<indicates if IA must display the choice Tree (debug)*/
	bool ia_display_messages;				/*!<indicates if IA must display flags*/
public:
	/*!
	 * \brief IA constructor
	 * \param level : IA thinking level
	 * \param display_tree : indicates if IA must display the choice Tree (debug)
	 */
	IA(string name, unsigned int team_id, unsigned int level, unsigned int max_free_choices=0, bool display_messages=true, bool display_tree = false);

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());
	string getName();
	unsigned int getTeam();
};

#endif
