#include <iostream>
#include "TicTacToe.hpp"
#include "FourInALine.hpp"
#include "Sticks.hpp"
#include "Draughts.hpp"
#include "Human.hpp"
#include "IA.hpp"
#include "Learner.hpp"

#define PLAYERS_NUMBER 2

#define INFINITE_LOOP true

int main( int argamec, const char* argamev[] ){
	vector<Player *> players(PLAYERS_NUMBER);
	Player * first_teacher = new IA(string("Octave"), 0, 1);
	Player * second_teacher = new IA(string("Octave"), 1, 1);
	players[0] = new Human(string("AdA"), 0);
	players[1] = new Human(string("AdA"), 0);

	Game * game;
	game = new Draughts(players[0], players[1]);
	while(true){
		game->start();
		for (unsigned int i_p = 0; i_p < players.size(); i_p++)
			players[i_p]->start(game);

		while (!game->isEnded()){
			game->display();

			vector<Coordinates> playable_moves = game->playableCoordinates();
			for (vector<Coordinates>::iterator it = playable_moves.begin(); it != playable_moves.end(); it++){
				it->display();
			}
			cerr<<endl;

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
			char commande;
			cout<<"Continue? (o/n):";
			cin>>commande;
			if (commande == 'n') break;
		}
	}
	delete game;
	for (unsigned int i_p = 0; i_p < players.size(); i_p++)
		delete players[i_p];
	delete first_teacher;
	delete second_teacher;
}
