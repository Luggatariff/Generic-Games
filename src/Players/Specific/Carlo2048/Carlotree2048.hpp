#ifndef CARLOTREE2048_HPP
#define CARLOTREE2048_HPP

#include "Carlo/CarloTree.hpp"

class CarloTree2048 : public CarloTree
{
protected:
    void fillPlayableMovesOrRandomEvents(Game *game);
    CarloTree * createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue);
public:
    CarloTree2048(Game * game, unsigned int team, CarloTree * root = NULL, CarloTree * father = NULL, Coordinates sonId = Coordinates(0), int winValue = 10, int defeatValue = 0, int drawValue = 2, int unfinishedValue = 1);
};

#endif // CARLOTREE2048_HPP
