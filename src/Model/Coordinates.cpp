/*!
 * \file Coordinates.cpp
 * \brief Coordinates code
 * \author Anaël Petit
 */

#include "Coordinates.hpp"

Coordinates::Coordinates(unsigned int dimension):
QObject(0)
{
	this->m_coordinates = vector<unsigned int>(dimension, 0);
}

Coordinates::Coordinates(const Coordinates& coordinates):
QObject(0)
{
    m_coordinates = coordinates.m_coordinates;
}

Coordinates Coordinates::copy(){
	Coordinates result(this->dimension());
	for (unsigned int i_c = 0; i_c < this->dimension(); i_c++)
		result.m_coordinates[i_c] = this->m_coordinates[i_c];
	return result;
}

unsigned int Coordinates::dimension(){
    return m_coordinates.size();
}

unsigned int& Coordinates::operator[](unsigned int const &dimension){
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
Coordinates& Coordinates::operator=(const Coordinates& coordinates){
    m_coordinates = coordinates.m_coordinates;
    return *this;
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

void Coordinates::display(std::ostream & out){
	out << "(";
	for (vector<unsigned int>::iterator coord_iterator = m_coordinates.begin(); coord_iterator != m_coordinates.end(); ++coord_iterator){
		if ( coord_iterator != m_coordinates.begin() ){
			out << " ";
		}
		out << *coord_iterator;
	}
	out << ")";
}
