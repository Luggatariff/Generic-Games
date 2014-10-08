/*!
 * \file Coordinates.cpp
 * \brief Coordinates code
 * \author Anaël Petit
 */

#include "Coordinates.hpp"

Coordinates::Coordinates(unsigned int dimension){
	this->m_coordinates = vector<unsigned int>(dimension, 0);
}

unsigned int Coordinates::dimension(){
	return this->m_coordinates.size();
}

unsigned int& Coordinates::operator[](unsigned int dimension){
	while (dimension >= this->dimension())
		m_coordinates.push_back(0);
	return m_coordinates[dimension];
}

bool Coordinates::operator<(const Coordinates& coordinates) const{
	return ((*this).m_coordinates < coordinates.m_coordinates);
}
bool Coordinates::operator>(const Coordinates& coordinates) const{
	return ((*this).m_coordinates > coordinates.m_coordinates);
}
bool Coordinates::operator==(const Coordinates& coordinates) const{
	return ((*this).m_coordinates == coordinates.m_coordinates);
}

unsigned int Coordinates::x(){
	return (*this)[0];
}

unsigned int Coordinates::y(){
	return (*this)[1];
}

unsigned int Coordinates::z(){
	return (*this)[2];
}
