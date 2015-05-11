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
	Player * lea_teacher = new IA(string("Octave"), 2);
	players[1] = new Learner(string("Eugenie"), lea_teacher);
	players[0] = new Human(string("Fabien"));

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

		Player * winner = game->whoWon();
		if (winner != NULL){
			unsigned int i_p;
			for (i_p = 0; i_p < players.size(); i_p++){
				if ((winner == players[i_p])){
					cout<<players[i_p]->getName()<<" Wins!"<<endl;
					break;
				}
			}
		}
		else{
			cout<<"Draw!"<<endl;
		}

		char commande;
		cout<<"Continue? (o/n):";
		cin>>commande;
		if (commande == 'n')
			break;
	}
	delete game;
	for (unsigned int i_p = 0; i_p < players.size(); i_p++)
		delete players[i_p];
	delete lea_teacher;
}
