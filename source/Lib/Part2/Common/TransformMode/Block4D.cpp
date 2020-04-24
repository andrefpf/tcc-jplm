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

/** \file     Block4D.cpp
 *  \brief    
 *  \details  
 *  \author    Murilo Bresciani
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */


#include "Block4D.h"

/*******************************************************************************/
/*                         Block4D class methods                               */
/*******************************************************************************/

void Block4D::set_number_of_elements() {
  number_of_elements = mlength_u * mlength_v * mlength_s * mlength_t;
}


std::size_t Block4D::get_number_of_elements() const {
  return number_of_elements;
}


Block4D::~Block4D() {
  set_dimension(0, 0, 0, 0);
}


void Block4D::resize_avoiding_free(uint32_t length_t, uint32_t length_s,
    uint32_t length_v, uint32_t length_u) {
  if (has_equal_size(length_t, length_s, length_v, length_u))
    return;

  // return set_dimension(length_t, length_s, length_v, length_u);

  if (static_cast<std::size_t>(length_u * length_v * length_s * length_t) >
      number_of_allocated_elements)
    return set_dimension(length_t, length_s, length_v, length_u);

  set_lengths(length_t, length_s, length_v, length_u);

  if (number_of_elements != 0) {
    for (auto t = decltype(mlength_t){0}; t < mlength_t; ++t) {
      for (auto s = decltype(mlength_s){0}; s < mlength_s; ++s) {
        for (auto v = decltype(mlength_v){0}; v < mlength_v; ++v) {
          mPixel[t][s][v] = &mPixelData[get_linear_position(t, s, v, 0)];
        }
      }
    }
  }
  //warning, this may lead to memory leaks
}


void Block4D::release_data() {
  if (mPixelData != nullptr) {
    for (auto t_index = decltype(mlength_t){0}; t_index < mlength_t;
         ++t_index) {
      for (auto s_index = decltype(mlength_s){0}; s_index < mlength_s;
           ++s_index) {
        delete[] mPixel[t_index][s_index];
      }
      delete[] mPixel[t_index];
    }
    delete[] mPixel;
    delete[] mPixelData;
    mPixelData = nullptr;
  }
}


void Block4D::set_strides() {
  stride_v = mlength_u;
  stride_s = mlength_u * mlength_v;
  stride_t = mlength_u * mlength_v * mlength_s;
}


void Block4D::set_lengths(uint32_t length_t, uint32_t length_s,
    uint32_t length_v, uint32_t length_u) {
  mlength_u = length_u;
  mlength_v = length_v;
  mlength_s = length_s;
  mlength_t = length_t;
  set_strides();
  set_number_of_elements();
}


void Block4D::set_dimension(uint32_t length_t, uint32_t length_s,
    uint32_t length_v, uint32_t length_u) {
  /*! used to allocate memory and set the 4 dimensional array of pointers to manipulate a 4 dimensional block of pixels */
  release_data();

  set_lengths(length_t, length_s, length_v, length_u);

  number_of_allocated_elements = number_of_elements;

  if (number_of_elements != 0) {
    mPixelData = new block4DElementType[number_of_elements];
    mPixel = new block4DElementType***[mlength_t];
    for (auto t = decltype(mlength_t){0}; t < mlength_t; ++t) {
      mPixel[t] = new block4DElementType**[mlength_s];
      for (auto s = decltype(mlength_s){0}; s < mlength_s; ++s) {
        mPixel[t][s] = new block4DElementType*[mlength_v];
        for (auto v = decltype(mlength_v){0}; v < mlength_v; ++v) {
          mPixel[t][s][v] = &mPixelData[get_linear_position(t, s, v, 0)];
        }
      }
    }
  }
}


void Block4D::set_dimension(
    const std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>& lengths) {
  using LF = LightFieldDimension;
  set_dimension(std::get<LF::T>(lengths), std::get<LF::S>(lengths),
      std::get<LF::V>(lengths), std::get<LF::U>(lengths));
}


Block4D& Block4D::operator+=(int const& value) {
  std::transform(mPixelData, mPixelData + number_of_elements, mPixelData,
      [value](
          block4DElementType a) -> block4DElementType { return a + value; });
  return *this;
}


