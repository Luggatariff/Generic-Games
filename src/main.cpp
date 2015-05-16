#include <iostream>
#include "TicTacToe.hpp"
#include "FourInALine.hpp"
#include "Sticks.hpp"
#include "Human.hpp"
#include "IA.hpp"
#include "Learner.hpp"

#define PLAYERS_NUMBER 2

int main( int argamec, const char* argamev[] ){
	vector<Player *> players(PLAYERS_NUMBER);
	Player * first_teacher = new IA(string("Octave"), 0, 1);
	Player * second_teacher = new IA(string("Fabien"), 1, 1);
	players[0] = new Learner(string("Eugenie"), 0, first_teacher, string("out/learner.db"), 1, -5, -10);
	players[1] = new Learner(string("Amelie"), 1, second_teacher, string("out/learner.db"), 1, -5, -10);

	Game * game;
	game = new FourInALine(players[0], players[1]);
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

		//char commande;
		//cout<<"Continue? (o/n):";
		//cin>>commande;
		//if (commande == 'n') break;
	}
	delete game;
	for (unsigned int i_p = 0; i_p < players.size(); i_p++)
		delete players[i_p];
	delete first_teacher;
	delete second_teacher;
}
