/*!
 * \file Square.hpp
 * \brief Square description
 * \author Anaël Petit
 */

#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "vector"

using namespace std;

/*!
 * \class Square
 * \brief class representing a Square on a game board
 */
template<typename Attribute> class Square{
	vector<Attribute> s_attributes; 	/*!<Square attributes*/
public:
	/*!
	 * \brief checks is a Square has no Attribute
	 * \return True is the Square had no Attribute, false otherwise
	 */
	bool isEmpty();
	/*!
	 * \brief checks if an Attribute is in Square Attributes list
	 * \param attribute : attribute
	 * \return True if the Attribute is in the list, False otherwise
	 */
	bool isAttribute(Attribute attribute);
	/*!
	 * \brief adds an Attribute to the Square
	 * \param attribute : Attribute to add
	 */
	void addAttribute(Attribute attribute);
	/*!
	 * \brief removes an Attribute from the Square
	 * \param attribute : Attribute to remove
	 */
	void delAttribute(Attribute attribute);

	/*!
	 * \brief checks if two Squares Attributes list are the same
	 * \param target : compared Square
	 * \return True if Attributes lists are the same, False otherwise
	 */
	bool isEqualTo(Square target);
	/*!
	 * \brief checks if source Attributes are included in target Attributes list
	 * \param target: compared Square
	 * \return True id source Attributes are included in target Attributes
	 */
	bool includedIn(Square target);
	/*!
	 * \brief checks if target Attributes are included in source Attributes list
	 * \param target: compared Square
	 * \return True id target Attributes are included in source Attributes
	 */
	bool includes(Square target);
};

#endif
