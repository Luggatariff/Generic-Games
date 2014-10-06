/*!
 * \file Pattern.hpp
 * \brief Pattern description
 * \author Anaël Petit
 */

#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "Square.hpp"
#include "Pattern.hpp"
#include "vector"

using namespace std;

/*!
 * \brief struct used to define variable integer to compute a set of patterns
 */
struct VarPattern{
	int v_begin;	/*!<beginning of variable interval*/
	int v_step;		/*!<step used to browse the interval*/
	int v_end;		/*!<end of variable interval*/
};

/*!
 * \class Pattern
 * \brief class representing a recognizable Pattern in a Board Game
 */
template<typename SquareAttribute> class Pattern{
public:

	/*!
	 * \brief gets the variables values of the Pattern
	 * \return variable values for the Pattern (used in getSquares)
	 */
	virtual vector<VarPattern> getVariables() = 0;

	/*!
	 * \brief for each variable value, gets a set of coordinates and Squares to compare with a Board
	 * \param variable_values : variable values (computed from VarPattern)
	 * \return pairs of coordinates and corresponding Squares
	 */
	virtual vector<pair<vector<unsigned int>, Square<SquareAttribute> > > getSquares(vector<int> variable_values) = 0;

	/*!
	 * \brief virtual destructor.
	 */
	virtual ~Pattern() = 0;
};

#endif
