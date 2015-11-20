#include "maincontroller.h"

MainController::MainController()
{

}

void MainController::init(){
    c_window.show();
    QList<QString> gameTypesList = c_model.getAvailableGameTypes();
    for (QList<QString>::Iterator gameTypesIterator = gameTypesList.begin(); gameTypesIterator != gameTypesList.end(); gameTypesIterator++){
        c_window.addAction(*gameTypesIterator, c_window.getGameMenu());
    }
}

