#include "Worker.h"
#include <QThread>

Worker::Worker(QObject *parent) : QObject(parent)
{

}

void Worker::compute(Game * game){
    if (!game->isEnded()){
        Player * player = game->nextPlayer();

        Coordinates coordinates = player->play(game);

        if (game->isPlayableMove(coordinates)){
            game->playMove(coordinates, true);
        }

        emit computed();
    }
}
