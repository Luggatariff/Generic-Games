#include <iostream>
#include "TicTacToe.hpp"
#include "Human.hpp"

int main( int argamec, const char* argamev[] )
{
	while(true){
		Player * player_one = new Human();
		Player * player_two = new Human();
		Game * game;
		game = new TicTacToe(player_one, player_two);
		game->start();
		game->display();

		Player * winner = game->whoWon();
		if (winner != NULL)
			if ((winner == player_one))
				cout<<"Player One Wins!"<<endl;
			else
				cout<<"Player Two Wins!"<<endl;
		else{
			cout<<"Draw!"<<endl;
		}

		delete game;
		delete player_one;
		delete player_two;
		char commande;
		cout<<"Continuer? (o/n):";
		cin>>commande;
		if (commande == 'n')
			break;
	}
}
