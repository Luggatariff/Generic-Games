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
#include <float.h>

CarloTree::CarloTree(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue){
	this->ct_team = team;
	this->ct_isExpandable = true;
	this->ct_explorationParameter = sqrt(2);
    this->ct_score = new CarloScore(ct_explorationParameter, winValue, defeatValue, drawValue, unfinishedValue);

	if (root == NULL)
		this->ct_root = this;
	else
		this->ct_root = root;

	if (ct_root == this){
		this->ct_father = NULL;
		this->ct_game = game;
        fillPlayableMovesOrRandomEvents(game);
	}
	else{
		this->ct_father = father;
		this->ct_sonId = sonId;
		this->ct_game = NULL;
	}

    this->ct_winValue = winValue;
    this->ct_defeatValue = defeatValue;
    this->ct_drawValue = drawValue;
    this->ct_unfinishedValue = unfinishedValue;
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

vector<Coordinates> CarloTree::getLastMovesAndRandomEvents(){
	vector<Coordinates> result;
	if (ct_root != this){
        result = ct_father->getLastMovesAndRandomEvents();
		result.push_back(ct_sonId);
	}
	return result;
}

Game * CarloTree::getGameCopy(){
	Game * rootGameCopy = this->ct_root->ct_game->copy();
    vector<Coordinates> lastMovesAndRandomEvents = this->getLastMovesAndRandomEvents();
    for (vector<Coordinates>::iterator lastMovesAndRandomEventsIterator = lastMovesAndRandomEvents.begin(); lastMovesAndRandomEventsIterator != lastMovesAndRandomEvents.end(); ++lastMovesAndRandomEventsIterator){
        if (!rootGameCopy->waitingForRandomEvents()){
            rootGameCopy->playMove(*lastMovesAndRandomEventsIterator, false);
        }
        else{
            rootGameCopy->forceRandom(*lastMovesAndRandomEventsIterator);
        }
	}
	return rootGameCopy;
}

Coordinates CarloTree::getBestMove(unsigned int maxSimulationNumber){
	if (ct_isExpandable){
		for (unsigned int simulationIndex = 0; simulationIndex < maxSimulationNumber; ++simulationIndex){
			CarloTree * newNode = selectionAndExpansion();
			newNode->simulation();
			newNode->backPropagation();

			if (!ct_isExpandable)
				break;
		}
	}
    return pickAMoveOrARandomEvent(true);
}

Coordinates CarloTree::pickAMoveOrARandomEvent(bool finalComputation){
	vector<Coordinates> bestMoves;

	if (ct_sons.empty()){
        bestMoves = ct_playableMovesOrRandomEvents;
	}
	else{
        vector<Coordinates> unplayedMovesOrRandomEvents = getUnplayedMovesOrRandomEvents();
        if (!finalComputation && !unplayedMovesOrRandomEvents.empty()){
            bestMoves = unplayedMovesOrRandomEvents;
		}
		else{
			double bestScore;
			bool bestScoreNotComputed = true;
            int bestGameScore;
            bool bestGameScoreNotDefined = true;
			for(map<Coordinates, CarloTree *>::iterator son_iterator = ct_sons.begin(); son_iterator != ct_sons.end(); ++son_iterator){
				if (son_iterator->second->ct_isExpandable || finalComputation){
					double score;
                    if (finalComputation){
                        score = son_iterator->second->ct_score->computeFinalScore();
                    }
					else{
						score = son_iterator->second->ct_score->computeScore(ct_score->getSimulationNumber());
					}
                    int gameScore = son_iterator->second->ct_score->getGameScore();

					if (bestScoreNotComputed || score > bestScore){
                        bestMoves.clear();
						bestScore = score;
						bestScoreNotComputed = false;
                        bestGameScoreNotDefined = true;
					}
					if (score == bestScore){
                        if (bestGameScoreNotDefined || gameScore > bestGameScore){
                            bestMoves.clear();
                            bestGameScore = gameScore;
                            bestGameScoreNotDefined = false;
                        }
                        if (gameScore == bestGameScore){
                            bestMoves.push_back(son_iterator->first);
                        }
					}
				}
			}
		}
	}
	srand (time(NULL));
	unsigned int random_index = rand() % (bestMoves.size());
	return bestMoves[random_index];
}

vector<Coordinates> CarloTree::getUnplayedMovesOrRandomEvents(){
	vector<Coordinates> result;
    for (vector<Coordinates>::iterator playableMovesOrRandomEventsIterator = ct_playableMovesOrRandomEvents.begin(); playableMovesOrRandomEventsIterator != ct_playableMovesOrRandomEvents.end(); ++playableMovesOrRandomEventsIterator){
        if (ct_sons.count(*playableMovesOrRandomEventsIterator) == 0){
            result.push_back(*playableMovesOrRandomEventsIterator);
		}
	}
	return result;
}

CarloTree * CarloTree::selectionAndExpansion(){
	CarloTree * selected = this;
    Coordinates selectedMoveOrRandomEvent = selected->pickAMoveOrARandomEvent(false);

    while (selected->ct_sons.count(selectedMoveOrRandomEvent) > 0){
        selected = selected->ct_sons[selectedMoveOrRandomEvent];
        selectedMoveOrRandomEvent = selected->pickAMoveOrARandomEvent(false);
	}

    CarloTree * newSon = createSon(NULL, ct_team, this, selected, selectedMoveOrRandomEvent, ct_winValue, ct_defeatValue, ct_drawValue, ct_unfinishedValue);
    selected->ct_sons.insert(pair<Coordinates, CarloTree *>(selectedMoveOrRandomEvent, newSon));

	return newSon;
}

CarloTree * CarloTree::createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue){
    return new CarloTree(game, team, root, father, sonId, winValue, defeatValue, drawValue, unfinishedValue);
}

