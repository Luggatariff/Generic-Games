#ifndef CARLO2048_HPP
#define CARLO2048_HPP

#include "Carlo/Carlo.hpp"

class Carlo2048 : public Carlo
{
protected:
    void newCarloTree(Game * game);
public:
    Carlo2048(string name, unsigned int teamId, unsigned int maxSimulationNumber, int winValue = 10, int defeatValue = 0, int drawValue = 2, int unfinishedValue = 1, bool displayTree = false);
};

#endif // CARLO2048_HPP
