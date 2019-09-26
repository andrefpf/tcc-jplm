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

/** \file     ProbabilityModel.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_PROBABILITYMODEL_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_PROBABILITYMODEL_H__

#include <stdio.h>
#include <cmath>
#include <cstdint>

class ProbabilityModel {
 private:
  double mRate[2]; /*> rates needed to encode each symbol.*/
  uint16_t frequency_of_zeros;
  uint16_t frequency_of_ones;
  static constexpr decltype(frequency_of_zeros) MAXFREQUENCY = 4095;

 public:
  ProbabilityModel();
  ~ProbabilityModel() = default;
  void update(bool bit);
  void update();
  template<bool bit>
  void update() {
    if (!bit)
      frequency_of_zeros++;
    frequency_of_ones++;
    update();
  };
  void reset();
  auto get_frequency_of_zeros() const noexcept;
  auto get_frequency_of_ones() const noexcept;
  ProbabilityModel& operator=(const ProbabilityModel& other);
  template<bool bit>
  double get_rate() const {
    double frequency_of_bit;
    if constexpr (bit) {
      frequency_of_bit =
          static_cast<double>(frequency_of_ones - frequency_of_zeros);
    } else {
      frequency_of_bit = static_cast<double>(frequency_of_zeros);
    }
    return -log2(frequency_of_bit / static_cast<double>(frequency_of_ones));
  };
  double get_rate(bool bit) const;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_PROBABILITYMODEL_H__ */