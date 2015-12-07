/*!
 * \file Human.hpp
 * \brief Human description
 * \author Anaël Petit
 */

#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "../Player.hpp"
#include <QLineEdit>
#include <QSpinBox>

/*!
 * \class Human
 * \brief this class represents a Human player
 */
class Human: public Player{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Generic-Games.Player" FILE "")
    Q_INTERFACES(Player)

    QLabel * hu_name_label;         /*!<label for name line edit*/
    QLineEdit * hu_name_line_edit;  /*!<line edit for name*/
    QLabel * hu_team_label;         /*!<label for team spin box*/
    QSpinBox * hu_team_spin_box;    /*!<spin box for team*/
public:
	/*!
	 * \brief Human constructor
	 * \param name : Player name
	 */
    Human();

    ~Human();

	void start(Game * game);
	void end(Game * game);
	Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>());
    QString getName();
	unsigned int getTeam();
    QString getType();
    Player * createInstance();
    QFrame * getParameterFrame();
    QList<QPair<QLabel *, QWidget *> > getParameterWidgets();
};

#endif