void CarloTree::simulation(){
	Game * game = getGameCopy();
    fillPlayableMovesOrRandomEvents(game);
	ct_score->newSimulation();
    ct_score->setGameScore(game->score(ct_team));
	if (game->isEnded() == true){
		ct_isExpandable = false;
		if (game->isWinner(ct_team)){
			ct_score->newWin();
		}
		else if (!game->isWon()){
			ct_score->newDraw();
		}
		else{
			ct_score->newDefeat();
		}
	}
	else{
		ct_score->newUnfinished();
	}
    customSimulation(game);
	delete game;
}

void CarloTree::customSimulation(Game * game){}

void CarloTree::fillPlayableMovesOrRandomEvents(Game * game){
    if (!game->waitingForRandomEvents()){
        ct_playableMovesOrRandomEvents = game->playableMoves();
    }
    else{
        ct_playableMovesOrRandomEvents = game->randomCoordinates();
    }
}

void CarloTree::backPropagation(){
	int win = ct_score->getWinNumber();
	int draw = ct_score->getDrawNumber();
	int defeat = ct_score->getDefeatNumber();
	int unfinished = ct_score->getUnfinishedNumber();
    int gameScore = ct_score->getGameScore();

	bool sonIsExpandable = ct_isExpandable;
	CarloTree * father = ct_father;

	unsigned int drawsToCancel = 0;
	unsigned int winsToCancel = 0;
	unsigned int defeatsToCancel = 0;
	unsigned int unfinishedToCancel = 0;

	bool cancelWins = false;
	bool cancelDefeats = false;
	bool firstFather = true;
	while (father != NULL){
		father->ct_score->newSimulation();
        if (father->ct_score->getGameScore() < gameScore){
            father->ct_score->setGameScore(gameScore);
        }
		if (win > 0){
			father->ct_score->newWin();
			if (firstFather){
				drawsToCancel = father->ct_score->getDrawNumber();
				unfinishedToCancel = father->ct_score->getUnfinishedNumber();
				defeatsToCancel = father->ct_score->getDefeatNumber();

				father->ct_isExpandable = false;
				cancelDefeats = true;
			}
		}
		if (draw > 0){
			father->ct_score->newDraw();
		}
		if (defeat > 0){
			father->ct_score->newDefeat();
			if (firstFather){
				drawsToCancel = father->ct_score->getDrawNumber();
				unfinishedToCancel = father->ct_score->getUnfinishedNumber();
				winsToCancel = father->ct_score->getWinNumber();

				father->ct_isExpandable = false;
				cancelWins = true;
			}
		}
		if (unfinished > 0){
			father->ct_score->newUnfinished();
		}
		if (cancelWins){
			father->ct_score->cancelWins(winsToCancel);
			father->ct_score->cancelDraws(drawsToCancel, false);
			father->ct_score->cancelUnfinished(unfinishedToCancel, false);
		}
		else if (cancelDefeats){
			father->ct_score->cancelDefeats(defeatsToCancel);
			father->ct_score->cancelDraws(drawsToCancel, true);
			father->ct_score->cancelUnfinished(unfinishedToCancel, true);
		}
		if (!sonIsExpandable){
			if (father->ct_isExpandable){
                vector<Coordinates> unplayedMovesOrRandomEvents = father->getUnplayedMovesOrRandomEvents();
                if (unplayedMovesOrRandomEvents.empty()){
					bool aSonIsExpandable = false;
					for(map<Coordinates, CarloTree *>::iterator son_iterator = father->ct_sons.begin(); son_iterator != father->ct_sons.end(); ++son_iterator){
						aSonIsExpandable = aSonIsExpandable || son_iterator->second->ct_isExpandable;
						if (aSonIsExpandable)
							break;
					}
					father->ct_isExpandable = aSonIsExpandable;
				}
			}
			sonIsExpandable = father->ct_isExpandable;
		}
        customBackPropagation(father);
		father = father->ct_father;
		firstFather = false;
	}
}

