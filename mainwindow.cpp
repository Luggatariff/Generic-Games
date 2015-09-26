#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/pluginloader.hpp"
#include "src/Players/Player.hpp"
#include "src/Games/Game.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QList<Game *> game_list = PluginLoader::pluginByDir<Game>(qApp->applicationDirPath() + "/plugins/");
    QList<Game *>::iterator game_list_it;

    for (game_list_it=game_list.begin(); game_list_it!=game_list.end(); game_list_it++){
        this->ui->menuGeneric_Games->addMenu((*game_list_it)->getName());
    }

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
