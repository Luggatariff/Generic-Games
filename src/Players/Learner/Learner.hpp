/*!
 * \file Learner.hpp
 * \brief Learner description
 * \author Anaël Petit
 */

#ifndef LEARNER_HPP
#define LEARNER_HPP

#include "Player.hpp"
#include <sqlite3.h>
#include "map"

/*!
 * \class MoveInfo
 * \brief Information about one of new moves
 */
class MoveInfo{
public:
	Coordinates mi_coordinates; /*!<Coordinates moves*/
	string mi_pre_moves;		/*!<pre moves*/
	string mi_label;			/*!<String depending on coordinates*/
	unsigned int mi_games;		/*!<Number of game the moves was played*/
	unsigned int mi_victories;	/*!<Number of victories*/
	unsigned int mi_defeats;	/*!<Number of defeats*/
	unsigned int mi_draws;		/*!<Number of draw games*/
};

/*!
 * \class Learner
 * \brief this class represents a Learner player
 */
class Learner: public Player{
	string lea_name;					/*!<Player name*/
	unsigned int lea_team;				/*!<Team id*/
	string lea_db_filename;				/*!<Database filename*/
	sqlite3 *lea_db;					/*!<sqlite3 database object*/
	Player * lea_teacher; 				/*!<Player to learn from*/

	map<string, MoveInfo *> lea_moves_info;	/*!<Playable moves info*/
	vector<MoveInfo *> lea_played_moves;		/*!<Played moves info*/
public:
	/*!
	 * \brief Learner constructor
	 * \param name : Player name
	 */
	Learner(string name, unsigned int team_id, Player * teacher, string db_filename);

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game);
	string getName();
	unsigned int getTeam();

	/*!
	 * \brief Learner callback
	 */
	int learnerCallback (int argc, char **argv, char **azColName);
};

#endif