void CarloTree::customBackPropagation(CarloTree * ancestor){}

CarloScore * CarloTree::getScore(){
    return ct_score;
}

void CarloTree::setAsRoot(){
	this->ct_game = this->getGameCopy();
	this->ct_father = NULL;
	this->ct_root = this;
	vector<CarloTree *> allDescendants = getAllDescendants();
	for (vector<CarloTree *>::iterator descendantsIterator = allDescendants.begin(); descendantsIterator != allDescendants.end(); descendantsIterator++){
		(*descendantsIterator)->ct_root = this;
	}
}

CarloTree * CarloTree::changeRoot(vector<Coordinates> coordinates){
	CarloTree * newRoot = this;
	for (vector<Coordinates>::iterator coordinates_iterator = coordinates.begin(); coordinates_iterator != coordinates.end(); coordinates_iterator++){
		if (newRoot->ct_sons.count(*coordinates_iterator) == 0){
			newRoot = NULL;
			break;
		}
		newRoot = newRoot->ct_sons.at(*coordinates_iterator);
	}

	if (newRoot != this){
		if (newRoot != NULL){
			newRoot->setAsRoot();
		}
		delete this;
	}

	return newRoot;
}

void CarloTree::display(){
	cerr<<endl;
}

CarloScore::CarloScore(int explorationParameter, int winValue, int defeatValue, int drawValue, int unfinishedValue){
	s_winNumber = 0;
	s_drawNumber = 0;
	s_defeatNumber = 0;
	s_simulationNumber = 0;
	s_unfinishedNumber = 0;
	s_explorationParameter = explorationParameter;
    s_winValue = winValue;
    s_defeatValue = defeatValue;
    s_drawValue = drawValue;
    s_unfinishedValue = unfinishedValue;
}

void CarloScore::newSimulation(){
	s_simulationNumber++;
}

void CarloScore::newWin(){
	s_winNumber++;
}

void CarloScore::newDraw(){
	s_drawNumber++;
}

void CarloScore::newDefeat(){
	s_defeatNumber++;
}

void CarloScore::newUnfinished(){
	s_unfinishedNumber++;
}

double CarloScore::computeScore(int totalSimulations){
	if (s_simulationNumber == 0){
        return DBL_MAX;
	}
	return computeFinalScore() + (double)s_explorationParameter*sqrt(log((double)totalSimulations)/(double)s_simulationNumber);
}

double CarloScore::computeFinalScore(){
	if (s_simulationNumber == 0){
        return DBL_MAX;
	}
    return ((double)(s_winValue * s_winNumber + s_drawValue * s_drawNumber + s_unfinishedValue * s_unfinishedNumber + s_defeatValue * s_defeatNumber)/(double)(s_simulationNumber));
}

unsigned int CarloScore::getSimulationNumber(){
	return s_simulationNumber;
}

unsigned int CarloScore::getWinNumber(){
	return s_winNumber;
}

unsigned int CarloScore::getDrawNumber(){
	return s_drawNumber;
}

unsigned int CarloScore::getDefeatNumber(){
	return s_defeatNumber;
}

unsigned int CarloScore::getUnfinishedNumber(){
	return s_unfinishedNumber;
}

void CarloScore::cancelWins(unsigned int winsToCancel){
	s_winNumber -= winsToCancel;
	s_defeatNumber += winsToCancel;
}

void CarloScore::cancelDraws(unsigned int drawsToCancel, bool replaceWithWins){
	s_drawNumber -= drawsToCancel;
	if (replaceWithWins){
		s_winNumber += drawsToCancel;
	}
	else{
		s_defeatNumber += drawsToCancel;
	}
}

void CarloScore::cancelUnfinished(unsigned int unfinishedToCancel, bool replaceWithWins){
	s_unfinishedNumber -= unfinishedToCancel;
	if (replaceWithWins){
		s_winNumber += unfinishedToCancel;
	}
	else{
		s_defeatNumber += unfinishedToCancel;
	}
}

void CarloScore::cancelDefeats(unsigned int defeatsToCancel){
	s_defeatNumber -= defeatsToCancel;
	s_winNumber += defeatsToCancel;
}

void CarloScore::setGameScore(int gameScore){
    s_gameScore = gameScore;
}

int CarloScore::getGameScore(){
    return s_gameScore;
}
