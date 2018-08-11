#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Player.hpp>
#include <Game.hpp>
#include <QMap>
#include <QPushButton>
#include <Coordinates.hpp>
#include <QThread>
#include <Carlo/Carlo.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void moveComputed();
    void restartLoop();
    void stopLoop();
    void parameterChanged();
signals:
    void computeMove(Game * game);

private:
    Ui::MainWindow  *ui;

    //Game
    Game            *m_game;

    void            startGame();
    void            stopGame(bool restart);

    //Player
    Player          *m_myPlayer;
    Carlo           *m_otherPlayer;

    int             m_winValue              = 1000;
    int             m_defeatValue           = 0;
    int             m_drawValue             = 0;
    int             m_unfinishedValue       = 100;
    int             m_maxSimulationNumber   = 100;

    //Loop Info
    int             m_bestScore             = 0;
    unsigned int    m_iterations            = 0;

    void            resetLoopInfo();
    void            updateLoopInfo();

    //Loop Management
    QThread         m_workerThread;
    bool            m_stopping              = false;
    bool            m_restarting            = false;
    bool            m_running               = false;

    void            setStopping(bool stopping);
    void            setRestarting(bool restarting);
    void            setRunning(bool running);
    bool            isStopping();
    bool            isRestarting();
    bool            isRunning();
};

#endif // MAINWINDOW_H
