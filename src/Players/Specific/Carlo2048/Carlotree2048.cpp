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
        Coordinates highPosition(2);
        highPosition[0] = My2048::MY_2048_SIDE - 1;
        highPosition[1] = My2048::MY_2048_SIDE - 1;
        do{
            playableMoves = ct_playableMovesOrRandomEvents;
            ct_playableMovesOrRandomEvents.clear();

            for (vector<Coordinates>::iterator it = playableMoves.begin(); it != playableMoves.end(); ++it){
                Game * moveGame = playableMovesGames.at(*it);
                vector<vector<unsigned int>> values = ((My2048 *)moveGame)->getValues();

                unsigned int highPositionValue = values[highPosition[0]][highPosition[1]];
                bool isHighestValue = true;
                if (highPositionValue > 0){
                    for (unsigned int line = 0; line < values.size(); ++line){
                        vector<unsigned int> lineValues = values[line];
                        for (unsigned int column = 0; column < lineValues.size(); ++column){
                            Coordinates position(2);
                            position[0] = line;
                            position[1] = column;
                            if (find(passedHighPositions.begin(), passedHighPositions.end(), position) == passedHighPositions.end()){
                                unsigned int value = values[line][column];
                                if (value > highPositionValue){
                                    isHighestValue = false;
                                    break;
                                }
                            }
                        }
                        if (!isHighestValue){
                            break;
                        }
                    }
                }
                else{
                    isHighestValue = false;
                }

                if (isHighestValue){
                     ct_playableMovesOrRandomEvents.push_back(*it);
                }
            }

            passedHighPositions.push_back(highPosition);
            if (highPosition[1] > 0){
                highPosition[1]--;
            }
            else{
                break;
            }
        }
        while (ct_playableMovesOrRandomEvents.size() > 1);

        if (ct_playableMovesOrRandomEvents.empty()){
            ct_playableMovesOrRandomEvents = playableMoves;
        }

        for (map<Coordinates, Game *>::iterator it = playableMovesGames.begin(); it != playableMovesGames.end(); ++it){
           delete it->second;
        }
    }
    else{
        ct_playableMovesOrRandomEvents = game->randomCoordinates();
    }
}

CarloTree * CarloTree2048::createSon(Game * game, unsigned int team, CarloTree * root, CarloTree * father, Coordinates sonId, int winValue, int defeatValue, int drawValue, int unfinishedValue){
    return new CarloTree2048(game, team, root, father, sonId, winValue, defeatValue, drawValue, unfinishedValue);
}
