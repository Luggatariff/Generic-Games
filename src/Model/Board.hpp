/*!
 * \file Board.hpp
 * \brief Board description
 * \author Anaël Petit
 */

#ifndef BOARD_HPP
#define BOARD_HPP

#include "Coordinates.hpp"
#include "Square.hpp"
#include "Pattern.hpp"
#include "vector"

using namespace std;

/*!
 * \class Board
 * \brief class representing a Game Board
 */
template<typename SquareAttribute> class Board{
	Coordinates b_max_coordinates;				/*!<Total size (maximum coordinates) of the Board for each dimension*/
	vector<Square<SquareAttribute> > b_squares;	/*!<Board Squares and their Attributes*/
public:
	/*!
	 * \brief creates a Board with a given size and dimension
	 * \param maximum_coordinates : size of each dimension
	 */
	Board(Coordinates maximum_coordinates);

	/*!
	 * \brief gets a Square from coordinates
	 * \param coordinates : coordinates of the wished Square
	 * \return wished Square
	 */
	Square<SquareAttribute> * getSquare(Coordinates coordinates);

	/*!
	 * \brief checks if a pattern is was found on the Board
	 * \param pattern : patter seeked in the Board
	 * \return 0 if the pattern wasn't found, or the number of occurrences.
	 */
	unsigned int checkPattern(Pattern<SquareAttribute> pattern);
};

#endif
