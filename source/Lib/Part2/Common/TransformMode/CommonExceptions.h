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

/** \file     CommonExceptions.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-26
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_COMMON_EXCEPTIONS_H
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_COMMON_EXCEPTIONS_H

#include <cinttypes>
#include <exception>
#include <string>

namespace LightFieldConfigurationMarker {
class OverflowInTheNumberOf4DBlocks : public std::exception {
 private:
  std::string message_;

 public:
  explicit OverflowInTheNumberOf4DBlocks(uint64_t n_4d) {
    message_ = "Overflow in the number of 4D blocks (N_4D). There are " +
               std::to_string(n_4d) +
               " blocks for the current configuration, but only " +
               std::to_string(std::numeric_limits<uint32_t>::max()) +
               " blocks are supported by the standard.";
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }
};
}  // namespace LightFieldConfigurationMarker

#endif  // JPLM_LIB_PART2_COMMON_TRANSFORMMODE_COMMON_EXCEPTIONS_H