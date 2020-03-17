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

/** \file     Transformed4DBlock.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-03-19
 */

#include "Transformed4DBlock.h"


bool Transformed4DBlock::operator==(const Block4D& block) {
  if (block.mlength_t != mlength_t) {
    return false;
  }
  if (block.mlength_s != mlength_s) {
    return false;
  }
  if (block.mlength_v != mlength_v) {
    return false;
  }
  if (block.mlength_u != mlength_u) {
    return false;
  }

  auto ptr_mydata = data.get();
  for (auto ptr_blockdata = block.mPixelData;
       ptr_blockdata < block.mPixelData + number_of_elements; ++ptr_blockdata) {
    if (*ptr_mydata != *ptr_blockdata) {
      return false;
    }
    ++ptr_mydata;
  }
  return true;
}


/**
 * @brief Used to create a copy of the transformed elements into a 4D block
 * @details [long description]
 * @return Returns a Block4D that contains a copy of the transformed values that the transformed4D block contains
 */
Block4D Transformed4DBlock::generate_copy_in_block() const {
  Block4D block;
  block.set_dimension(mlength_t, mlength_s, mlength_v, mlength_u);

  auto ptr_mydata = data.get();

  for (auto ptr_blockdata = block.mPixelData;
       ptr_blockdata < block.mPixelData + number_of_elements; ++ptr_blockdata) {
    *ptr_blockdata = *ptr_mydata;
    ++ptr_mydata;
  }
  return block;  //i hope move is used here..
}


void Transformed4DBlock::swap_data_with_block(Block4D& block) {
  auto data_ptr = data.release();
  std::swap(data_ptr, block.mPixelData);
  data.reset(data_ptr);

  for (auto t = decltype(mlength_t){0}; t < mlength_t; ++t) {
    for (auto s = decltype(mlength_s){0}; s < mlength_s; ++s) {
      for (auto v = decltype(mlength_v){0}; v < mlength_v; ++v) {
        block.mPixel[t][s][v] =
            block.mPixelData + block.get_linear_position(t, s, v, 0);
      }
    }
  }
}


std::size_t Transformed4DBlock::get_energy() const {
  std::size_t energy = 0;
  auto data_ptr = data.get();
  for (std::size_t i = 0; i < number_of_elements; ++i) {
    energy += std::abs(*data_ptr);
    ++data_ptr;
  }
  return energy;
}


void Transformed4DBlock::alloc_temp() {
  auto max = std::max({mlength_u, mlength_v, mlength_s, mlength_t});
  temp = std::make_unique<block4DElementType[]>(max);
  temp_double = std::make_unique<double[]>(max);
}


void Transformed4DBlock::alloc_data() {
  if (!data) {
    data = std::make_unique<block4DElementType[]>(number_of_elements);
  }
  data_double = std::make_unique<double[]>(number_of_elements);
}


void Transformed4DBlock::alloc_resources() {
  alloc_data();
  alloc_temp();
}


std::size_t Transformed4DBlock::get_number_of_elements() const {
  return number_of_elements;
}


void Transformed4DBlock::set_number_of_elements() {
  number_of_elements = mlength_t * mlength_s * mlength_v * mlength_u;
}


void Transformed4DBlock::set_dimensions(uint32_t length_t, uint32_t length_s,
    uint32_t length_v, uint32_t length_u) {
  mlength_t = length_t;
  mlength_s = length_s;
  mlength_v = length_v;
  mlength_u = length_u;
  set_number_of_elements();
}


//this constructor should only be visible to derived classes
Transformed4DBlock::Transformed4DBlock(const Block4D& block) {
  set_dimensions(
      block.mlength_t, block.mlength_s, block.mlength_v, block.mlength_u);
  alloc_resources();
}


Transformed4DBlock::Transformed4DBlock(const Block4D& block,
    const std::tuple<const double*, const double*, const double*, const double*>
        coefficients,
    const std::tuple<double, double, double, double> transform_gains =
        std::make_tuple(1.0, 1.0, 1.0, 1.0))
    : Transformed4DBlock(block) {
  do_4d_transform(data.get(), block.mPixelData, coefficients, transform_gains);
}


Transformed4DBlock::Transformed4DBlock(
    const block4DElementType* transformed_values, uint32_t u, uint32_t v,
    uint32_t s, uint32_t t) {
  set_dimensions(t, s, v, u);
  alloc_resources();
  std::memcpy(data.get(), transformed_values,
      mlength_u * mlength_v * mlength_s * mlength_t *
          sizeof(block4DElementType));
}


Transformed4DBlock::Transformed4DBlock(Block4D&& source) {
  set_dimensions(
      source.mlength_t, source.mlength_s, source.mlength_v, source.mlength_u);
  data = std::unique_ptr<block4DElementType[]>(source.mPixelData);
  alloc_resources();
  source.mPixelData = nullptr;
}


Transformed4DBlock::Transformed4DBlock(Transformed4DBlock&& other) {
  set_dimensions(
      other.mlength_t, other.mlength_s, other.mlength_v, other.mlength_u);
  data = std::move(other.data);
  temp = std::move(other.temp);
}


Block4D Transformed4DBlock::inverse(
    const std::tuple<const double*, const double*, const double*, const double*>
        coefficients,
    const std::tuple<double, double, double, double> transform_gains =
        std::make_tuple(1.0, 1.0, 1.0, 1.0)) {
  Block4D block;
  block.set_dimension(mlength_t, mlength_s, mlength_v, mlength_u);
  do_4d_transform(block.mPixelData, data.get(), coefficients, transform_gains);
  return block;
}