Block4D& Block4D::operator-=(int const& value) {
  std::transform(mPixelData, mPixelData + number_of_elements, mPixelData,
      [value](
          block4DElementType a) -> block4DElementType { return a - value; });
  return *this;
}


Block4D& Block4D::operator*=(double const& value) {
  std::transform(mPixelData, mPixelData + number_of_elements, mPixelData,
      [value](block4DElementType a) -> block4DElementType {
        return std::round(a * value);
      });
  return *this;
}


Block4D& Block4D::operator/=(double const& value) {
  std::transform(mPixelData, mPixelData + number_of_elements, mPixelData,
      [value](block4DElementType a) -> block4DElementType {
        return std::round(a / value);
      });
  return *this;
}


void Block4D::operator=(const Block4D& other) {
  set_dimension(
      other.mlength_t, other.mlength_s, other.mlength_v, other.mlength_u);
  std::memcpy(mPixelData, other.mPixelData,
      number_of_elements * sizeof(block4DElementType));
}


void Block4D::swap_data_with(Block4D& other) {  //assumes the sizes are equal...
  std::swap(mPixelData, other.mPixelData);
  std::swap(mPixel, other.mPixel);
}


//move constructor
Block4D::Block4D(Block4D&& other) {
  set_lengths(
      other.mlength_t, other.mlength_s, other.mlength_v, other.mlength_u);
  number_of_elements = other.number_of_elements;
  //getting ownership over pointers
  mPixelData = other.mPixelData;
  mPixel = other.mPixel;
  other.mPixelData = nullptr;
  other.mPixel = nullptr;
}


bool Block4D::has_equal_size(uint32_t length_t, uint32_t length_s,
    uint32_t length_v, uint32_t length_u) const {
  if (length_t != mlength_t)
    return false;
  if (length_s != mlength_s)
    return false;
  if (length_v != mlength_v)
    return false;
  if (length_u != mlength_u)
    return false;
  return true;
}


bool Block4D::has_equal_size(const Block4D& other) const {
  return has_equal_size(
      other.mlength_t, other.mlength_s, other.mlength_v, other.mlength_u);
}


//move assignment
Block4D& Block4D::operator=(Block4D&& other) {
  if (&other == this)
    return *this;
  // set_dimension(0, 0, 0, 0);
  release_data();
  set_lengths(
      other.mlength_t, other.mlength_s, other.mlength_v, other.mlength_u);
  number_of_elements = other.number_of_elements;
  //getting ownership over pointers
  mPixelData = other.mPixelData;
  mPixel = other.mPixel;
  other.mPixelData = nullptr;
  other.mPixel = nullptr;
  return *this;
}


void Block4D::copy_sub_block_from(const Block4D& B, std::size_t source_offset_t,
    std::size_t source_offset_s, std::size_t source_offset_v,
    std::size_t source_offset_u, std::size_t target_offset_t,
    std::size_t target_offset_s, std::size_t target_offset_v,
    std::size_t target_offset_u) {
  /*! copy data from the 4DBlock B starting from pixel position at source_offset_t, source_offset_s, source_offset_v and source_offset_u
 to the THIS 4DBlock starting from pixel position at target_offset_t, target_offset_s, target_offset_v and target_offset_u */
  auto length_t =
      std::min(mlength_t - target_offset_t, B.mlength_t - source_offset_t);
  auto length_s =
      std::min(mlength_s - target_offset_s, B.mlength_s - source_offset_s);
  auto length_v =
      std::min(mlength_v - target_offset_v, B.mlength_v - source_offset_v);
  auto length_u =
      std::min(mlength_u - target_offset_u, B.mlength_u - source_offset_u);
  for (auto index_t = decltype(length_t){0}; index_t < length_t; ++index_t) {
    for (auto index_s = decltype(length_t){0}; index_s < length_s; ++index_s) {
      for (auto index_v = decltype(length_t){0}; index_v < length_v;
           ++index_v) {
        std::memcpy(
            &mPixel[index_t + target_offset_t][index_s + target_offset_s]
                   [index_v + target_offset_v][target_offset_u],
            &B.mPixel[index_t + source_offset_t][index_s + source_offset_s]
                     [index_v + source_offset_v][source_offset_u],
            length_u * sizeof(block4DElementType));
      }
    }
  }
}


