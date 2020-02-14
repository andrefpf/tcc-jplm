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
  if (block.mlength_t != mlength_t)
    return false;
  if (block.mlength_s != mlength_s)
    return false;
  if (block.mlength_v != mlength_v)
    return false;
  if (block.mlength_u != mlength_u)
    return false;

  auto ptr_mydata = data.get();
  for (auto ptr_blockdata = block.mPixelData;
       ptr_blockdata < block.mPixelData + number_of_elements; ++ptr_blockdata) {
    if (*ptr_mydata != *ptr_blockdata)
      return false;
    ptr_mydata++;
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
    ptr_mydata++;
  }
  return block;  //i hope move is used here..
}


void Transformed4DBlock::swap_data_with_block(Block4D& block) {
  auto data_ptr = data.release();
  std::swap(data_ptr, block.mPixelData);
  data.reset(data_ptr);

  for (int t = 0; t < mlength_t; t++) {
    for (int s = 0; s < mlength_s; s++) {
      for (int v = 0; v < mlength_v; v++) {
        block.mPixel[t][s][v] =
            block.mPixelData + block.get_linear_position(t, s, v, 0);
      }
    }
  }
}


/**
 * @brief This function makes a copy from elements in the 4D space to a temporary and linear array of elements.
 * @details [long description]
 * 
 * @param ptr Pointer to the first element to be copied in the temp array.
 * @param ptr_forward_stride How many elements to skip until finding the next one to be copied
 * @param num_elements The total number of elements to be copied
 */
void Transformed4DBlock::copy_values_to_temp(const block4DElementType* ptr,
    std::size_t ptr_forward_stride, std::size_t num_elements) {
  if (ptr_forward_stride == 1) {
    //can't use memcpy because temp is double and ptr is block4DElementType
    std::copy(ptr, ptr + num_elements, temp.get());
  } else {
    for (decltype(temp.get()) i = temp.get(); i < temp.get() + num_elements;
         i++) {
      *i = *ptr;
      ptr += ptr_forward_stride;
    }
  }
}


std::size_t Transformed4DBlock::get_energy() const {
  std::size_t energy = 0;
  auto data_ptr = data.get();
  for (std::size_t i = 0; i < number_of_elements; ++i) {
    energy += std::abs(*data_ptr);
    data_ptr++;
  }
  return energy;
}


void Transformed4DBlock::generic_4d_separable_transform_in_1d(
    block4DElementType* dest, const block4DElementType* src,
    double weight,  //scale
    const double* coefficients, std::size_t max_a, std::size_t max_b,
    std::size_t max_c, std::size_t max_d, std::size_t stride_a,
    std::size_t stride_b, std::size_t stride_c, std::size_t stride_d) {
  auto temp_initial = temp.get();
  auto temp_end = temp_initial + max_d;
  auto total_a_stride = 0;
  for (decltype(max_c) a = 0; a < max_a; a++) {
    auto total_b_stride = 0;
    for (decltype(max_c) b = 0; b < max_b; b++) {
      auto stride = total_a_stride + total_b_stride;
      for (decltype(max_c) c = 0; c < max_c; c++) {
        auto src_ptr = src + stride;
        auto dest_ptr = dest + stride;
        copy_values_to_temp(src_ptr, stride_d, max_d);
        for (decltype(max_c) d = 0; d < max_d; d++) {
          if constexpr (std::is_integral<block4DElementType>()) {
            *dest_ptr = static_cast<block4DElementType>(
                //when std::transform_reduce is available, it should be possible to just swap std::inner_product by std::transform_reduce
                std::round(
                    weight *
                    std::inner_product(
                        temp_initial,  //initial position of the original array
                        temp_end,  //final position of the original array+1
                        coefficients +
                            d * max_d,  //initial position of the multiplication array (coefficients array)
                        0.0  //initial sum value
                        )));
          } else {
            *dest_ptr = static_cast<block4DElementType>(
                weight *
                std::inner_product(
                    temp_initial,  //initial position of the original array
                    temp_end,  //final position of the original array+1
                    coefficients +
                        d * max_d,  //initial position of the multiplication array (coefficients array)
                    0.0  //initial sum value
                    ));
          }
          dest_ptr += stride_d;
        }
        stride += stride_c;
      }
      total_b_stride += stride_b;
    }
    total_a_stride += stride_a;
  }
}


void Transformed4DBlock::do_4d_transform(block4DElementType* dest,
    const block4DElementType* src,
    const std::tuple<const double*, const double*, const double*, const double*>
        coefficients,
    const std::tuple<double, double, double, double> transform_weights =
        std::make_tuple(1.0, 1.0, 1.0, 1.0)) {
  std::size_t stride_u = 1;
  std::size_t stride_v = mlength_u;
  std::size_t stride_s = mlength_v * mlength_u;
  std::size_t stride_t = mlength_s * stride_s;

  using LF = LightFieldDimension;

  //U, V, S, T
  generic_4d_separable_transform_in_1d(dest,
      src,  //using this function avoids performing an extra memcpy
      std::get<LF::U>(transform_weights), std::get<LF::U>(coefficients),
      mlength_t, mlength_s, mlength_v, mlength_u,  //max vals
      stride_t,  //stride_a
      stride_s,  //stride_b
      stride_v,  //stride_c
      stride_u  //stride d
  );
  generic_4d_separable_transform_in_1d(
      dest, dest, std::get<LF::V>(transform_weights),
      std::get<LF::V>(coefficients), mlength_t, mlength_s, mlength_u, mlength_v,
      stride_t,  //stride_a
      stride_s,  //stride_b
      stride_u,  //stride c
      stride_v  //stride_d
  );
  generic_4d_separable_transform_in_1d(
      dest, dest, std::get<LF::S>(transform_weights),
      std::get<LF::S>(coefficients), mlength_t, mlength_v, mlength_u, mlength_s,
      stride_t,  //stride_a
      stride_v,  //stride_b
      stride_u,  //stride c
      stride_s  //stride_d
  );
  generic_4d_separable_transform_in_1d(
      dest, dest, std::get<LF::T>(transform_weights),
      std::get<LF::T>(coefficients), mlength_s, mlength_v, mlength_u, mlength_t,
      stride_s,  //stride_a
      stride_v,  //stride_b
      stride_u,  //stride c
      stride_t  //stride_d
  );
}


void Transformed4DBlock::alloc_temp() {
  temp = std::make_unique<block4DElementType[]>(
      std::max({mlength_u, mlength_v, mlength_s, mlength_t}));
}


void Transformed4DBlock::alloc_data() {
  data = std::make_unique<block4DElementType[]>(number_of_elements);
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


void Transformed4DBlock::set_dimensions(
    int length_t, int length_s, int length_v, int length_u) {
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
    const block4DElementType* transformed_values, int u, int v, int s, int t) {
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
  alloc_temp();
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

