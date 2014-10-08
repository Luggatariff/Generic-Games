/*!
 * \file Coordinates.hpp
  *\brief Coordinates description
 * \author Anaël Petit
 */

#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include "vector"

using namespace std;

/*!
 * \class Coordinates
 * \brief class used to represent integer Coordinates
 */
class Coordinates{
	vector<unsigned int> m_coordinates;	/*!<vector of coordinates*/
public:
	/*!
	 * \brief initializes Coordinates with the desired dimension
	 * \param dimension : number of coordinates
	 */
	Coordinates(unsigned int dimension = 0);

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
	unsigned int& operator[](unsigned int dimension);

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
};

#endif
