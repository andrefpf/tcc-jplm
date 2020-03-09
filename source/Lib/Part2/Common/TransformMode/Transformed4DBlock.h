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

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_TRANSFORMED4DBLOCKDOUBLE_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_TRANSFORMED4DBLOCKDOUBLE_H__


#include <math.h>  //required for sqrt computation
#include <algorithm>  //for std::max
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>  //for unique_ptr, make_unique
#include <numeric>  //for inner_product
#include <tuple>
#include <utility>
#include "Block4D.h"


class Transformed4DBlock {
 private:
  std::unique_ptr<block4DElementType[]> temp;
  std::unique_ptr<double[]> temp_double;

  void alloc_temp();
  void alloc_data();
  void alloc_resources();

  void set_number_of_elements();
  void set_dimensions(uint32_t length_t, uint32_t length_s, uint32_t length_v,
      uint32_t length_u);

  /**
 * @brief This function makes a copy from elements in the 4D space to a temporary and linear array of elements.
 * @details [long description]
 * 
 * @param ptr Pointer to the first element to be copied in the temp array.
 * @param ptr_forward_stride How many elements to skip until finding the next one to be copied
 * @param num_elements The total number of elements to be copied
 */
  void copy_values_to_temp(const double* ptr, std::size_t ptr_forward_stride,
      std::size_t num_elements) {
    if (ptr_forward_stride == 1) {
      //can't use memcpy because temp is double and ptr is block4DElementType
      std::copy(ptr, ptr + num_elements, temp_double.get());
    } else {
      for (decltype(temp_double.get()) i = temp_double.get();
           i < temp_double.get() + num_elements; ++i) {
        *i = *ptr;
        ptr += ptr_forward_stride;
      }
    }
  }


  void generic_4d_separable_transform_in_1d(double* dest, const double* src,
      double weight,  //scale
      const double* coefficients, std::size_t max_a, std::size_t max_b,
      std::size_t max_c, std::size_t max_d, std::size_t stride_a,
      std::size_t stride_b, std::size_t stride_c, std::size_t stride_d) {
    // std::cout << "weight: " << weight << std::endl;
    auto temp_initial = temp_double.get();
    auto temp_end = temp_initial + max_d;
    auto total_a_stride = 0;
    for (decltype(max_c) a = 0; a < max_a; ++a) {
      auto total_b_stride = 0;
      for (decltype(max_c) b = 0; b < max_b; ++b) {
        auto stride = total_a_stride + total_b_stride;
        for (decltype(max_c) c = 0; c < max_c; ++c) {
          auto src_ptr = src + stride;
          auto dest_ptr = dest + stride;
          copy_values_to_temp(src_ptr, stride_d, max_d);
          for (decltype(max_c) d = 0; d < max_d; ++d) {
            //when std::transform_reduce is available, it should be possible to
            //just swap std::inner_product by std::transform_reduce
            *dest_ptr =
                weight *
                std::inner_product(
                    temp_initial,  //initial position of the original array
                    temp_end,  //final position of the original array+1
                    coefficients +
                        d * max_d,  //initial position of the multiplication array (coefficients array)
                    0.0  //initial sum value
                );
            dest_ptr += stride_d;
          }
          stride += stride_c;
        }
        total_b_stride += stride_b;
      }
      total_a_stride += stride_a;
    }
  }


 protected:
  /*The length values will be copied from the Block4D*/
  uint32_t mlength_t; /*!< t dimension block size */
  uint32_t mlength_s; /*!< s dimension block size */
  uint32_t mlength_v; /*!< v dimension block size */
  uint32_t mlength_u; /*!< u dimension block size */
  std::size_t number_of_elements;
  std::unique_ptr<block4DElementType[]>
      data; /*!< pointer to a linear array of transformed pixel data */
  std::unique_ptr<double[]> data_double;
  std::size_t get_number_of_elements() const;
  Transformed4DBlock(const Block4D& block);


  template<typename dest_t, typename src_t>
  void do_4d_transform(dest_t* dest, const src_t* src,
      const std::tuple<const double*, const double*, const double*,
          const double*>
          coefficients,
      const std::tuple<double, double, double, double> transform_weights =
          std::make_tuple(1.0, 1.0, 1.0, 1.0)) {
    std::size_t stride_u = 1;
    std::size_t stride_v = mlength_u;
    std::size_t stride_s = mlength_v * mlength_u;
    std::size_t stride_t = mlength_s * stride_s;

    using LF = LightFieldDimension;

    auto data_double_ptr = data_double.get();
    for (decltype(number_of_elements) e = 0; e < number_of_elements; ++e) {
      *(data_double_ptr++) = static_cast<double>(*(src++));
    }

    // //U, V, S, T
    generic_4d_separable_transform_in_1d(data_double.get(),
        data_double
            .get(),  //using this function avoids performing an extra memcpy
        std::get<LF::U>(transform_weights), std::get<LF::U>(coefficients),
        mlength_t, mlength_s, mlength_v, mlength_u,  //max vals
        stride_t,  //stride_a
        stride_s,  //stride_b
        stride_v,  //stride_c
        stride_u  //stride d
    );

    generic_4d_separable_transform_in_1d(data_double.get(), data_double.get(),
        std::get<LF::V>(transform_weights), std::get<LF::V>(coefficients),
        mlength_t, mlength_s, mlength_u, mlength_v,
        stride_t,  //stride_a
        stride_s,  //stride_b
        stride_u,  //stride c
        stride_v  //stride_d
    );

    generic_4d_separable_transform_in_1d(data_double.get(), data_double.get(),
        std::get<LF::S>(transform_weights), std::get<LF::S>(coefficients),
        mlength_t, mlength_v, mlength_u, mlength_s,
        stride_t,  //stride_a
        stride_v,  //stride_b
        stride_u,  //stride c
        stride_s  //stride_d
    );

    generic_4d_separable_transform_in_1d(data_double.get(), data_double.get(),
        std::get<LF::T>(transform_weights), std::get<LF::T>(coefficients),
        mlength_s, mlength_v, mlength_u, mlength_t,
        stride_s,  //stride_a
        stride_v,  //stride_b
        stride_u,  //stride c
        stride_t  //stride_d
    );

    data_double_ptr = data_double.get();
    for (decltype(number_of_elements) e = 0; e < number_of_elements; ++e) {
      *(dest++) = static_cast<dest_t>(std::round(*(data_double_ptr++)));
    }
  }


 public:
  Transformed4DBlock(const Block4D& block,
      const std::tuple<const double*, const double*, const double*,
          const double*>
          coefficients,
      const std::tuple<double, double, double, double> transform_gains);


  Transformed4DBlock(const block4DElementType* transformed_values, uint32_t u,
      uint32_t v, uint32_t s, uint32_t t);


  Transformed4DBlock(Block4D&& source);

  //move constructor
  Transformed4DBlock(Transformed4DBlock&& other);


  virtual ~Transformed4DBlock() = default;


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

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_TRANSFORMED4DBLOCKDOUBLE_H__ */