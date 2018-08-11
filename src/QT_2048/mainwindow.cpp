#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>
#include <utility>
#include <TicTacToe/TicTacToe.hpp>
#include <My2048/My2048.hpp>
#include <Carlo/Carlo.hpp>
#include <Specific/Carlo2048/Carlo2048.hpp>
#include <Human/Human.hpp>
#include <string>
#include <Worker.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_myPlayer = new Human(string("X"), 0);
    m_otherPlayer = new Carlo2048(string("Carlo"), 1, m_maxSimulationNumber, m_winValue, m_defeatValue, m_drawValue, m_unfinishedValue);
    m_game = new My2048(m_otherPlayer, 8196);

    for (unsigned int line = 0; line < My2048::MY_2048_SIDE; ++line){
        ui->tableWidget->setRowHeight(line, ui->tableWidget->height() / My2048::MY_2048_SIDE);
    }
    for (unsigned int column = 0; column < My2048::MY_2048_SIDE; ++column){
        ui->tableWidget->setColumnWidth(column, ui->tableWidget->width() / My2048::MY_2048_SIDE);
    }

    ui->textSimu->setText(QString::number(m_maxSimulationNumber));
    ui->textWin->setText(QString::number(m_winValue));
    ui->textDefeat->setText(QString::number(m_defeatValue));
    ui->textDraw->setText(QString::number(m_drawValue));
    ui->textUnfinished->setText(QString::number(m_unfinishedValue));

    resetLoopInfo();

    Worker *worker = new Worker();
    worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::computeMove, worker, &Worker::compute);
    connect(worker, &Worker::computed, this, &MainWindow::moveComputed);
    m_workerThread.start();

    connect (ui->textSimu, SIGNAL(textChanged(QString)), this, SLOT(parameterChanged()));
    connect (ui->textWin, SIGNAL(textChanged(QString)), this, SLOT(parameterChanged()));
    connect (ui->textDefeat, SIGNAL(textChanged(QString)), this, SLOT(parameterChanged()));
    connect (ui->textDraw, SIGNAL(textChanged(QString)), this, SLOT(parameterChanged()));
    connect (ui->textUnfinished, SIGNAL(textChanged(QString)), this, SLOT(parameterChanged()));

    connect (ui->actionRed_marrer, SIGNAL(triggered(bool)), this, SLOT(restartLoop()));
    connect (ui->actionArr_ter, SIGNAL(triggered(bool)), this, SLOT(stopLoop()));
    connect (qApp, SIGNAL(aboutToQuit()), &m_workerThread, SLOT(terminate()));
}

void MainWindow::restartLoop(){
    resetLoopInfo();
    if (isRunning()){
        setRestarting(true);
        setStopping(true);
    }
    else{
        startGame();
    }
}

void MainWindow::stopLoop(){
    if (isRunning()){
        setStopping(true);
    }
}

void MainWindow::resetLoopInfo(){
    m_iterations = 0;
    m_bestScore = 0;
    updateLoopInfo();
}

void MainWindow::updateLoopInfo(){
    ui->labelIterValue->setText(QString::number(m_iterations));
    ui->labelBestScoreValue->setText(QString::number(m_bestScore));
}

void MainWindow::parameterChanged(){
    bool conversionOk;
    {
        unsigned int tempValue = ui->textSimu->text().toUInt(&conversionOk);
        if (conversionOk){
             m_maxSimulationNumber = tempValue;
             m_otherPlayer->setMaxSimulationNumber(m_maxSimulationNumber);
        }
    }
    {
        int tempValue = ui->textWin->text().toInt(&conversionOk);
        if (conversionOk){
             m_winValue = tempValue;
             m_otherPlayer->setWinValue(tempValue);
        }
    }
    {
        int tempValue = ui->textDraw->text().toInt(&conversionOk);
        if (conversionOk){
             m_drawValue = tempValue;
             m_otherPlayer->setDrawValue(tempValue);
        }
    }
    {
        int tempValue = ui->textDefeat->text().toInt(&conversionOk);
        if (conversionOk){
             m_defeatValue = tempValue;
             m_otherPlayer->setDefeatValue(tempValue);
        }
    }
    {
        int tempValue = ui->textUnfinished->text().toInt(&conversionOk);
        if (conversionOk){
             m_unfinishedValue = tempValue;
             m_otherPlayer->setUnfinishedValue(tempValue);
        }
    }

    resetLoopInfo();
}

