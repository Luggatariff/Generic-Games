/*!
 * \file Learner.cpp
 * \brief Learner code
 * \author Anaël Petit
 */

#include "Learner.hpp"
#include <cstdio>
#include <sstream>
#include <iostream>

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
		if (!string("move").compare(azColName[i])){
			move = string(argv[i]);
		}
		else if (!string("n_games").compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> games))
				games=0;
		}
		else if (!string("n_victories").compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> victories))
				victories=0;
		}
		else if (!string("n_defeats").compare(azColName[i])){
			string to_convert = argv[i];
			stringstream convert(to_convert);
			if ( !(convert >> defeats))
				defeats=0;
		}
		else if (!string("n_draws").compare(azColName[i])){
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

Learner::Learner(string name, unsigned int team_id, Player * teacher, string db_filename, bool always_learn){
	lea_name=name;
	lea_team=team_id;
	lea_teacher=teacher;
	lea_db_filename=db_filename;
	lea_db=NULL;
	lea_always_learn=always_learn;
}

void Learner::start(Game * game){
	lea_teacher->start(game);
	lea_played_moves.clear();

	char *zErrMsg = 0;

	int rc = sqlite3_open(lea_db_filename.c_str(), &lea_db);
	if( rc ){
	  cerr << "Can't open database: " << endl;
	}

	string sql=string("CREATE TABLE IF NOT EXISTS moves (game_name TEXT, player_name TEXT, pre_moves TEXT, move TEXT, n_games INTEGER DEFAULT 0, n_victories INTEGER DEFAULT 0, n_defeats INTEGER DEFAULT 0, n_draws INTEGER DEFAULT 0, PRIMARY KEY (game_name, player_name, pre_moves, move));");

	rc = sqlite3_exec(lea_db, sql.c_str(), callback, this, &zErrMsg);
	if( rc != SQLITE_OK ){
	  cerr << "SQL Error" << endl;
	}

}

int Learner::compute_score(MoveInfo * move_info){
	return (move_info->mi_victories * 2 - move_info->mi_draws - move_info->mi_defeats * 2);
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

		insert_sql_stream << "INSERT OR IGNORE INTO moves(game_name, player_name, pre_moves, move) ";
		insert_sql_stream << "VALUES(\"";
		insert_sql_stream << game_name;
		insert_sql_stream << "\",\"" << teacher_name;
		insert_sql_stream << "\",\"" << pre_moves;
		insert_sql_stream << "\",\"" << move;
		insert_sql_stream << "\");";
	}

	ostringstream select_sql_stream;
	select_sql_stream << "SELECT * FROM moves WHERE ";
	select_sql_stream << "game_name=\"" << game_name << "\" AND ";
	select_sql_stream << "player_name=\"" << teacher_name << "\" AND ";
	select_sql_stream << "pre_moves=\"" << pre_moves << "\";";

	char *zErrMsg = 0;

	int rc = sqlite3_exec(lea_db, insert_sql_stream.str().c_str(), callback, this, &zErrMsg);
	if( rc != SQLITE_OK ){
	  cerr << "SQL error:" << string(sqlite3_errmsg(lea_db)) << endl;
	}

	rc = sqlite3_exec(lea_db, select_sql_stream.str().c_str(), callback, this, &zErrMsg);
	if( rc != SQLITE_OK ){
	  cerr << "SQL error:" << string(sqlite3_errmsg(lea_db)) << endl;
	}

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

	string column_to_increment = "n_draws";
	if (game->isWon()){
		if (game->whoWon() == (int)this->getTeam()){
			column_to_increment = "n_victories";
		}
		else{
			column_to_increment = "n_defeats";
		}
	}

	ostringstream update_sql_stream;
	for (vector<MoveInfo *>::iterator played_moves_it = lea_played_moves.begin(); played_moves_it != lea_played_moves.end(); played_moves_it++){
		update_sql_stream << "UPDATE moves SET n_games = n_games + 1, ";
		update_sql_stream << column_to_increment << " = " << column_to_increment << " + 1 ";
		update_sql_stream << "WHERE game_name=\"" << game->getName() << "\" AND ";
		update_sql_stream << "player_name=\"" << lea_teacher->getName() << "\" AND ";
		update_sql_stream << "pre_moves=\"" << (*played_moves_it)->mi_pre_moves << "\" AND ";
		update_sql_stream << "move=\"" << (*played_moves_it)->mi_label << "\";";
	}

	char *zErrMsg = 0;
	int rc = sqlite3_exec(lea_db, update_sql_stream.str().c_str(), callback, this, &zErrMsg);
	if( rc != SQLITE_OK ){
	  cerr << "SQL error:" << string(sqlite3_errmsg(lea_db)) << endl;
	}

	for (vector<MoveInfo *>::iterator played_moves_it = lea_played_moves.begin(); played_moves_it != lea_played_moves.end(); played_moves_it++){
		delete *played_moves_it;
	}

	sqlite3_close(lea_db);
}

string Learner::getName(){
	return lea_name;
}

unsigned int Learner::getTeam(){
	return lea_team;
}