void Block4D::copy_sub_block_from(const Block4D& B,
    const std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>&
        source_offsets,
    const std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>&
        target_offsets) {
  using LF = LightFieldDimension;
  copy_sub_block_from(B, std::get<LF::T>(source_offsets),
      std::get<LF::S>(source_offsets), std::get<LF::V>(source_offsets),
      std::get<LF::U>(source_offsets), std::get<LF::T>(target_offsets),
      std::get<LF::S>(target_offsets), std::get<LF::V>(target_offsets),
      std::get<LF::U>(target_offsets));
}


/*! Sets every pixel of this block to zero */
void Block4D::fill_with_zeros() {
  std::memset(mPixelData, 0, number_of_elements * sizeof(block4DElementType));
}


void Block4D::extend(ExtensionMethod extensionMethod, uint32_t extensionLength,
    LightFieldDimension direction) {
  if (extensionMethod == REPEAT_LAST) {
    switch (direction) {
      case LightFieldDimension::T:
        extend_t(extensionLength - 1);
        return;
      case LightFieldDimension::S:
        extend_s(extensionLength - 1);
        return;
      case LightFieldDimension::V:
        extend_v(extensionLength - 1);
        return;
      case LightFieldDimension::U:
        extend_u(extensionLength - 1);
        return;
    }
  }
  if (extensionMethod == CYCLIC) {
    switch (direction) {
      case LightFieldDimension::T:
        copy_sub_block_from(*this, 0, 0, 0, 0, extensionLength, 0, 0, 0);
        return;
      case LightFieldDimension::S:
        copy_sub_block_from(*this, 0, 0, 0, 0, 0, extensionLength, 0, 0);
        return;
      case LightFieldDimension::V:
        copy_sub_block_from(*this, 0, 0, 0, 0, 0, 0, extensionLength, 0);
        return;
      case LightFieldDimension::U:
        copy_sub_block_from(*this, 0, 0, 0, 0, 0, 0, 0, extensionLength);
        return;
    }
  }
}


//The extend function could be more efficient if using raw pointer access
void Block4D::extend_u(uint32_t position_u) {
  /*! extend this block by repeating the pixel at position_u in the u dimension from that position to the end of the block*/
  if (position_u >= mlength_u - 1) {
    return;
  }
  auto number_of_elements_to_copy = mlength_u - position_u - 1;
  for (auto t = decltype(mlength_t){0}; t < mlength_t; ++t) {
    for (auto s = decltype(mlength_s){0}; s < mlength_s; ++s) {
      for (auto v = decltype(mlength_v){0}; v < mlength_v; ++v) {
        std::fill_n(&mPixel[t][s][v][position_u + 1],
            number_of_elements_to_copy, mPixel[t][s][v][position_u]);
      }
    }
  }
}


void Block4D::extend_v(uint32_t position_v) {
  /*! extend this block by repeating the pixel at position_v in the v dimension from that position to the end of the block*/
  if (position_v >= mlength_v - 1) {
    return;
  }
  for (auto t = decltype(mlength_t){0}; t < mlength_t; ++t) {
    for (auto s = decltype(mlength_s){0}; s < mlength_s; ++s) {
      for (auto v = decltype(mlength_v){position_v + 1}; v < mlength_v; ++v) {
        for (auto u = decltype(mlength_u){0}; u < mlength_u; ++u) {
          mPixel[t][s][v][u] = mPixel[t][s][position_v][u];
        }
      }
    }
  }
}


void Block4D::extend_s(uint32_t position_s) {
  /*! extend this block by repeating the pixel at position_s in the s dimension from that position to the end of the block*/
  if (position_s >= mlength_s - 1) {
    return;
  }
  for (auto t = decltype(mlength_t){0}; t < mlength_t; ++t) {
    for (auto s = decltype(mlength_s){position_s + 1}; s < mlength_s; ++s) {
      for (auto v = decltype(mlength_v){0}; v < mlength_v; ++v) {
        for (auto u = decltype(mlength_u){0}; u < mlength_u; ++u) {
          mPixel[t][s][v][u] = mPixel[t][position_s][v][u];
        }
      }
    }
  }
}


