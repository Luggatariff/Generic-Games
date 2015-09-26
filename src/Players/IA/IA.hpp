/*!
 * \file IA.hpp
 * \brief IA description
 * \author Anaël Petit
 */

#ifndef IA_HPP
#define IA_HPP

#include "../Player.hpp"
#include "IATree.hpp"
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>

/*!
 * \class IA
 * \brief this class represents a Artificial Intelligence player
 */
class IA: public Player{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Generic-Games.Player" FILE "")
    Q_INTERFACES(Player)

    QFrame * ia_parameter_frame;    /*!<Parameter frame*/
    QLineEdit * ia_name_line_edit;  /*!<line edit for name*/
    QSpinBox * ia_team_spin_box;    /*!<spin box for team*/
    QSpinBox * ia_level_spin_box;   /*!<spin box for level*/
    QSpinBox * ia_max_free_choices_spin_box;   /*!<spin box for free choices let to the IA*/
    QCheckBox * ia_display_tree_check_box;     /*!<check box for IATree display*/
    QCheckBox * ia_display_messages_check_box; /*!<check box for messages display*/

	IATree * ia_tree;						/*!<IA Choice Tree*/

    inline unsigned int ia_level();
    inline unsigned int ia_max_free_choices();
    inline bool ia_display_tree();
    inline bool ia_display_messages();
public:
	/*!
	 * \brief IA constructor
	 * \param level : IA thinking level
	 * \param display_tree : indicates if IA must display the choice Tree (debug)
	 */
    IA();

    ~IA();

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
