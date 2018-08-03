#include "Carlo2048.hpp"
#include "Carlotree2048.hpp"

Carlo2048::Carlo2048(string name, unsigned int teamId, unsigned int maxSimulationNumber, int winValue, int defeatValue, int drawValue, int unfinishedValue, bool displayTree)
    : Carlo(name, teamId, maxSimulationNumber, winValue, defeatValue, drawValue, unfinishedValue, displayTree)
{
}

void Carlo2048::newCarloTree(Game * game){
    c_tree = new CarloTree2048(game->copy(), c_team, NULL, NULL, Coordinates(0), c_winValue, c_defeatValue, c_drawValue, c_unfinishedValue);
}
