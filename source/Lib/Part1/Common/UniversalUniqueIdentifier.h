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

/** \file     UniversalUniqueIdentifier.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-25
 */

#ifndef JPLM_LIB_PART1_COMMON_UNIVERSALUNIQUEIDENTIFIER_H__
#define JPLM_LIB_PART1_COMMON_UNIVERSALUNIQUEIDENTIFIER_H__

#include <iomanip>  //std::setfill, std::setw
#include <iostream>
#include <sstream>
#include <tuple>

struct my_uint48_t {
  uint16_t hi;
  uint64_t lo;

  bool operator==(const my_uint48_t& other) const {
    return (this->hi == other.hi) && (this->lo == other.lo);
  }

  bool operator!=(const my_uint48_t& other) const {
    return !this->operator==(other);
  }
};


class UniversalUniqueIdentifier {
 protected:
  my_uint48_t node;
  uint8_t clock_seq_low;
  uint8_t clock_seq_hi_and_reserved;
  uint16_t time_hi_and_version;
  uint16_t time_mid;
  uint32_t time_low;

 public:
  UniversalUniqueIdentifier()
      : node({0, 0}), clock_seq_low(0), clock_seq_hi_and_reserved(0),
        time_hi_and_version(0), time_mid(0), time_low(0) {
  }

  UniversalUniqueIdentifier(my_uint48_t node, uint8_t clock_seq_low,
      uint8_t clock_seq_hi_and_reserved, uint16_t time_hi_and_version,
      uint16_t time_mid, uint32_t time_low)
      : node(node), clock_seq_low(clock_seq_low),
        clock_seq_hi_and_reserved(clock_seq_hi_and_reserved),
        time_hi_and_version(time_hi_and_version), time_mid(time_mid),
        time_low(time_low) {
  }


  UniversalUniqueIdentifier(const std::string& hex_string) {
    //! \todo implement
    //static_assert(false, "Not implemented");
    std::cerr << "Contructor not implemented yet << " << hex_string
              << std::endl;
  }


  ~UniversalUniqueIdentifier() = default;


  bool operator==(const UniversalUniqueIdentifier& other) const {
    return std::tie(this->node, this->clock_seq_low,
               this->clock_seq_hi_and_reserved, this->time_hi_and_version,
               this->time_mid, this->time_low) ==
           std::tie(other.node, other.clock_seq_low,
               other.clock_seq_hi_and_reserved, other.time_hi_and_version,
               other.time_mid, other.time_low);
  }


  bool operator!=(const UniversalUniqueIdentifier& other) const {
    return !this->operator==(other);
  }


  uint64_t size() const noexcept {
    return 16;
    //16 = 128 bits
  }


  std::string to_hex_string() {
    std::stringstream string_stream;
    string_stream << std::hex << std::setfill('0') << std::setw(8)
                  << (int) time_low << "-" << std::setw(4) << (int) time_mid << "-"
                  << std::setw(4) << (int) time_hi_and_version << "-"
                  << std::setw(2) << (int) clock_seq_hi_and_reserved << std::setw(2) << (int) clock_seq_low
                  << "-" << std::setw(2) << (int) node.hi << std::setw(10) << (int) node.lo;
    return string_stream.str();
  }
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_UNIVERSALUNIQUEIDENTIFIER_H__ */