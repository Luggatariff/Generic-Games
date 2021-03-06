/*!
 * \file Carlo.cpp
 * \brief Carlo code
 * \author Ana?l Petit
 */

#include <Carlo/Carlo.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Carlo::Carlo(string name, unsigned int teamId, unsigned int maxSimulationNumber, int winValue, int defeatValue, int drawValue, int unfinishedValue, bool displayTree){
    c_name = name;
    c_team = teamId;

    c_maxSimulationNumber = maxSimulationNumber;
    c_tree = NULL;
    c_displayTree = displayTree;

    c_winValue = winValue;
    c_defeatValue = defeatValue;
    c_drawValue = drawValue;
    c_unfinishedValue = unfinishedValue;
}

void Carlo::setMaxSimulationNumber(unsigned int maxSimulationNumber){
    c_maxSimulationNumber = maxSimulationNumber;
}

void Carlo::setWinValue(int winValue){
    c_winValue = winValue;
}

void Carlo::setDefeatValue(int defeatValue){
    c_defeatValue = defeatValue;
}

void Carlo::setDrawValue(int drawValue){
    c_drawValue = drawValue;
}

void Carlo::setUnfinishedValue(int unfinishedValue){
    c_unfinishedValue = unfinishedValue;
}

void Carlo::start(Game * game){
    if (c_tree != NULL){
        delete c_tree;
        c_tree = NULL;
    }
}

void Carlo::end(Game * game){
    if (c_tree != NULL){
        delete c_tree;
        c_tree = NULL;
    }
}

Coordinates Carlo::play(Game * game , vector<Coordinates> limit_choices){
    vector<Coordinates> last_moves_and_random_events = game->lastMovesAndRandomEvents();

    if (c_tree != NULL){
        vector<Coordinates> each_player_last_move;
        for (unsigned int i_p = c_lastMovesAndRandomEvents.size(); i_p < last_moves_and_random_events.size(); i_p++){
            each_player_last_move.push_back(last_moves_and_random_events[i_p]);
        }
        c_tree = c_tree->changeRoot(each_player_last_move);
    }

    if (c_tree == NULL){
        newCarloTree(game);
    }

    c_lastMovesAndRandomEvents = last_moves_and_random_events;

    return c_tree->getBestMove(c_maxSimulationNumber);
}

void Carlo::newCarloTree(Game * game){
    c_tree = new CarloTree(game->copy(), c_team, NULL, NULL, Coordinates(0), c_winValue, c_defeatValue, c_drawValue, c_unfinishedValue);
}

string Carlo::getName(){
    return c_name;
}

unsigned int Carlo::getTeam(){
    return c_team;
}
