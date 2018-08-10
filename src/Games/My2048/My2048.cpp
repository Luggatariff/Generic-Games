#include "My2048.hpp"
#include <algorithm>
#include <stdlib.h>
#include <time.h>

My2048::My2048(Player * player, int winValue){
    m_player = player;
    m_winValue = winValue;
}

My2048::~My2048(){}

Game * My2048::copy(){
    My2048 * game = new My2048(m_player, m_winValue);
    game->m_score = m_score;
    game->m_waitingForRandomEvents = m_waitingForRandomEvents;
    for (unsigned int l = 0; l < MY_2048_SIDE; l++){
        for (unsigned int c = 0; c < MY_2048_SIDE; c++){
            game->m_values[l][c] = m_values[l][c];
        }
    }
    return game;
}

vector<Player *> My2048::players(){
    vector<Player *> players;
    players.push_back(m_player);
    return players;
}

bool My2048::isEnded(){
    return (isWon() || playableMoves().empty());
}
bool My2048::isWon(){
    bool isWon = false;
    for (unsigned int l = 0; l < MY_2048_SIDE; l++){
        for (unsigned int c = 0; c < MY_2048_SIDE; c++){
            if (m_values[l][c] >= m_winValue){
                isWon = true;
                break;
            }
        }
        if (isWon){
            break;
        }
    }
    return isWon;
}
int My2048::whoWon(){
    return m_player->getTeam();
}
bool My2048::isWinner(unsigned int team_id){
    return (isWon() && (m_player->getTeam() == team_id));
}

int My2048::score(unsigned int team_id){
    int score = m_score;
    if (m_player->getTeam() != team_id){
        score = 0;
    }
    return score;
}
int My2048::victoryScore(){
    return std::numeric_limits<int>::max();
}

void My2048::start(){
    m_score = 0;
    for (unsigned int l = 0; l < MY_2048_SIDE; l++){
        for (unsigned int c = 0; c < MY_2048_SIDE; c++){
            m_values[l][c] = 0;
        }
    }
    srand (time(NULL));
    launchRandomEvent(2);
}

void My2048::launchRandomEvent(int iterations){
    for (int i = 0; i<iterations; ++i){
        vector<Coordinates> randomEvents = randomCoordinates();
        int randomEventIndex = rand() % randomEvents.size();
        forceRandom(randomEvents[randomEventIndex]);
    }
}

unsigned int My2048::moveDimension(){
    return MY_2048_MOVE_DIMENSION;
}
bool My2048::isPlayableMove(Coordinates coordinates){
    if (isEnded()) return false;

    vector<Coordinates> playableMovesVector = playableMoves();

    return (find(playableMovesVector.begin(), playableMovesVector.end(), coordinates) != playableMovesVector.end());
}
vector<Coordinates> My2048::playableMoves(){
    vector<Coordinates> playableMoves;
    for (unsigned int move = 0; move < 4; ++move){
        bool browseLines;
        bool revert;
        if (move == MY_2048_MOVE_LEFT){
            browseLines = true;
            revert = false;
        }
        else if (move == MY_2048_MOVE_UP){
            browseLines = false;
            revert = false;
        }
        else if (move == MY_2048_MOVE_RIGHT){
            browseLines = true;
            revert = true;
        }
        else{
            browseLines = false;
            revert = true;
        }
        int firstYValue = -1;
        int lastYValue = MY_2048_SIDE;
        int yIncrement = 1;
        if (revert){
            firstYValue = lastYValue;
            lastYValue = -1;
            yIncrement = -1;
        }
        bool moveIsPlayable = false;
        for (unsigned int x = 0; x < MY_2048_SIDE; ++x){
            int lastZeroIndex = -1;
            unsigned int lastValue = 0;
            for (int y = firstYValue + yIncrement; y != lastYValue; y += yIncrement){
                unsigned int value;
                if (browseLines){
                    value = m_values[x][y];
                }
                else{
                    value = m_values[y][x];
                }
                if (value == 0){
                    lastZeroIndex = y;
                }
                else if (lastZeroIndex >= 0 || lastValue == value){
                    moveIsPlayable = true;
                    break;
                }
                else{
                    lastValue = value;
                }
            }
            if (moveIsPlayable){
                break;
            }
        }
        if (moveIsPlayable){
            Coordinates coordinates = Coordinates(MY_2048_MOVE_DIMENSION);
            coordinates[0] = move;
            playableMoves.push_back(coordinates);
        }
    }
    return playableMoves;
}

