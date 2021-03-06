#include "Carlotree2048.hpp"
#include "My2048/My2048.hpp"
#include <algorithm>
#include "map"

CarloScore2048::CarloScore2048(int explorationParameter, int winValue, int defeatValue, int drawValue, int unfinishedValue)
    : CarloScore(explorationParameter, winValue, defeatValue, drawValue, unfinishedValue)
{
    s_heuristicNumber = 0;
}

void CarloScore2048::newHeuristic(unsigned int heuristicIncrement){
    s_heuristicNumber += heuristicIncrement;
}

unsigned int CarloScore2048::getHeuristicNumber(){
    return s_heuristicNumber;
}

double CarloScore2048::computeFinalScore(){
    unsigned int simulationNumber = getSimulationNumber();
    double carloScore = CarloScore::computeFinalScore();
    if (simulationNumber == 0){
        return carloScore;
    }
    return carloScore + (((double) (s_heuristicNumber)) / ((double)(simulationNumber)));
}

CarloTree2048::CarloTree2048(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue)
    : CarloTree(game,team,root,father,sonId,winValue,defeatValue,drawValue,unfinishedValue)
{
    if (ct_score != NULL){
        delete ct_score;
    }
    ct_score = new CarloScore2048(ct_explorationParameter, winValue, defeatValue, drawValue, unfinishedValue);
    if (ct_root == this){
        fillPlayableMovesOrRandomEvents(game);
    }
}

void CarloTree2048::fillPlayableMovesOrRandomEvents(Game * game){
    CarloTree::fillPlayableMovesOrRandomEvents(game);
}

void CarloTree2048::customSimulation(Game * game){
    vector<vector<unsigned int> > values = ((My2048 *)game)->getValues();

    bool isAnIsolatedValue = false;
    for (unsigned int line = 0; line < values.size(); ++line){
        vector<unsigned int> lineValues = values[line];
        for (unsigned int column = 0; column < lineValues.size(); ++column){
            unsigned int value = values[line][column];

            if (value > 0){
                bool isIsolatedValue = true;
                for (unsigned int neighborLine = (line == 0 ? line : line - 1); neighborLine <= (line == My2048::MY_2048_SIDE - 1 ? line : line + 1); ++neighborLine){
                    for (unsigned int neighborColumn = (column == 0 ? column : column - 1); neighborColumn <= (column == My2048::MY_2048_SIDE - 1 ? column : column + 1); ++neighborColumn){
                        if ((neighborLine == line) != (neighborColumn == column)){
                           unsigned int neighborValue = values[neighborLine][neighborColumn];
                           if (neighborValue <= value){
                               isIsolatedValue = false;
                               if (neighborValue == value){
                                  ((CarloScore2048 *)ct_score)->newHeuristic(1);
                               }
                           }
                        }
                    }
                }
                if (isIsolatedValue){
                    isAnIsolatedValue = true;
                }
            }
            else{
                ((CarloScore2048 *)ct_score)->newHeuristic(3);
            }
        }
    }
    if (!isAnIsolatedValue){
        ((CarloScore2048 *)ct_score)->newHeuristic(5);
    }


}

void CarloTree2048::customBackPropagation(CarloTree * ancestor){
    unsigned int heuristicNumber = ((CarloScore2048 *)ct_score)->getHeuristicNumber();
    ((CarloScore2048 *)ancestor->getScore())->newHeuristic(heuristicNumber);
}

CarloTree * CarloTree2048::createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue){
    return new CarloTree2048(game, team, root, father, sonId, winValue, defeatValue, drawValue, unfinishedValue);
}
