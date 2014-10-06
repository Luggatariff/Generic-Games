/*!
 * \file Square.cpp
 * \brief Square code
 * \author Anaël Petit
 */

#include "Square.hpp"

template<typename Attribute>
bool Square<Attribute>::isEmpty(){
	return s_attributes.empty();
}

template<typename Attribute>
bool Square<Attribute>::isAttribute(Attribute attribute){
	for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
		if (s_attributes[i_a] == attribute){
			return true;
		}
	}
	return false;
}

template<typename Attribute>
void Square<Attribute>::addAttribute(Attribute attribute){
	if (!isAttribute(attribute))
		s_attributes.push_back(attribute);
}

template<typename Attribute>
void Square<Attribute>::delAttribute(Attribute attribute){
	for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
		if (s_attributes[i_a] == attribute){
			s_attributes.erase(s_attributes.begin() + i_a);
			break;
		}
	}
}

template<typename Attribute>
bool Square<Attribute>::isEqualTo(Square target){
	if (s_attributes.size() == target.s_attributes.size()){
		for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
			if (!target.isAttribute(s_attributes[i_a]))
				return false;
		}
		return true;
	}
	return false;
}

template<typename Attribute>
bool Square<Attribute>::includedIn(Square target){
	if (s_attributes.size() <= target.s_attributes.size()){
		for (unsigned int i_a = 0; i_a < s_attributes.size(); i_a++){
			if (!target.isAttribute(s_attributes[i_a]))
				return false;
		}
		return true;
	}
	return false;
}

template<typename Attribute>
bool Square<Attribute>::includes(Square target){
	if (s_attributes.size() >= target.s_attributes.size()){
		for (unsigned int i_a = 0; i_a < target.s_attributes.size(); i_a++){
			if (!isAttribute(target.s_attributes[i_a]))
				return false;
		}
		return true;
	}
	return false;
}
