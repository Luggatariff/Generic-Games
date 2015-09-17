/*!
 * \file Learner.hpp
 * \brief Learner description
 * \author Anaël Petit
 */

#ifndef LEARNER_HPP
#define LEARNER_HPP

#include "../Player.hpp"
#include "../../../lib/sqlite-amalgamation-3081001/sqlite3.h"
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

	bool lea_always_learn;				/*!<Indicates if Lerner must always asks its teacher to play*/

	map<string, MoveInfo *> lea_moves_info;	/*!<Playable moves info*/
	vector<MoveInfo *> lea_played_moves;	/*!<Played moves info*/

	int lea_victory_coefficient;	/*<Coefficient determining the importance of a victory*/
	int lea_defeat_coefficient;		/*<Coefficient determining the importance of a defeat*/
	int lea_draw_coefficient;		/*<Coefficient determining the importance of a draw game*/

	/*
	 * \brief computes the score of a move from its information
	 * \param move_info move information
	 * \return score
	 */
	int compute_score(MoveInfo * move_info);

	/*!
	 * \brief sends an sql requests to the database
	 * \param request : string containing the sql request
	 */
	void sql_request(string request);
	/*!
	 * \brief opens the database session
	 */
	void sql_open();
	/*!
	 * \brief closes the database session
	 */
	void sql_close();
public:
	/*!
	 * \brief Learner constructor
	 * \param name : Player name
	 */
	Learner(string name, unsigned int team_id, Player * teacher, string db_filename, int victory_coefficient = 1, int defeat_coefficient = -1, int draw_coefficient = 0, bool always_learn = false);

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());
	string getName();
	unsigned int getTeam();

	/*!
	 * \brief Learner callback
	 */
	int learnerCallback (int argc, char **argv, char **azColName);
};

#endif
