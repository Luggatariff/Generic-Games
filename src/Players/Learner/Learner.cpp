/*!
 * \file Learner.cpp
 * \brief Learner code
 * \author Anaël Petit
 */

#include "Learner.hpp"
#include <cstdio>
#include <sstream>
#include <iostream>

#define LEARNER_TABLE_NAME 		"moves"
#define LEARNER_COL_MOVE 		"move"
#define LEARNER_COL_PRE_MOVES 	"pre_moves"
#define LEARNER_COL_GAME		"game_name"
#define LEARNER_COL_PLAYER		"player_name"
#define LEARNER_COL_N_GAMES		"n_games"
#define LEARNER_COL_N_VICTORIES	"n_victories"
#define LEARNER_COL_N_DEFEATS	"n_defeats"
#define LEARNER_COL_N_DRAWS		"n_draws"

static int callback(void *learner, int argc, char **argv, char **azColName){
	Learner * lea=reinterpret_cast<Learner*>(learner);
	return lea->learnerCallback(argc, argv, azColName);
}

int Learner::learnerCallback(int argc, char **argv, char **azColName){
	string move;
	unsigned int games;
	unsigned int victories;
	unsigned int defeats;
	unsigned int draws;
	for(int i=0; i<argc; i++){
		if (!string(LEARNER_COL_MOVE).compare(azColName[i])){
			move = string(argv[i]);
		}
		else if (!string(LEARNER_COL_N_GAMES).compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> games))
				games=0;
		}
		else if (!string(LEARNER_COL_N_VICTORIES).compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> victories))
				victories=0;
		}
		else if (!string(LEARNER_COL_N_DEFEATS).compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> defeats))
				defeats=0;
		}
		else if (!string(LEARNER_COL_N_DRAWS).compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> draws))
				draws=0;
		}
	}
	MoveInfo * move_info = lea_moves_info[move];
	move_info->mi_games = games;
	move_info->mi_victories = victories;
	move_info->mi_defeats = defeats;
	move_info->mi_draws = draws;
	return 0;
}

Learner::Learner(string name, unsigned int team_id, Player * teacher, string db_filename, int victory_coefficient, int defeat_coefficient, int draw_coefficient, bool always_learn){
	lea_name=name;
	lea_team=team_id;

	lea_teacher=teacher;
	lea_always_learn=always_learn;

	lea_db_filename=db_filename;
	lea_db=NULL;

	lea_victory_coefficient=victory_coefficient;
	lea_defeat_coefficient=defeat_coefficient;
	lea_draw_coefficient=draw_coefficient;
}

void Learner::sql_open(){
	if (lea_db == NULL){
		int rc = sqlite3_open(lea_db_filename.c_str(), &lea_db);
		if( rc ){
		  cerr << "Can't open database: " << endl;
		}
	}
}

void Learner::sql_close(){
	if (lea_db != NULL){
		sqlite3_close(lea_db);
		lea_db = NULL;
	}
}

void Learner::sql_request(string request){
	bool lea_db_was_null = false;
	if (lea_db == NULL){
		sql_open();
		lea_db_was_null = true;
	}
	char *zErrMsg = 0;
	int rc = sqlite3_exec(lea_db, request.c_str(), callback, this, &zErrMsg);
	if( rc != SQLITE_OK ){
	  cerr << "SQL Error" << string(sqlite3_errmsg(lea_db)) << endl;
	}
	if (lea_db_was_null){
		sql_close();
	}
}

