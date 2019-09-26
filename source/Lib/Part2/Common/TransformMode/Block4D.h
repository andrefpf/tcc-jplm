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
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_BLOCK4D_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_BLOCK4D_H__

#include <cstdint>
#include <vector>
#include "Lib/Part2/Common/LightfieldDimension.h"

template<typename T = uint32_t, typename DimensionT = uint32_t>
class Block4D {
 protected:
  LightfieldDimension<DimensionT>
      dimension;  //!< Dimension (t, s, v, u) of this Block4D
  std::size_t number_of_elements =
      0;  //!< Number of elements within this 4D block
  std::vector<T> pixels;
 public:
  // Block4D(); //should this be possible?
  Block4D(const LightfieldDimension<DimensionT>& block_dimension)
      : dimension(block_dimension),
        number_of_elements(block_dimension.get_number_of_pixels_per_view()) {
        	pixels.reserve(number_of_elements);
  }


  virtual ~Block4D() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_BLOCK4D_H__ */