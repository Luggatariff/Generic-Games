/*!
 * \file Coordinates.hpp
  *\brief Coordinates description
 * \author Anaël Petit
 */

#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include "vector"
#include <iostream>
#include <QObject>

using namespace std;

/*!
 * \class Coordinates
 * \brief class used to represent integer Coordinates
 */
class Coordinates: public QObject{
    Q_OBJECT
	vector<unsigned int> m_coordinates;	/*!<vector of coordinates*/
public:
	/*!
	 * \brief initializes Coordinates with the desired dimension
	 * \param dimension : number of coordinates
	 */
	Coordinates(unsigned int dimension = 0);

    /*!
     * \brief Coordinates copy constructor
     * \param coordinates : source
     */
    Coordinates(const Coordinates& coordinates);

	/*!
	 * \brief copies the Coordinates
	 * \return Coordinates copy
	 */
	Coordinates copy();

	/*!
	 * \brief return the dimension of the Coordinates (number of coordinates)
	 * \return dimension of the Coordinates
	 */
	unsigned int dimension();

	/*!
	 * \brief gets the coordinate on the given dimension
	 * \param dimension : dimension of the desired coordinate
	 * \return coordinate on the given dimension, 0 if the dimension isn't considered
	 */
	unsigned int& operator[](unsigned int const &dimension);

	/*!
	 * \brief compares two Coordinates
	 */
	bool operator<(const Coordinates& coordinates) const;
	/*!
	 * \brief compares two Coordinates
	 */
	bool operator>(const Coordinates& coordinates) const;
	/*!
	 * \brief compares two Coordinates
	 */
	bool operator==(const Coordinates& coordinates) const;

    /*!
     * \brief Coordinates::operator =
     * \param coordinates : source
     * \return destination
     */
    Coordinates& operator=(const Coordinates& coordinates);

	/*!
	 * \brief gets the coordinate for the first dimension
	 * \return coordinate on the first dimension (0 if the dimension isn't considered)
	 */
	unsigned int x();
	/*!
	 * \brief gets the coordinate for the second dimension
	 * \return coordinate on the second dimension (0 if the dimension isn't considered)
	 */
	unsigned int y();
	/*!
	 * \brief gets the coordinate for the third dimension
	 * \return coordinate on the third dimension (0 if the dimension isn't considered)
	 */
	unsigned int z();

	/*!
	 * \brief displays the coordinates on the standard output
	 */
	void display(std::ostream & out = std::cout);
};

#endif
