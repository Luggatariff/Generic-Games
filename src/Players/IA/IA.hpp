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
	unsigned int ia_level;		/*!<IA level of thinking*/
	IATree * ia_tree;			/*!<IA Choice Tree*/
	Coordinates ia_last_move;	/*!<IA last move*/
	bool ia_fast_compute; 		/*!<IA indicates if ia fast compute is used*/
	bool ia_display_tree;		/*!<indicates if IA must display the choice Tree (debug)*/
public:
	/*!
	 * \brief IA constructor
	 * \param level : IA thinking level
	 * \param fast_compute : indicates if fast_compute must be used or not
	 * \param display_tree : indicates if IA must display the choice Tree (debug)
	 */
	IA(unsigned int level, bool fast_compute = false, bool display_tree = false);

	void start(Game * game);
	Coordinates play(Game * game);
};

#endif
