#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <Player.hpp>
#include <Game.hpp>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = NULL);

signals:
    void computed();

public slots:
    void compute(Game * game);
};

#endif // WORKER_H
