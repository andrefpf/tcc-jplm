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

/** \file     Transformed4DBlock.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-03-19
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_TRANSFORMED4DBLOCK_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_TRANSFORMED4DBLOCK_H__


#include <math.h>  //required for sqrt computation

// #include <stdlib.h>
#include <algorithm>  //for std::max
#include <memory>  //for unique_ptr, make_unique
#include <numeric>  //for inner_product
#include <tuple>
// #include <functional>
// #include <numeric> //if c++17 and using transform_reduce
#include <cstring>
#include <iostream>
#include <utility>
#include "Block4D.h"


class Transformed4DBlock {
 private:
  std::unique_ptr<block4DElementType[]> temp;


  void alloc_temp();
  void alloc_data();
  void alloc_resources();

  void set_number_of_elements();
  void set_dimensions(int length_t, int length_s, int length_v, int length_u);
  void copy_values_to_temp(const block4DElementType* ptr,
      std::size_t ptr_forward_stride, std::size_t num_elements);
  void generic_4d_separable_transform_in_1d(block4DElementType* dest,
      const block4DElementType* src,
      double weight,  //scale
      const double* coefficients, std::size_t max_a, std::size_t max_b,
      std::size_t max_c, std::size_t max_d, std::size_t stride_a,
      std::size_t stride_b, std::size_t stride_c, std::size_t stride_d);

 protected:
  /*The length values will be copied from the Block4D*/
  int mlength_t; /*!< t dimension block size */
  int mlength_s; /*!< s dimension block size */
  int mlength_v; /*!< v dimension block size */
  int mlength_u; /*!< u dimension block size */
  std::size_t number_of_elements;
  std::unique_ptr<block4DElementType[]>
      data; /*!< pointer to a linear array of transformed pixel data */
  std::size_t get_number_of_elements() const;
  Transformed4DBlock(const Block4D& block);
  void do_4d_transform(block4DElementType* dest, const block4DElementType* src,
      const std::tuple<const double*, const double*, const double*,
          const double*>
          coefficients,
      const std::tuple<double, double, double, double> transform_gains);

 public:
  Transformed4DBlock(const Block4D& block,
      const std::tuple<const double*, const double*, const double*,
          const double*>
          coefficients,
      const std::tuple<double, double, double, double> transform_gains);
  Transformed4DBlock(
      const block4DElementType* transformed_values, int u, int v, int s, int t);
  Transformed4DBlock(Block4D&& source);

  //move constructor
  Transformed4DBlock(Transformed4DBlock&& other);

  ~Transformed4DBlock() = default;

  std::size_t get_energy() const;

  bool operator==(const Block4D& block);

  Block4D inverse(const std::tuple<const double*, const double*, const double*,
                      const double*>
                      coefficients,
      const std::tuple<double, double, double, double> transform_gains);

  Block4D generate_copy_in_block() const;
  void swap_data_with_block(Block4D& block);

  decltype(mlength_t) get_t() const {
    return mlength_t;
  }
  decltype(mlength_s) get_s() const {
    return mlength_s;
  }
  decltype(mlength_v) get_v() const {
    return mlength_v;
  }
  decltype(mlength_u) get_u() const {
    return mlength_u;
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_TRANSFORMED4DBLOCK_H__ */