void Learner::start(Game * game){
	lea_teacher->start(game);
	lea_played_moves.clear();

	ostringstream create_table_stream;
	create_table_stream << "CREATE TABLE IF NOT EXISTS ";
	create_table_stream << LEARNER_TABLE_NAME << " (";
	create_table_stream << LEARNER_COL_GAME << " TEXT, ";
	create_table_stream << LEARNER_COL_PLAYER << " TEXT, ";
	create_table_stream << LEARNER_COL_PRE_MOVES << " TEXT, ";
	create_table_stream << LEARNER_COL_MOVE << " TEXT, ";
	create_table_stream << LEARNER_COL_N_GAMES << " INTEGER DEFAULT 0, ";
	create_table_stream << LEARNER_COL_N_VICTORIES << " INTEGER DEFAULT 0, ";
	create_table_stream << LEARNER_COL_N_DEFEATS << " INTEGER DEFAULT 0, ";
	create_table_stream << LEARNER_COL_N_DRAWS << " INTEGER DEFAULT 0, ";
	create_table_stream << "PRIMARY KEY (" << LEARNER_COL_GAME << "," << LEARNER_COL_PLAYER << "," << LEARNER_COL_PRE_MOVES << "," << LEARNER_COL_MOVE << ")";
	create_table_stream << ");";

	sql_request(create_table_stream.str());
}

int Learner::compute_score(MoveInfo * move_info){
	return (move_info->mi_victories * lea_victory_coefficient + move_info->mi_draws * lea_draw_coefficient + move_info->mi_defeats * lea_defeat_coefficient);
}

Coordinates Learner::play(Game * game, vector<Coordinates> limit_choices){
	string game_name=game->getName();
	string teacher_name=lea_teacher->getName();

	ostringstream pre_moves_stream;
	vector<Coordinates> last_moves=game->lastMoves();
	for (vector<Coordinates>::iterator last_moves_it = last_moves.begin(); last_moves_it != last_moves.end(); last_moves_it++){
		for (unsigned int coordinates_index=0; coordinates_index < last_moves_it->dimension(); coordinates_index++){
			pre_moves_stream << last_moves_it->operator [](coordinates_index);
		}
	}
	string pre_moves=pre_moves_stream.str();

	ostringstream insert_sql_stream;

	vector<Coordinates> playable_moves;
	if (limit_choices.size() <= 1)
		playable_moves = game->playableCoordinates();
	else
		playable_moves = limit_choices;
	for (vector<Coordinates>::iterator playable_moves_it = playable_moves.begin(); playable_moves_it != playable_moves.end(); playable_moves_it++){
		MoveInfo  * move_info = new MoveInfo();
		move_info->mi_pre_moves=pre_moves;
		move_info->mi_coordinates=*playable_moves_it;

		ostringstream move_stream;
		for (unsigned int coordinates_index=0; coordinates_index < playable_moves_it->dimension(); coordinates_index++){
			move_stream << playable_moves_it->operator [](coordinates_index);
		}
		string move=move_stream.str();
		move_info->mi_label=move;

		lea_moves_info.insert(pair<string, MoveInfo *>(move, move_info));

		insert_sql_stream << "INSERT OR IGNORE INTO ";
		insert_sql_stream << LEARNER_TABLE_NAME << "(";
		insert_sql_stream << LEARNER_COL_GAME << ",";
		insert_sql_stream << LEARNER_COL_PLAYER << ",";
		insert_sql_stream << LEARNER_COL_PRE_MOVES << ",";
		insert_sql_stream << LEARNER_COL_MOVE;
		insert_sql_stream << ") ";
		insert_sql_stream << "VALUES(\"";
		insert_sql_stream << game_name;
		insert_sql_stream << "\",\"" << teacher_name;
		insert_sql_stream << "\",\"" << pre_moves;
		insert_sql_stream << "\",\"" << move;
		insert_sql_stream << "\");";
	}

	ostringstream select_sql_stream;
	select_sql_stream << "SELECT * FROM ";
	select_sql_stream << LEARNER_TABLE_NAME << " WHERE ";
	select_sql_stream << LEARNER_COL_GAME << "=\"" << game_name << "\" AND ";
	select_sql_stream << LEARNER_COL_PLAYER << "=\"" << teacher_name << "\" AND ";
	select_sql_stream << LEARNER_COL_PRE_MOVES << "=\"" << pre_moves << "\";";

	sql_open();

	sql_request(insert_sql_stream.str());

	sql_request(select_sql_stream.str());

	sql_close();

	Coordinates result;

	map<string, MoveInfo *>::iterator moves_info_it = lea_moves_info.begin();
	int best_score=compute_score(moves_info_it->second);
	vector<MoveInfo *> best_moves;
	best_moves.push_back(moves_info_it->second);

	for (moves_info_it++; moves_info_it != lea_moves_info.end(); moves_info_it++){
		int score=compute_score(moves_info_it->second);
		if (score > best_score){
			best_score=score;
			best_moves.clear();
			best_moves.push_back(moves_info_it->second);
		}
		else if (score == best_score){
			best_moves.push_back(moves_info_it->second);
		}
	}
	if (lea_always_learn || best_moves.size() > 1){
		vector<Coordinates> best_moves_coordinates;
		for (vector<MoveInfo *>::iterator best_moves_it = best_moves.begin(); best_moves_it != best_moves.end(); best_moves_it++){
			best_moves_coordinates.push_back((*best_moves_it)->mi_coordinates);
		}
		result = lea_teacher->play(game, best_moves_coordinates);
	}
	else{
		result = best_moves[0]->mi_coordinates;
		cout<<"Learner chose:";
		for (unsigned int dim = 0; dim < game->dimension(); dim++)
			cout<<result[dim]<<" ";
		cout<<endl;
		cout << "Score :" << best_score << endl;
	}

	ostringstream played_move_stream;
	for (unsigned int coordinates_index=0; coordinates_index < result.dimension(); coordinates_index++){
		played_move_stream << result[coordinates_index];
	}
	string played_move=played_move_stream.str();

	MoveInfo * move_info_result=lea_moves_info[played_move];
	lea_played_moves.push_back(move_info_result);

	for(map<string, MoveInfo *>::iterator moves_info_it = lea_moves_info.begin(); moves_info_it != lea_moves_info.end(); moves_info_it++){
		if (moves_info_it->second != move_info_result){
			delete moves_info_it->second;
		}
	}

	lea_moves_info.clear();

	return result;
}

