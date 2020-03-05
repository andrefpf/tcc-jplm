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

/** \file     DCT4DBlock.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-03-21
 */

#include "DCT4DBlock.h"


DCT4DBlock::DCT4DBlock(const Block4D& block) : Transformed4DBlock(block) {
  DCT4DCoefficientsManager& manager(
      DCT4DCoefficientsManager::get_instance(true));

  auto coefficients =
      std::make_tuple(manager.get_coefficients_for_size(block.mlength_t),  //
          manager.get_coefficients_for_size(block.mlength_s),
          manager.get_coefficients_for_size(block.mlength_v),
          manager.get_coefficients_for_size(block.mlength_u));

  auto weights =
      std::make_tuple(manager.get_weight_for_size_in_dimension(
                          block.mlength_t, LightFieldDimensions::T),  //
          manager.get_weight_for_size_in_dimension(
              block.mlength_s, LightFieldDimensions::S),
          manager.get_weight_for_size_in_dimension(
              block.mlength_v, LightFieldDimensions::V),
          manager.get_weight_for_size_in_dimension(
              block.mlength_u, LightFieldDimensions::U));

  auto n =
      block.mlength_t * block.mlength_s * block.mlength_v * block.mlength_u;

  this->mult = std::get<0>(weights) * std::get<1>(weights) *
               std::get<2>(weights) * std::get<3>(weights) *
               static_cast<double>(n);

  do_4d_transform(data.get(), block.mPixelData, coefficients, weights);
}


Block4D DCT4DBlock::inverse() {
  DCT4DCoefficientsManager& manager(
      DCT4DCoefficientsManager::get_instance(false));

  auto coefficients =
      std::make_tuple(manager.get_coefficients_for_size(mlength_t),  //
          manager.get_coefficients_for_size(mlength_s),
          manager.get_coefficients_for_size(mlength_v),
          manager.get_coefficients_for_size(mlength_u));

  auto weights =
      std::make_tuple(1.0 / manager.get_weight_for_size_in_dimension(
                                mlength_t, LightFieldDimensions::T),  //
          1.0 / manager.get_weight_for_size_in_dimension(
                    mlength_s, LightFieldDimensions::S),
          1.0 / manager.get_weight_for_size_in_dimension(
                    mlength_v, LightFieldDimensions::V),
          1.0 / manager.get_weight_for_size_in_dimension(
                    mlength_u, LightFieldDimensions::U));

  return Transformed4DBlock::inverse(coefficients, weights);
}