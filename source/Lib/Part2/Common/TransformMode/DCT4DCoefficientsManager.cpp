/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     DCT4DCoefficientsManager.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-03-21
 */


#include "DCT4DCoefficientsManager.h"


void DCT4DCoefficientsManager::set_transform_max_sizes(int max_size_u, int max_size_v, int max_size_s, int max_size_t) {
	size_to_transform_weights_map.clear();
	max_sizes[LightFieldDimension::U]=max_size_u;
	max_sizes[LightFieldDimension::V]=max_size_v;
	max_sizes[LightFieldDimension::S]=max_size_s;
	max_sizes[LightFieldDimension::T]=max_size_t;
}

void DCT4DCoefficientsManager::set_transform_gains(double transform_gain_u, double transform_gain_v, double transform_gain_s, double transform_gain_t) {
	size_to_transform_weights_map.clear();//all data that is there is now invalid
	gains[LightFieldDimension::U]=transform_gain_u;
	gains[LightFieldDimension::V]=transform_gain_v;
	gains[LightFieldDimension::S]=transform_gain_s;
	gains[LightFieldDimension::T]=transform_gain_t;
}

double DCT4DCoefficientsManager::compute_weight_for_size_in_dimension(std::pair<int, LightFieldDimension> size_dimension_pair) {
	int size  = std::get<0>(size_dimension_pair);
	LightFieldDimension dimension_name = std::get<1>(size_dimension_pair);

	double transform_gain =gains[dimension_name]*sqrt(max_sizes[dimension_name]/static_cast<double>(size));
	//used to test correctness of corner block shrinking: 
	//double transform_gain =gains[dimension_name]*sqrt(2.0/static_cast<double>(size));

	size_to_transform_weights_map.insert(std::make_pair(size_dimension_pair, transform_gain));

	return transform_gain;
}

double DCT4DCoefficientsManager::get_weight_for_size_in_dimension(int size, LightFieldDimension dimension_name) {
	auto key = std::make_pair(size, dimension_name);
	auto it = size_to_transform_weights_map.find(key);
	if (it != size_to_transform_weights_map.end()) { 
		return it->second;
	}
	return compute_weight_for_size_in_dimension(key);
}


//in theory the IDCT can be computed using the same coefficients, but transposed and scaled..
const double* DCT4DCoefficientsManager::generate_coeffients_for_size(std::pair<int, int> size) { 
// in a multi thread execution, this is not thread safe... perhaps this can be called using std::call_once
	int columns  = std::get<0>(size);
	int rows = std::get<1>(size);

	auto coefficients = std::make_unique<double[]>(columns*rows);
	double* coefficients_ptr=coefficients.get();

	if (forward){
		for(auto j = 0; j < columns; ++j) { //i=0
			*coefficients_ptr=1.0;
			coefficients_ptr++;
		}
		for(auto i = 1; i < rows; ++i) {
			for(auto j = 0; j < columns; ++j) {
				*coefficients_ptr=sqrt(2.0)*cos(dctPi*(i*(2.0*j+1.0))/(2.0*rows));
				coefficients_ptr++;
			}
		}
	} else {//inverse
		for(auto j = 0; j < columns; ++j) {
			*coefficients_ptr=1.0/static_cast<double>(columns);
			coefficients_ptr++;
			for(auto i = 1; i < rows; ++i) {
				*coefficients_ptr=sqrt(2.0)*cos(dctPi*(i*(2.0*j+1.0))/(2.0*rows))/static_cast<double>(columns);
				coefficients_ptr++;
			}
		}
	}

	coefficients_ptr=coefficients.get(); //this is required because the move changes the ownership and sets the uniqueptr to null
	size_to_coefficients_map.insert(std::make_pair(size, std::move(coefficients)));

	return coefficients_ptr;
}


const double* DCT4DCoefficientsManager::get_coefficients_for_size(int width, int height) {
	auto key = std::make_pair(width, height);
	auto it = size_to_coefficients_map.find(key);
	if (it != size_to_coefficients_map.end()) { 
		return it->second.get();
	}
	return generate_coeffients_for_size(key);
}

const double* DCT4DCoefficientsManager::get_coefficients_for_size(int size) {
	return get_coefficients_for_size(size, size);
}

DCT4DCoefficientsManager& DCT4DCoefficientsManager::get_instance(bool forward) {
	static DCT4DCoefficientsManager instance(forward);
	return instance;
}