#ifndef MY2048_HPP
#define MY2048_HPP

#include "Game.hpp"

class My2048 : public Game
{
public:
    static const unsigned int MY_2048_SIDE                  = 4;

    static const unsigned int MY_2048_MOVE_LEFT             = 0;
    static const unsigned int MY_2048_MOVE_UP               = 1;
    static const unsigned int MY_2048_MOVE_RIGHT            = 2;
    static const unsigned int MY_2048_MOVE_DOWN             = 3;

private:
    static const unsigned int MY_2048_RANDOM_EVENT_LINE     = 0;
    static const unsigned int MY_2048_RANDOM_EVENT_COLUMN   = 1;
    static const unsigned int MY_2048_RANDOM_EVENT_VALUE    = 2;
    static const unsigned int MY_2048_RANDOM_EVENT_DIMENSION= 3;

    static const unsigned int MY_2048_MOVE_DIMENSION        = 1;

    Player *            m_player;
    bool                m_waitingForRandomEvents;
    vector<Coordinates> m_last_moves;

    unsigned int        m_values[MY_2048_SIDE][MY_2048_SIDE];
    unsigned int        m_winValue;
    unsigned int        m_score;

public:

    My2048(Player * player, int winValue = 2048);
    ~My2048();
    Game * copy();

    vector<Player *> players();

    bool isEnded();
    bool isWon();
    int whoWon();
    bool isWinner(unsigned int team_id);

    int score(unsigned int team_id);
    int victoryScore();

    void start();
    unsigned int moveDimension();
    bool isPlayableMove(Coordinates coordinates);
    vector<Coordinates> playableMoves();
    void playMove(Coordinates coordinates, bool launchRandomEvents);

    bool waitingForRandomEvents();
    unsigned int randomDimension();
    vector<Coordinates> randomCoordinates();
    void forceRandom(Coordinates coordinates);
    void launchRandomEvent(int iterations);

    Player * nextPlayer();
    vector<Coordinates> lastMovesAndRandomEvents();

    void display(std::ostream & out);
    vector<vector<unsigned int>> getValues();

    string getName();
};

#endif // MY2048_HPP
