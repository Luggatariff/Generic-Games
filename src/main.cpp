#include <iostream>
#include "TicTacToe.hpp"
#include "FourInALine.hpp"
#include "Human.hpp"
#include "IA.hpp"

#define PLAYERS_NUMBER 2

int main( int argamec, const char* argamev[] )
{
	vector<Player *> players(PLAYERS_NUMBER);
	players[0] = new IA(3);
	players[1] = new IA(1);

	Game * game;
	game = new TicTacToe(players[0], players[1]);
	while(true){
		game->start();
		for (unsigned int i_p = 0; i_p < players.size(); i_p++)
			players[i_p]->start(game);

		while (!game->isEnded()){
			game->display();
			Coordinates move = game->nextPlayer()->play(game);
			if (game->isPlayable(move))
				game->play(move);
		}
		game->display();

		Player * winner = game->whoWon();
		if (winner != NULL){
			for (unsigned int i_p = 0; i_p < players.size(); i_p++)
				if ((winner == players[i_p]))
					cout<<"Player "<<i_p + 1<<" Wins!"<<endl;
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
}
