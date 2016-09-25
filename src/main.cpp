#include <iostream>
#include "TicTacToe.hpp"
#include "FourInALine.hpp"
#include "Sticks.hpp"
#include "Draughts.hpp"
#include "Chess.hpp"
#include "Human.hpp"
#include "IA.hpp"
#include "Carlo.hpp"
#include "Learner.hpp"

#define PLAYERS_NUMBER 2

#define INFINITE_LOOP true

int main( int argamec, const char* argamev[] ){
	vector<Player *> players(PLAYERS_NUMBER);
	Player * first_teacher = new Carlo(string("Solene"), 0, 0);
	Player * second_teacher = new Carlo(string("Lustu"), 1, 2);
	players[0] = new Carlo(string("Solene"), 0, 100); //new Learner(string("Triptonik"), 0, first_teacher, "learner1.db");
	players[1] = new Carlo(string("Lustu"), 1, 100); //new Learner(string("AdA"), 1, second_teacher, "learner2.db");

	Game * game;
	game = new TicTacToe(players[0], players[1]);
	while(true){
		game->start();
		for (unsigned int i_p = 0; i_p < players.size(); i_p++)
			players[i_p]->start(game);

		while (!game->isEnded()){
			game->display();
			cout << game->nextPlayer()->getName() << " Turn to play " << game->getName() << "..." << endl;
			Coordinates move = game->nextPlayer()->play(game);
			if (game->isPlayable(move)){
				game->play(move);
			}
		}
		game->display();

		int winner = game->whoWon();
		if (winner != -1){
			unsigned int i_p;
			for (i_p = 0; i_p < players.size(); i_p++){
				if (((unsigned int)winner == players[i_p]->getTeam())){
					cout<<players[i_p]->getName()<<" Wins!"<<endl;
					break;
				}
			}
		}
		else{
			cout<<"Draw!"<<endl;
		}

		for (unsigned int i_p = 0; i_p < players.size(); i_p++)
			players[i_p]->end(game);

		if (!INFINITE_LOOP){
			break;
		}
	}
	delete game;
	for (unsigned int i_p = 0; i_p < players.size(); i_p++)
		delete players[i_p];
	delete first_teacher;
	delete second_teacher;
}
