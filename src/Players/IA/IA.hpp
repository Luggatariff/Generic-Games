/*!
 * \file IA.hpp
 * \brief IA description
 * \author Anaël Petit
 */

#ifndef IA_HPP
#define IA_HPP

#include "Player.hpp"
#include "IATree.hpp"

/*!
 * \class IA
 * \brief this class represents a Artificial Intelligence player
 */
class IA: public Player{
	unsigned int ia_level;					/*!<IA level of thinking*/
	IATree * ia_tree;						/*!<IA Choice Tree*/
	unsigned int ia_max_choice_tree_size;	/*!<IA max choice tree size in nodes number*/
	bool ia_display_tree;					/*!<indicates if IA must display the choice Tree (debug)*/
public:
	/*!
	 * \brief IA constructor
	 * \param level : IA thinking level
	 * \param display_tree : indicates if IA must display the choice Tree (debug)
	 */
	IA(unsigned int level, bool display_tree = false);

	void start(Game * game);
	Coordinates play(Game * game);
};

#endif
