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

/** \file     Hierarchical4DCodec.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-04-11
 */

#include "Lib/Part2/Common/TransformMode/Hierarchical4DCodec.h"


void Hierarchical4DCodec::reset_probability_models() {
  probability_models.reset();
}


void Hierarchical4DCodec::set_level_shift(int value) {
  mPGMScale = value;
}


int Hierarchical4DCodec::get_level_shift() const {
  return mPGMScale;
}


void Hierarchical4DCodec::set_inferior_bit_plane(uint8_t value) {
  inferior_bit_plane = value;
}


uint8_t Hierarchical4DCodec::get_inferior_bit_plane() const {
  return inferior_bit_plane;
}


void Hierarchical4DCodec::set_superior_bit_plane(uint8_t value) {
  superior_bit_plane = value;
}


uint8_t Hierarchical4DCodec::get_superior_bit_plane() const {
  return superior_bit_plane;
}


void Hierarchical4DCodec::set_lightfield_dimension(
    const LightfieldDimension<uint32_t>& dimension) {
  std::tie(mNumberOfVerticalViews, mNumberOfHorizontalViews, mNumberOfViewLines,
      mNumberOfViewColumns) = dimension.as_tuple();
}


void Hierarchical4DCodec::set_transform_dimension(
    const LightfieldDimension<uint32_t>& dimension) {
  std::tie(mTransformLength_t, mTransformLength_s, mTransformLength_v,
      mTransformLength_u) = dimension.as_tuple();
}


void Hierarchical4DCodec::set_minimum_transform_dimension(
    const LightfieldDimension<uint32_t>& dimension) {
  std::tie(mMinimumTransformLength_t, mMinimumTransformLength_s,
      mMinimumTransformLength_v, mMinimumTransformLength_u) =
      dimension.as_tuple();
}