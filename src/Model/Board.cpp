/*!
 * \file Board.cpp
 * \brief Board code
 * \author Anaël Petit
 */

#include "Board.hpp"

template<typename SquareAttribute>
Board<SquareAttribute>::Board(vector<unsigned int> size){
	this->b_size = size;
	unsigned int case_number = 1;
	for (unsigned int i_s = 0; i_s < b_size.size(); i_s++){
		case_number *= b_size[i_s];
	}

	this->b_squares = new vector<Square<SquareAttribute> >(case_number);
}

template<typename SquareAttribute>
Square<SquareAttribute> * Board<SquareAttribute>::getSquare(vector<unsigned int> coordinates){
	unsigned int multi = 1;
	unsigned int flattened_coordinates = 0;
	for (unsigned int i_c = 0; i_c < coordinates.size(); i_c++){
		flattened_coordinates += coordinates[i_c] * multi;
		multi *= b_size[i_c];
	}

	return b_squares[flattened_coordinates];
}

template<typename SquareAttribute>
unsigned int Board<SquareAttribute>::checkPattern(Pattern<SquareAttribute> pattern){
	unsigned int found_patterns = 0;

	vector<VarPattern> pattern_vars = pattern.getVariables();
	vector<int> pattern_vars_values;
	for (unsigned int i_pv = 0; i_pv < pattern_vars.size(); i_pv++){
		pattern_vars_values.push_back(pattern_vars[i_pv].v_begin);
	}

	bool all_values_to_end = false;
	while (!all_values_to_end){
		vector<pair<vector<unsigned int>, Square<SquareAttribute> > > pattern_squares = pattern.getSquares(pattern_vars_values);
		bool success = true;
		for (unsigned int i_ps = 0; i_ps < pattern_squares.size(); i_ps++){
			success = success && this->getSquare(pattern_squares[i_ps].first)->isEqualTo(pattern_squares[i_ps].second);
			if (!success) break;
		}
		if (success) found_patterns++;

		all_values_to_end = true;
		for (unsigned int i_pv = 0; i_pv < pattern_vars_values.size(); i_pv++){
			if (pattern_vars_values[i_pv] + pattern_vars[i_pv].v_step < pattern_vars[i_pv].v_end){
				all_values_to_end = false;
				pattern_vars_values[i_pv] += pattern_vars[i_pv].v_step;
				break;
			}
			else{
				pattern_vars_values[i_pv] = pattern_vars[i_pv].v_begin;
			}
		}
	}

	return found_patterns;
}
