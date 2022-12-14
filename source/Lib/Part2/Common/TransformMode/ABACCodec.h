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

/** \file     ABACCodec.h
 *  \brief    
 *  \details  
 *  \author   Murilo Bresciani
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_ABACCODEC_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_ABACCODEC_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef>
#include <cstdint>
#include <iomanip>  // for leading zeros in hex cout
#include <iostream>
#include <limits>


class ABACCodec {
 protected:
  static constexpr uint16_t MAXINT =
      0xffff;  //std::numeric_limits<uint32_t>::max();
  static constexpr uint16_t RESET_LSB_MASK = 0xfffe;
  static constexpr uint16_t SET_LSB_MASK = 0x0001;
  static constexpr uint16_t INTERVAL_PRECISION = 16;
  static constexpr uint16_t MSB_MASK = 0x8000;
  static constexpr uint16_t SECOND_MSB_MASK = 0x4000;
  static constexpr uint16_t TWO_MSBS_MASK = MSB_MASK + SECOND_MSB_MASK;
  uint16_t mLow; /*!< interval lower limit */
  uint16_t mHigh; /*!< interval upper limit */
  int number_of_bits_in_byte; /*!< number of valid bits in buffer */
  // unsigned char mBitBuffer; /*!< bit-readable buffer */
  std::byte byte_buffer;
  FILE *file_ptr = nullptr; /*!< pointer to file */
 public:
  ABACCodec()
      : mLow(0), mHigh(MAXINT), number_of_bits_in_byte(0),
        byte_buffer(std::byte{0}) {
  }

  void print_byte(std::byte byte) const {
    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex
              << std::to_integer<uint16_t>(byte) << std::dec << std::endl;
  }

  virtual ~ABACCodec() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_ABACCODEC_H__ */