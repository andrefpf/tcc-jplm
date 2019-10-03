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

/** \file     ContiguousCodestreamCode.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-29
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODE_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODE_H__

#include <cstddef>  //for std::byte
#include <cstdint>
#include <ostream>

/**
 * \brief      Class for contiguous codestream code.
 */
class ContiguousCodestreamCode {
 public:
  virtual uint64_t size() const noexcept = 0;


  virtual ContiguousCodestreamCode* clone() const = 0;


  virtual bool is_equal(const ContiguousCodestreamCode& other) const = 0;


  virtual std::ostream& write_to(std::ostream& stream) const = 0;


  virtual void push_byte(const std::byte byte) = 0;


  virtual std::byte get_byte_at(const uint64_t pos) const = 0;


  virtual std::byte get_next_byte() const = 0;


  virtual bool is_next_valid() const = 0;


  virtual std::byte peek_next_byte() const = 0;


  bool operator==(const ContiguousCodestreamCode& other) const {
    return this->is_equal(other);
  }


  bool operator!=(const ContiguousCodestreamCode& other) const {
    return !this->operator==(other);
  }

  //! \todo define a iterator interface for ContiguousCodestreamCode
};


std::ostream& operator<<(
    std::ostream& stream, const ContiguousCodestreamCode& code);

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODE_H__ */