#include "Carlotree2048.hpp"
#include "My2048/My2048.hpp"
#include <algorithm>

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

        Coordinates leftCoordinates = Coordinates(1);
        leftCoordinates[0] = My2048::MY_2048_MOVE_LEFT;
        Coordinates rightCoordinates = Coordinates(1);
        rightCoordinates[0] = My2048::MY_2048_MOVE_RIGHT;
        Coordinates downCoordinates = Coordinates(1);
        downCoordinates[0] = My2048::MY_2048_MOVE_DOWN;

        ct_playableMovesOrRandomEvents.clear();
        if (find(playableMoves.begin(), playableMoves.end(), rightCoordinates) != playableMoves.end()){
            ct_playableMovesOrRandomEvents.push_back(rightCoordinates);
        }
        if (find(playableMoves.begin(), playableMoves.end(), downCoordinates) != playableMoves.end()){
            ct_playableMovesOrRandomEvents.push_back(downCoordinates);
        }
        if (find(playableMoves.begin(), playableMoves.end(), leftCoordinates) != playableMoves.end()){
            ct_playableMovesOrRandomEvents.push_back(leftCoordinates);
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
