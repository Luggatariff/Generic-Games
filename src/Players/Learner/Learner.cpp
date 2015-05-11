/*!
 * \file Learner.cpp
 * \brief Learner code
 * \author Anaël Petit
 */

#include "Learner.hpp"
#include <sqlite3.h>

Learner::Learner(string name, Player * teacher, string db_filename){
	lea_name=name;
	lea_teacher=teacher;
	lea_db=db_filename;
}

void Learner::start(Game * game){
	lea_teacher->start(game);

	sqlite3 *mydb;

	cout << sqlite3_open(lea_db.c_str(), &mydb) << endl;
	cout << sqlite3_close(mydb) << endl;

}

Coordinates Learner::play(Game * game){
	return lea_teacher->play(game);
}

string Learner::getName(){
	return lea_name;
}
