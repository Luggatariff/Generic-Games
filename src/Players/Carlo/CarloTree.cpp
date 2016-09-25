/*!
 * \file CarloTree.cpp
 * \brief CarloTree code
 * \author Ana�l Petit
 */

#include <Carlo/CarloTree.hpp>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <math.h>

CarloTree::CarloTree(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId){
	this->ct_team = team;
	this->ct_isExpandable = true;
	this->ct_explorationParameter = sqrt(2);
	this->ct_score = new CarloScore(ct_explorationParameter);

	if (root == NULL)
		this->ct_root = this;
	else
		this->ct_root = root;

	if (ct_root == this){
		this->ct_father = NULL;
		this->ct_game = game;
		this->ct_playableMoves = ct_game->playableCoordinates();
	}
	else{
		this->ct_father = father;
		this->ct_sonId = sonId;
		this->ct_game = NULL;
	}
}

CarloTree::~CarloTree(){
	delete ct_score;
	if (ct_root == this){
		delete ct_game;
		vector<CarloTree *> allDescendants = getAllDescendants();
		for(vector<CarloTree *>::iterator descendantsIterator = allDescendants.begin(); descendantsIterator != allDescendants.end(); ++descendantsIterator){
			if ((*descendantsIterator)->ct_root == this)
				delete *descendantsIterator;
		}
	}
}

vector<CarloTree *> CarloTree::getAllDescendants(){
	vector<CarloTree *> result;
	for(map<Coordinates, CarloTree *>::iterator son_iterator = ct_sons.begin(); son_iterator != ct_sons.end(); ++son_iterator){
		result.push_back(son_iterator->second);
	}
	unsigned int nextIndex = 0;
	while (nextIndex < result.size()){
		CarloTree * sonNode = result[nextIndex];
		for(map<Coordinates, CarloTree *>::iterator son_iterator = sonNode->ct_sons.begin(); son_iterator != sonNode->ct_sons.end(); ++son_iterator){
			result.push_back(son_iterator->second);
		}
		nextIndex++;
	}
	return result;
}

vector<unsigned int> CarloTree::pickRandomIndexes(unsigned int number, unsigned int max){
	vector<unsigned int> result;
	unsigned int true_number=number;
	if (max < number){
		true_number=max;
	}
	srand (time(NULL));
	for (unsigned int index=0; index<true_number; index++){
		unsigned int random_index=0;
		random_index = rand() % (max);
		if (find(result.begin(), result.end(), random_index) == result.end()){
			result.push_back(random_index);
		}
		else{
			index--;
		}
	}
	return result;
}

vector<Coordinates> CarloTree::getLastMoves(){
	vector<Coordinates> result;
	if (ct_root != this){
		result = ct_father->getLastMoves();
		result.push_back(ct_sonId);
	}
	return result;
}

Game * CarloTree::getGameCopy(){
	Game * rootGameCopy = this->ct_root->ct_game->copy();
	vector<Coordinates> lastMoves = this->getLastMoves();
	for (vector<Coordinates>::iterator lastMovesIterator = lastMoves.begin(); lastMovesIterator != lastMoves.end(); ++lastMovesIterator){
		rootGameCopy->play(*lastMovesIterator);
	}
	return rootGameCopy;
}

Coordinates CarloTree::getBestMove(unsigned int maxSimulationNumber){
	for (unsigned int simulationIndex = 0; simulationIndex < maxSimulationNumber; ++simulationIndex){
		CarloTree * newNode = selectionAndExpansion();
		newNode->simulation();
		newNode->backPropagation();

		if (!ct_isExpandable)
			break;
	}
	return pickAMove(true, false);
}

Coordinates CarloTree::pickAMove(bool finalComputation, bool addUnplayedMoves){
	vector<Coordinates> bestMoves;
	if (ct_sons.empty()){
		bestMoves = ct_playableMoves;
	}
	else{
		double bestScore = (double)0.0;
		for(map<Coordinates, CarloTree *>::iterator son_iterator = ct_sons.begin(); son_iterator != ct_sons.end(); ++son_iterator){
			if (son_iterator->second->ct_isExpandable || finalComputation){
				double score;
				if (finalComputation)
					score = son_iterator->second->ct_score->computeFinalScore();
				else{
					score = son_iterator->second->ct_score->computeScore(ct_score->getSimulationNumber());
				}
				if (score > bestScore){
					bestMoves.clear();
				}
				if (score >= bestScore){
					bestMoves.push_back(son_iterator->first);
				}
			}
		}
		if (addUnplayedMoves){
			vector<Coordinates> unplayedMoves = getUnplayedMoves();
			for (vector<Coordinates>::iterator unplayedMovesIterator = unplayedMoves.begin(); unplayedMovesIterator != unplayedMoves.end(); ++unplayedMovesIterator){
				if (ct_sons.count(*unplayedMovesIterator) == 0){
					bestMoves.push_back(*unplayedMovesIterator);
				}
			}
		}
	}
	srand (time(NULL));
	unsigned int random_index = rand() % (bestMoves.size());
	return bestMoves[random_index];
}

