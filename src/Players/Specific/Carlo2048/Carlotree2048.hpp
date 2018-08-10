#ifndef CARLOTREE2048_HPP
#define CARLOTREE2048_HPP

#include "Carlo/CarloTree.hpp"

class CarloScore2048 : public CarloScore
{
    unsigned int s_heuristicNumber = 0;
public:
    CarloScore2048(int explorationParameter, int winValue, int defeatValue, int drawValue, int unfinishedValue);
    void newHeuristic(unsigned int heuristicIncrement);
    unsigned int getHeuristicNumber();
    double computeFinalScore();
};

class CarloTree2048 : public CarloTree
{
protected:
    void fillPlayableMovesOrRandomEvents(Game *game);
    CarloTree * createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue);
    void customSimulation(Game * game);
    void customBackPropagation(CarloTree * ancestor);
public:
    CarloTree2048(Game * game, unsigned int team, CarloTree * root = NULL, CarloTree * father = NULL, Coordinates sonId = Coordinates(0), int winValue = 10, int defeatValue = 0, int drawValue = 2, int unfinishedValue = 1);
};

#endif // CARLOTREE2048_HPP