void MainWindow::startGame(){
    setRunning(true);
    setStopping(false);
    setRestarting(false);

    vector<Player *> players = m_game->players();
    for (unsigned int i = 0; i < players.size(); i++){
        players[i]->start(m_game);
    }
    m_game->start();

    emit computeMove(m_game);
}

void MainWindow::stopGame(bool restart){
    m_iterations++;
    int score = m_game->score(m_otherPlayer->getTeam());
    if (m_bestScore < score){
        m_bestScore = score;
    }
    updateLoopInfo();

    vector<Player *> players = m_game->players();
    for (unsigned int i = 0; i < players.size(); i++){
        players[i]->end(m_game);
    }

    if (restart){
        startGame();
    }
    else{
        setRunning(false);
    }
}

void MainWindow::moveComputed(){
    vector<vector<unsigned int> > values = ((My2048 *)m_game)->getValues();
    for (unsigned int line = 0; line < values.size(); ++line){
        vector<unsigned int> lineValues = values[line];
        for (unsigned int column = 0; column < lineValues.size(); ++column){
            QTableWidgetItem * item = ui->tableWidget->item(line, column);
            if (item == NULL){
                item = new QTableWidgetItem(QString());
                ui->tableWidget->setItem(line, column, item);
            }
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            unsigned int lineValue = lineValues[column];
            if (lineValue != 0){
                item->setText(QString::number(lineValue));
                if (lineValue > 2048){
                    item->setBackgroundColor(QColor(0,0,0,255));
                    item->setTextColor(QColor(255,255,255,255));
                }
                else if (lineValue > 64){
                    item->setBackgroundColor(QColor(0,255,128,255));
                    item->setTextColor(QColor(0,0,0,255));
                }
                else if (lineValue > 16){
                    item->setBackgroundColor(QColor(255,0,0,255));
                    item->setTextColor(QColor(0,0,0,255));
                }
                else if (lineValue > 4){
                    item->setBackgroundColor(QColor(255,128,128,255));
                    item->setTextColor(QColor(0,0,0,255));
                }
                else{
                    item->setBackgroundColor(QColor(75,75,75,255));
                    item->setTextColor(QColor(0,0,0,255));
                }
            }
            else{
                item->setText(QString());
                item->setBackgroundColor(QColor(255,255,255,255));
                item->setTextColor(QColor(0,0,0,255));
            }
        }
    }
    ui->tableWidget->repaint();

    if (isStopping()){
        stopGame(isRestarting());
    }
    else{
        if (m_game->isEnded()){
            stopGame(!m_game->isWon());
        }
        else{
            emit computeMove(m_game);
        }
    }
}

void MainWindow::setStopping(bool stopping){
    m_stopping = stopping;
}

void MainWindow::setRestarting(bool restarting){
    m_restarting = restarting;
}

void MainWindow::setRunning(bool running){
    m_running = running;
    ui->textSimu->setEnabled(!running);
    ui->textWin->setEnabled(!running);
    ui->textDefeat->setEnabled(!running);
    ui->textDraw->setEnabled(!running);
    ui->textUnfinished->setEnabled(!running);
}


bool MainWindow::isStopping(){
    return m_stopping;
}

bool MainWindow::isRestarting(){
    return m_restarting;
}

bool MainWindow::isRunning(){
    return m_running;
}

MainWindow::~MainWindow()
{
    delete ui;
}