void Learner::end(Game * game){
	lea_teacher->end(game);

	string column_to_increment = LEARNER_COL_N_DRAWS;
	if (game->isWon()){
		if (game->whoWon() == (int)this->getTeam()){
			column_to_increment = LEARNER_COL_N_VICTORIES;
		}
		else{
			column_to_increment = LEARNER_COL_N_DEFEATS;
		}
	}

	ostringstream update_sql_stream;
	for (vector<MoveInfo *>::iterator played_moves_it = lea_played_moves.begin(); played_moves_it != lea_played_moves.end(); played_moves_it++){
		update_sql_stream << "UPDATE ";
		update_sql_stream << LEARNER_TABLE_NAME;
		update_sql_stream << " SET " << LEARNER_COL_N_GAMES << " = " << LEARNER_COL_N_GAMES << " + 1, ";
		update_sql_stream << column_to_increment << " = " << column_to_increment << " + 1 ";
		update_sql_stream << "WHERE ";
		update_sql_stream << LEARNER_COL_GAME << "=\"" << game->getName() << "\" AND ";
		update_sql_stream << LEARNER_COL_PLAYER << "=\"" << lea_teacher->getName() << "\" AND ";
		update_sql_stream << LEARNER_COL_PRE_MOVES << "=\"" << (*played_moves_it)->mi_pre_moves << "\" AND ";
		update_sql_stream << LEARNER_COL_MOVE << "=\"" << (*played_moves_it)->mi_label << "\";";
	}

	sql_request(update_sql_stream.str());

	for (vector<MoveInfo *>::iterator played_moves_it = lea_played_moves.begin(); played_moves_it != lea_played_moves.end(); played_moves_it++){
		delete *played_moves_it;
	}
}

string Learner::getName(){
	return lea_name;
}

unsigned int Learner::getTeam(){
	return lea_team;
}
