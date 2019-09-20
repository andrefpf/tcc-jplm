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

/** \file     VariablePrecisionFloatingPointCoordinates.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-20
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_VARIABLEPRECISIONFLOATINGPOINTCOORDINATES_H__
#define JPLM_LIB_PART2_COMMON_BOXES_VARIABLEPRECISIONFLOATINGPOINTCOORDINATES_H__

#include <vector>
#include <cstdint>
#include "Lib/Utils/Stream/BinaryTools.h"

/**
 * \brief      Base virtual class for variable precision floating point coordinates.
 */
class VariablePrecisionFloatingPointCoordinates {
 protected:
  //PP = 0, precision = 16 == "short float" //unavailable
  //PP = 1, precision = 32 == float
  //PP = 2, precision = 64 == double
  //PP = 3, precision = 128 == "long double" //unavailable (some implemetation uses 80 bits...)
  // ...
  uint8_t pp;  //!< Precision of coordinates

 public:
  /**
   * \brief      Constructs the object storing the precision
   *
   * \param[in]  pp    Precision. 
   *   <table>  
      <tr><th> PP value <th> Precision (in bits) <th> FP Type
      <tr><td> 0 <td> 16 <td> not available
      <tr><td> 1 <td> 32 <td> <b>float</b>
      <tr><td> 2 <td> 64 <td> <b>double</b>
      <tr><td> 3 <td> 128 <td> not ensured
      </table>
   * 
   */
  VariablePrecisionFloatingPointCoordinates(uint8_t pp) : pp(pp) {
  }

  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     Raw pointer to a copy of this object.
   */
  virtual VariablePrecisionFloatingPointCoordinates* clone() const = 0;


  /**
   * \brief      Gets the pp (precision).
   *
   * \return     The pp.
   */
  auto get_pp() const noexcept {
    return pp;
  }

  /**
   * \brief      Size of the contents (in bytes)
   *
   * \return     The size (in bytes)
   */
  virtual uint64_t size() const noexcept = 0;


  /**
   * \brief      Gets the bytes (std::byte) in the specified order within a std::vector.
   * \details    Each value must be converted to a big endian sequence of bytes.
   *
   * \return     The bytes (vector of bytes).
   */
  virtual std::vector<std::byte> get_bytes() const noexcept = 0;


  virtual VariablePrecisionFloatingPointCoordinates& test() = 0;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_VARIABLEPRECISIONFLOATINGPOINTCOORDINATES_H__ */