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

    QFrame * hu_parameter_frame;    /*!<Parameter frame*/
    QLineEdit * hu_name_line_edit;  /*!<line edit for name*/
    QSpinBox * hu_team_spin_box;  /*!<line edit for team*/
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
};

#endif
