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

/** \file     DCT4DCoefficientsManager.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-03-21
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_DCT4DCOEFFICIENTSMANAGER_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_DCT4DCOEFFICIENTSMANAGER_H__

#include <cmath>
#include <memory>
#include <map>
#include <tuple>

#include "LightField.h"

class DCT4DCoefficientsManager
{
private:
	const double dctPi = 3.141592653589793;
	//static constexpr double dctPi = std::atan(1.0)*4.0;
	//static constexpr auto dctPi = std::acos(-1);

	bool forward;
	std::map<std::pair<int, int>, std::unique_ptr<double[]> > size_to_coefficients_map;
	
	std::map<std::pair<int, LightFieldDimension>, double> size_to_transform_weights_map;
	int max_sizes[4];
	double gains[4];


	const double* generate_coeffients_for_size(std::pair<int, int> size);
	double compute_weight_for_size_in_dimension(std::pair<int, LightFieldDimension> size_dimension_pair);
	DCT4DCoefficientsManager(bool is_forward) : forward(is_forward) {};
	~DCT4DCoefficientsManager()= default;
	DCT4DCoefficientsManager(const DCT4DCoefficientsManager&)= delete;
	DCT4DCoefficientsManager& operator=(const DCT4DCoefficientsManager&)= delete;

public:
	static DCT4DCoefficientsManager& get_instance(bool forward);
	
	const double* get_coefficients_for_size(int width, int height); //returns const double* to ensure that the returned data will not be modified
	const double* get_coefficients_for_size(int size);
	void set_transform_max_sizes(int max_size_u, int max_size_v, int max_size_s, int max_size_t);
	void set_transform_gains(double transform_gain_u, double transform_gain_v, double transform_gain_s, double transform_gain_t);
	double get_weight_for_size_in_dimension(int size, LightFieldDimension dimension_name); //

};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_DCT4DCOEFFICIENTSMANAGER_H__ */