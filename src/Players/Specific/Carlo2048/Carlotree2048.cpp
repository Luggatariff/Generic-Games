#include "Carlotree2048.hpp"
#include "My2048/My2048.hpp"
#include <algorithm>
#include "map"

CarloTree2048::CarloTree2048(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue)
    : CarloTree(game,team,root,father,sonId,winValue,defeatValue,drawValue,unfinishedValue)
{
    if (ct_root == this){
        fillPlayableMovesOrRandomEvents(game);
    }
}

void CarloTree2048::fillPlayableMovesOrRandomEvents(Game * game){
    if (!game->waitingForRandomEvents()){
        vector<Coordinates> playableMoves = game->playableMoves();

        map<Coordinates, Game *> playableMovesGames;
        for (vector<Coordinates>::iterator it = playableMoves.begin(); it != playableMoves.end(); ++it){
            Game * gameCopy = game->copy();
            gameCopy->playMove(*it, false);
            playableMovesGames.insert(pair<Coordinates, Game *>(*it, gameCopy));
        }

        ct_playableMovesOrRandomEvents = playableMoves;

        vector<Coordinates> passedHighPositions;
        vector<Coordinates> nextHighPositions;
        Coordinates nextHighPosition(2);
        nextHighPosition[0] = My2048::MY_2048_SIDE - 1;
        nextHighPosition[1] = My2048::MY_2048_SIDE - 1;
        nextHighPositions.push_back(nextHighPosition);

        unsigned int oldMaxHighPositionValue = 0;
        bool firstIteration = true;
        int increment = -1;
        do{
            Coordinates highPosition = nextHighPositions.back();
            nextHighPositions.pop_back();

            unsigned int maxHighPositionValue = 0;
            bool maxHighPositionValueDefined = false;
            for (vector<Coordinates>::iterator it = playableMoves.begin(); it != playableMoves.end(); ++it){
                Game * moveGame = playableMovesGames.at(*it);
                vector<vector<unsigned int>> values = ((My2048 *)moveGame)->getValues();

                unsigned int highPositionValue = values[highPosition[0]][highPosition[1]];
                if (highPositionValue > maxHighPositionValue){
                    ct_playableMovesOrRandomEvents.clear();
                    maxHighPositionValue = highPositionValue;
                    maxHighPositionValueDefined = true;
                }
                if (maxHighPositionValue == highPositionValue && maxHighPositionValueDefined){
                     ct_playableMovesOrRandomEvents.push_back(*it);
                }
            }
            playableMoves = ct_playableMovesOrRandomEvents;

            if (firstIteration || maxHighPositionValue <= oldMaxHighPositionValue){
                passedHighPositions.push_back(highPosition);
                Coordinates neighbor = highPosition;
                if ((increment < 0 && neighbor[1] > 0) || (increment > 0 && neighbor[1] < (My2048::MY_2048_SIDE - 1))){
                    neighbor[1] += increment;
                }
                else{
                    neighbor[0]--;
                    increment *= -1;
                }
                if (find(passedHighPositions.begin(), passedHighPositions.end(), neighbor) == passedHighPositions.end()){
                    nextHighPositions.insert(nextHighPositions.begin(), neighbor);
                }
            }
            else{
                break;
            }
            oldMaxHighPositionValue = maxHighPositionValue;
            firstIteration = false;
        }
        while (ct_playableMovesOrRandomEvents.size() > 1 && !nextHighPositions.empty());

        for (map<Coordinates, Game *>::iterator it = playableMovesGames.begin(); it != playableMovesGames.end(); ++it){
           delete it->second;
        }

        if (ct_playableMovesOrRandomEvents.empty()){
            ct_playableMovesOrRandomEvents = playableMoves;
        }

    }
    else{
        ct_playableMovesOrRandomEvents = game->randomCoordinates();
    }
}

CarloTree * CarloTree2048::createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue){
    return new CarloTree2048(game, team, root, father, sonId, winValue, defeatValue, drawValue, unfinishedValue);
}
