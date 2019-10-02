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

/** \file     Block4D.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 *  \note This file must be updated by a new implementation of 4D Block
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_BLOCK4D_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_BLOCK4D_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cstdint>
#include <cstring>  //used for memcpy
#include <functional>
#include <iostream>
#include <vector>
#include "Lib/Part2/Common/LightfieldDimension.h"


#ifndef block4DElementType
#define block4DElementType int32_t
#endif

enum ExtensionMethod {
  SHRINK_TO_FIT,
  ZERO_PADDING,
  REPEAT_LAST,
  CYCLIC,
  EXTENDED_DCT,
  TRANSFORM_DOMAIN_ZERO_PADDING,
  MEAN_VALUE,
  NONE
};

enum LightFieldDimension {
  T = 0,
  S,
  V,
  U
};  //cannot place this in LightField because it creates a circular dependency :(

class Block4D {
 private:
  std::size_t number_of_elements = 0;
  std::size_t number_of_allocated_elements = 0;
  void set_number_of_elements();
  void set_strides();
  void set_lengths(int length_t, int length_s, int length_v, int length_u);
  void release_data();
  void extend_u(int position_u);
  void extend_v(int position_v);
  void extend_s(int position_s);
  void extend_t(int position_t);
  bool has_equal_size(
      int length_t, int length_s, int length_v, int length_u) const;

 public:
  block4DElementType* mPixelData =
      nullptr; /*!< pointer to a linear array of pixel data */
  block4DElementType**** mPixel =
      nullptr; /*!< pointer to a 4 dimensional array of pixel data that accesses the same positions as mPixelData */
  std::size_t stride_t;
  std::size_t stride_s;
  std::size_t stride_v;
  int mlength_t = 0; /*!< t dimension block size */
  int mlength_s = 0; /*!< s dimension block size */
  int mlength_v = 0; /*!< v dimension block size */
  int mlength_u = 0; /*!< u dimension block size */
  Block4D() = default;
  Block4D(Block4D&& other);  //move constructor
  Block4D(const LightfieldDimension<uint32_t>& block_dimension) {
    set_dimension(block_dimension.get_t(), block_dimension.get_s(), block_dimension.get_v(), block_dimension.get_u());
  }
  ~Block4D();
  void set_dimension(int length_t, int length_s, int length_v, int length_u);
  void set_dimension(const std::tuple<int, int, int, int>& lengths);
  std::tuple<int, int, int, int> get_dimension() {
    return std::make_tuple(mlength_t, mlength_s, mlength_v, mlength_u);
  }
  LightfieldDimension<uint32_t> get_dimension() const {
    return {static_cast<uint32_t>(mlength_t), static_cast<uint32_t>(mlength_s), static_cast<uint32_t>(mlength_v), static_cast<uint32_t>(mlength_u)};
  }
  std::size_t get_linear_position(
      int position_t, int position_s, int position_v, int position_u) const;
  void set_pixel_at(block4DElementType pixel_value, int position_t,
      int position_s, int position_v, int position_u);
  block4DElementType get_pixel_at(
      int position_t, int position_s, int position_v, int position_u) const;
  std::size_t get_number_of_elements() const;
  void resize_avoiding_free(
      int length_t, int length_s, int length_v, int length_u);
  void swap_data_with(Block4D& other);
  void copy_sub_block_from(const Block4D& B, int source_offset_t,
      int source_offset_s, int source_offset_v, int source_offset_u,
      int target_offset_t = 0, int target_offset_s = 0, int target_offset_v = 0,
      int target_offset_u = 0);
  void copy_sub_block_from(const Block4D& B,
      const std::tuple<int, int, int, int>& source_offsets,
      const std::tuple<int, int, int, int>& target_offsets = {0, 0, 0, 0});
  void extend(ExtensionMethod extensionMethod, int extensionLength,
      LightFieldDimension direction);
  void clip(block4DElementType minValue, block4DElementType maxValue);
  void shift_data_from_uv_plane_at(int shift, int position_t, int position_s);
  Block4D& operator+=(int const& value);
  Block4D& operator-=(int const& value);
  void operator=(const Block4D& other);  //copy
  Block4D& operator=(Block4D&& other);  //move assignment
  bool has_equal_size(const Block4D& other) const;
  void fill_with_zeros();
  
  friend std::ostream& operator<<(std::ostream& stream, const Block4D& Box);

  

};

std::ostream& operator<<(std::ostream& o_stream, const Block4D& block);

// template<typename T = uint32_t, typename DimensionT = uint32_t>
// class Block4D {
//  protected:
//   LightfieldDimension<DimensionT>
//       dimension;  //!< Dimension (t, s, v, u) of this Block4D
//   std::size_t number_of_elements =
//       0;  //!< Number of elements within this 4D block
//   std::vector<T> pixels;
//  public:
//   // Block4D(); //should this be possible?
//   Block4D(const LightfieldDimension<DimensionT>& block_dimension)
//       : dimension(block_dimension),
//         number_of_elements(block_dimension.get_number_of_pixels_per_view()) {
//         	pixels.reserve(number_of_elements);
//   }


//   virtual ~Block4D() = default;
// };

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_BLOCK4D_H__ */