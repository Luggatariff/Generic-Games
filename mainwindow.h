#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    vector<QMenu *> mw_menu_list; /*!<Dynamically added Menu list*/
    QMenu * mw_game_menu;         /*!<Generic Games menu*/
    vector<QFrame *> mw_dynamic_parameter_frame_list;   /*<Dynamically added Parameter Frames list*/

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    /*!
     * \brief adds a Menu to the MenuBar
     * \param index : index of the added Menu
     * \param title : Menu title
     * \return pointer to the added Menu
     */
    QMenu * addMenu(unsigned int index, QString title);

    /*!
     * \brief removes thhe menu at the given index
     * \param index : Menu index
     */
    void removeMenu(unsigned int index);

    /*!
     * \brief gets the total number of menus
     * \return number of menus
     */
    unsigned int getMenuNumber();

    /*!
     * \brief gets a pointer to Generic Games menu
     * \return Pointer on Generic Games menu
     */
    QMenu * getGameMenu();

    /*!
     * \brief adds an Action to a given menu
     * \param title : Action title
     * \param menu : Pointer to the menu
     * \return created Action
     */
    QAction * addAction(QString title, QMenu * menu);

    /*!
     * \brief displays a blocking Parameter window
     * \param title : window title
     * \param parameter_frame : pointer to Parameter Frame
     * \return true if user has clicked OK, false otherwise
     */
    bool displayBlockingParameterWindow(QString title, QFrame * parameter_frame);

    /*!
     * \brief adds a Dynamic Parameter Frame to the window
     * \param index : index where to add the frame
     * \param title : frame title
     * \param parameter_frame : pointer to Frame
     */
    void addDynamicParameterFrame(unsigned int index, QString title, QFrame * parameter_frame);

    /*!
     * \brief removes the Frame to the given index
     * \param index
     */
    void removeDynamicParameterFrame(unsigned int index);

    /*!
     * \brief gets the total number of Dynamic Parameter Frames
     * \return Framesn number
     */
    unsigned int getDynamicParameterFrameNumber();

    /*!
     * \brief changes Frame in main display
     * \param frame
     */
    void changeMainDisplay(QFrame * frame);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
