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
	bool isEmpty(){
		return s_attributes.empty();
	}

	/*!
	 * \brief copies the Square
	 * \return Square copy
	 */
	Square copy(){
		Square result;
		for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
			result.addAttribute(s_attributes[i_a]);
		}
		return result;
	}

	/*!
	 * \brief checks if an Attribute is in Square Attributes list
	 * \param attribute : attribute
	 * \return True if the Attribute is in the list, False otherwise
	 */
	bool isAttribute(Attribute attribute){
		for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
			if (s_attributes[i_a] == attribute){
				return true;
			}
		}
		return false;
	}

	/*!
	 * \brief adds an Attribute to the Square
	 * \param attribute : Attribute to add
	 */
	void addAttribute(Attribute attribute){
		if (!isAttribute(attribute))
			s_attributes.push_back(attribute);
	}

	/*!
	 * \brief removes an Attribute from the Square
	 * \param attribute : Attribute to remove
	 */
	void delAttribute(Attribute attribute){
		for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
			if (s_attributes[i_a] == attribute){
				s_attributes.erase(s_attributes.begin() + i_a);
				break;
			}
		}
	}

	/*!
	 * \brief checks if two Squares Attributes list are the same
	 * \param target : compared Square
	 * \return True if Attributes lists are the same, False otherwise
	 */
	bool isEqualTo(Square target){
		if (s_attributes.size() == target.s_attributes.size()){
			for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
				if (!target.isAttribute(s_attributes[i_a]))
					return false;
			}
			return true;
		}
		return false;
	}

	/*!
	 * \brief checks if source Attributes are included in target Attributes list
	 * \param target: compared Square
	 * \return True id source Attributes are included in target Attributes
	 */
	bool includedIn(Square target){
		if (s_attributes.size() <= target.s_attributes.size()){
			for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
				if (!target.isAttribute(s_attributes[i_a]))
					return false;
			}
			return true;
		}
		return false;
	}

	/*!
	 * \brief checks if target Attributes are included in source Attributes list
	 * \param target: compared Square
	 * \return True id target Attributes are included in source Attributes
	 */
	bool includes(Square target){
		if (s_attributes.size() >= target.s_attributes.size()){
			for (unsigned int i_a = 0; i_a < target.s_attributes.size(); i_a++){
				if (!isAttribute(target.s_attributes[i_a]))
					return false;
			}
			return true;
		}
		return false;
	}

	/*!
	 * \brief compares two Squares
	 */
	bool operator==(const Square& square) const{
		vector<Attribute> first = (*this).s_attributes;
		vector<Attribute> second = square.s_attributes;
		sort (first.begin(), first.end());
		sort (second.begin(), second.end());
		return ( first == second );
	}

};

#endif