void Block4D::extend_t(uint32_t position_t) {
  /*! extend this block by repeating the pixel at position_t in the t dimension from that position to the end of the block*/
  if (position_t >= mlength_t - 1) {
    return;
  }
  for (auto t = decltype(mlength_t){position_t + 1}; t < mlength_t; ++t) {
    for (auto s = decltype(mlength_s){0}; s < mlength_s; ++s) {
      for (auto v = decltype(mlength_v){0}; v < mlength_v; ++v) {
        for (auto u = decltype(mlength_u){0}; u < mlength_u; ++u) {
          mPixel[t][s][v][u] = mPixel[position_t][s][v][u];
        }
      }
    }
  }
}


void Block4D::clip(block4DElementType minValue, block4DElementType maxValue) {
  /*! restrict pixels values to the interval [minValue, maxValue] */
  std::transform(mPixelData, mPixelData + number_of_elements, mPixelData,
      [minValue, maxValue](block4DElementType value) -> block4DElementType {
        if (value < minValue) {
          return minValue;
        }
        if (value > maxValue) {
          return maxValue;
        }
        return value;
      });
}


void Block4D::shift_data_from_uv_plane_at(
    int shift, uint32_t position_t, uint32_t position_s) {
  /*! The pixel values of the UV plane at st coordinates (position_t, position_s) are left shifted by shift bits if shift is positive,
 and right shifted by -shift if shift is negative */
  if (shift == 0)
    return;
  std::size_t number_of_elements_in_uv_planes = mlength_v * mlength_u;
  block4DElementType* initial_element =
      mPixelData +
      number_of_elements_in_uv_planes * (position_t * mlength_s + position_s);

  std::function<block4DElementType(block4DElementType)> used_shift;
  if (shift < 0) {  //i.e, negative
    shift *= -1;
    used_shift = [shift](block4DElementType value) -> block4DElementType {
      return value >> shift;
    };
  } else {
    used_shift = [shift](block4DElementType value) -> block4DElementType {
      return value << shift;
    };
  }

  std::transform(initial_element,
      initial_element + number_of_elements_in_uv_planes, initial_element,
      used_shift);
}


std::size_t Block4D::get_linear_position(
    const LightfieldCoordinate<uint32_t>& position) const {
  return position.get_t() * stride_t + position.get_s() * stride_s +
         position.get_v() * stride_v + position.get_u();
}


std::size_t Block4D::get_linear_position(uint32_t position_t,
    uint32_t position_s, uint32_t position_v, uint32_t position_u) const {
  return this->get_linear_position(
      {position_t, position_s, position_v, position_u});
}


block4DElementType Block4D::get_pixel_at(
    const LightfieldCoordinate<uint32_t>& position) const {
  return *(mPixelData + get_linear_position(position));
}


block4DElementType Block4D::get_pixel_at(uint32_t position_t,
    uint32_t position_s, uint32_t position_v, uint32_t position_u) const {
  return this->get_pixel_at({position_t, position_s, position_v, position_u});
}


void Block4D::set_pixel_at(block4DElementType pixel_value, uint32_t position_t,
    uint32_t position_s, uint32_t position_v, uint32_t position_u) {
  *(mPixelData + get_linear_position(position_t, position_s, position_v,
                     position_u)) = pixel_value;
}


std::ostream& operator<<(std::ostream& o_stream, const Block4D& block) {
  o_stream << "## Block4D ##" << std::endl;
  for (auto t = decltype(block.mlength_t){0}; t < block.mlength_t; ++t) {
    for (auto s = decltype(block.mlength_s){0}; s < block.mlength_s; ++s) {
      for (auto v = decltype(block.mlength_v){0}; v < block.mlength_v; ++v) {
        for (auto u = decltype(block.mlength_u){0}; u < block.mlength_u; ++u) {
          o_stream << block.mPixel[t][s][v][u] << " ";
        }
        o_stream << std::endl;
      }
      o_stream << std::endl;
    }
    o_stream << std::endl;
  }
  // for(auto i=decltype(block.number_of_elements){0}; i<block.number_of_elements;++i) {
  //   o_stream << i << ": " << block.mPixelData[i] << "\n";
  // }
  o_stream << "END Block4D" << std::endl;
  return o_stream;
}


Block4D::Block4D(const LightfieldDimension<uint32_t>& block_dimension) {
  set_dimension(block_dimension.get_t(), block_dimension.get_s(),
      block_dimension.get_v(), block_dimension.get_u());
}