vector<Coordinates> CarloTree::getUnplayedMoves(){
	vector<Coordinates> result;
	for (vector<Coordinates>::iterator playableMovesIterator = ct_playableMoves.begin(); playableMovesIterator != ct_playableMoves.end(); ++playableMovesIterator){
		if (ct_sons.count(*playableMovesIterator) == 0){
			result.push_back(*playableMovesIterator);
		}
	}
	return result;
}

CarloTree * CarloTree::selectionAndExpansion(){
	CarloTree * selected = this;
	Coordinates selectedMove = selected->pickAMove(false, true);

	while (selected->ct_sons.count(selectedMove) > 0){
		selected = selected->ct_sons[selectedMove];
		selectedMove = selected->pickAMove(false, true);
	}

	CarloTree * newSon = new CarloTree(NULL, ct_team, this, selected, selectedMove);
	selected->ct_sons.insert(pair<Coordinates, CarloTree *>(selectedMove, newSon));

	return newSon;
}

void CarloTree::simulation(){
	Game * game = getGameCopy();
	ct_playableMoves = game->playableCoordinates();
	ct_score->incSimulationNumber();
	if (game->isEnded() == true){
		ct_isExpandable = false;
		if (game->isWinner(ct_team)){
			ct_score->incWinNumber();
		}
	}
	delete game;
}

void CarloTree::backPropagation(){
	int win = ct_score->getWinNumber();
	bool sonIsExpandable = ct_isExpandable;
	CarloTree * father = ct_father;
	while (father != NULL){
		father->ct_score->incSimulationNumber();
		if (win > 0){
			father->ct_score->incWinNumber();
		}
		if (!sonIsExpandable){
			vector<Coordinates> unplayedMoves = father->getUnplayedMoves();
			if (unplayedMoves.empty()){
				bool aSonIsExpandable = false;
				for(map<Coordinates, CarloTree *>::iterator son_iterator = father->ct_sons.begin(); son_iterator != father->ct_sons.end(); ++son_iterator){
					aSonIsExpandable = aSonIsExpandable || son_iterator->second->ct_isExpandable;
					if (aSonIsExpandable)
						break;
				}
				father->ct_isExpandable = aSonIsExpandable;
			}
			sonIsExpandable = father->ct_isExpandable;
		}
		father = father->ct_father;
	}
}

void CarloTree::setAsRoot(){
	this->ct_game = this->getGameCopy();
	this->ct_father = NULL;
	vector<CarloTree *> allDescendants = getAllDescendants();
	for (vector<CarloTree *>::iterator descendantsIterator = allDescendants.begin(); descendantsIterator != allDescendants.end(); descendantsIterator++){
		(*descendantsIterator)->ct_root = this;
	}
}

CarloTree * CarloTree::changeRoot(vector<Coordinates> coordinates){
	CarloTree * newRoot = NULL;
	for (vector<Coordinates>::iterator coordinates_iterator = coordinates.begin(); coordinates_iterator != coordinates.end(); coordinates_iterator++){
		if (newRoot->ct_sons.empty()) break;
		try{
			newRoot = newRoot->ct_sons.at(*coordinates_iterator);
		}
		catch(...){
			newRoot = NULL;
			break;
		}
	}
	if (newRoot != NULL){
		newRoot->setAsRoot();
	}

	delete this;

	return newRoot;
}

void CarloTree::display(){
	cerr<<endl;
}

CarloScore::CarloScore(int explorationParameter){
	s_winNumber = 0;
	s_simulationNumber = 0;
	s_explorationParameter = explorationParameter;
}

void CarloScore::incSimulationNumber(){
	s_simulationNumber++;
}

void CarloScore::incWinNumber(){
	s_winNumber++;
}

double CarloScore::computeScore(int totalSimulations){
	if (s_simulationNumber == 0){
		return (double)-1.0;
	}
	return ((double)s_winNumber/(double)s_simulationNumber) + (double)s_explorationParameter*sqrt(log((double)totalSimulations)/(double)s_simulationNumber);
}

double CarloScore::computeFinalScore(){
	if (s_simulationNumber == 0){
		return (double)-1.0;
	}
	return (double)s_winNumber/(double)s_simulationNumber;
}

int CarloScore::getSimulationNumber(){
	return s_simulationNumber;
}

int CarloScore::getWinNumber(){
	return s_winNumber;
}
