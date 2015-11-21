#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "mainmodel.h"

namespace Ui {
class MainWindow;
class Dialog;
}

class MainController;

class ParameterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParameterDialog(QWidget *parent = 0);
    ~ParameterDialog();
    Ui::Dialog * getUi();
private:
    Ui::Dialog *ui;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    vector<QMenu *> mw_menu_list;                       /*!<Dynamically added Menu list*/
    vector<QFrame *> mw_dynamic_parameter_frame_list;   /*!<Dynamically added Parameter Frames list*/

    inline void update_menu_bar();
    inline void update_dynamic_parameter_pane();
    inline void clear_layout(QLayout * layout);
public:
    explicit MainWindow(QWidget *parent = 0, MainController *controller = NULL);

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
     * \param parameter_widgets : pointers to parameter widgets
     */
     void displayBlockingParameterWindow(QString title, QList<QPair<QLabel *, QWidget *> > parameter_widgets);

    /*!
     * \brief adds a Dynamic Parameter Frame to the window
     * \param index : index where to add the frame
     * \param title : frame title
     * \param parameter_widgets : pointers to parameter widgets
     */
    void addDynamicParameterFrame(unsigned int index, QString title, QList<QPair<QLabel *, QWidget *> > parameter_widgets);

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
    ParameterDialog mw_parameter_dialog;
};

#endif // MAINWINDOW_H