void My2048::playMove(Coordinates coordinates, bool launchRandomEvents){
    m_last_moves.push_back(coordinates);
    int move = coordinates[0];

    bool browseLines;
    bool revert;
    if (move == MY_2048_MOVE_LEFT){
        browseLines = true;
        revert = false;
    }
    else if (move == MY_2048_MOVE_UP){
        browseLines = false;
        revert = false;
    }
    else if (move == MY_2048_MOVE_RIGHT){
        browseLines = true;
        revert = true;
    }
    else{
        browseLines = false;
        revert = true;
    }
    int firstYValue = -1;
    int lastYValue = MY_2048_SIDE;
    int yIncrement = 1;
    if (revert){
        firstYValue = lastYValue;
        lastYValue = -1;
        yIncrement = -1;
    }
    for (unsigned int x = 0; x < MY_2048_SIDE; ++x){
        vector<unsigned int> line;
        for (int y = firstYValue + yIncrement; y != lastYValue; y += yIncrement){
            if (browseLines){
                line.push_back(m_values[x][y]);
            }
            else{
                line.push_back(m_values[y][x]);
            }
        }

        bool alreadyFused[line.size()];
        for (unsigned int i = 0; i < line.size(); ++i){
            alreadyFused[i] = false;
        }
        vector<unsigned int> oldLine;
        do{
            oldLine = line;
            unsigned int lastValue = 0;
            int lastValueIndex = -1;
            int lastZeroIndex = -1;
            for (unsigned int i = 0; i < line.size(); ++i){
                unsigned int value = line[i];
                if (value == 0){
                    lastZeroIndex = i;
                }
                else if (lastValue == value && !alreadyFused[lastValueIndex] && !alreadyFused[i]){
                    line[lastValueIndex] = lastValue*2;
                    m_score += line[lastValueIndex];
                    alreadyFused[lastValueIndex] = true;
                    line[i] = 0;
                    break;
                }
                else if (lastZeroIndex >= 0){
                    line[lastZeroIndex] = value;
                    alreadyFused[lastValueIndex] = alreadyFused[i];
                    line[i] = 0;
                    alreadyFused[i] = false;
                    break;
                }
                else{
                    lastValueIndex = i;
                    lastValue = value;
                }
            }
        }
        while (oldLine != line);

        int lineIndex = 0;
        for (int y = firstYValue + yIncrement; y != lastYValue; y += yIncrement){
            if (browseLines){
                m_values[x][y] = line[lineIndex];
            }
            else{
                m_values[y][x] = line[lineIndex];
            }
            lineIndex++;
        }
    }

    m_waitingForRandomEvents = true;
    if (launchRandomEvents){
        launchRandomEvent(1);
    }
}

bool My2048::waitingForRandomEvents(){
    return m_waitingForRandomEvents;
}

unsigned int My2048::randomDimension(){
    return MY_2048_RANDOM_EVENT_DIMENSION;
}

vector<Coordinates> My2048::randomCoordinates(){
    vector<Coordinates> randomCoordinates;

    int lineIndex = MY_2048_RANDOM_EVENT_LINE;
    int columnIndex = MY_2048_RANDOM_EVENT_COLUMN;
    int valueIndex = MY_2048_RANDOM_EVENT_VALUE;

    for (unsigned int l = 0; l < MY_2048_SIDE; l++){
        for (unsigned int c = 0; c < MY_2048_SIDE; c++){
            if (m_values[l][c] == 0){
                Coordinates coordinates = Coordinates(MY_2048_RANDOM_EVENT_DIMENSION);

                coordinates[lineIndex] = l;
                coordinates[columnIndex] = c;

                coordinates[valueIndex] = 2;
                randomCoordinates.push_back(coordinates);
                coordinates[valueIndex] = 4;
                randomCoordinates.push_back(coordinates);
            }
        }
    }
    return randomCoordinates;
}

void My2048::forceRandom(Coordinates coordinates){
    m_last_moves.push_back(coordinates);

    int lineIndex = MY_2048_RANDOM_EVENT_LINE;
    int columnIndex = MY_2048_RANDOM_EVENT_COLUMN;
    int valueIndex = MY_2048_RANDOM_EVENT_VALUE;

    m_values[coordinates[lineIndex]][coordinates[columnIndex]] = coordinates[valueIndex];

    m_waitingForRandomEvents = false;
}

Player * My2048::nextPlayer(){
    return m_player;
}
vector<Coordinates> My2048::lastMovesAndRandomEvents(){
    return m_last_moves;
}

void My2048::display(std::ostream & out){
    for (unsigned int l = 0; l < MY_2048_SIDE; l++){
        for (unsigned int c = 0; c < MY_2048_SIDE; c++){
            unsigned int tempValue = m_values[l][c];
            if (tempValue == 0){
                tempValue = 1;
            }
            while (tempValue < 10000){
                out<<" ";
                tempValue *= 10;
            }
            out << m_values[l][c] << "|";
        }
        out << endl;
    }
    out << "Score: " << m_score << endl;
}

vector<vector<unsigned int>> My2048::getValues(){
    vector<vector<unsigned int>> values;
    for (unsigned int l = 0; l < MY_2048_SIDE; l++){
        vector<unsigned int> line;
        for (unsigned int c = 0; c < MY_2048_SIDE; c++){
            line.push_back(m_values[l][c]);
        }
        values.push_back(line);
    }
    return values;
}

string My2048::getName(){
    return string("2048");
}
