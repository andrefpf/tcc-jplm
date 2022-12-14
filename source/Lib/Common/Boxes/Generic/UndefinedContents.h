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

/** \file     UndefinedContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-25
 */


#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_UNDEFINEDCONTENTS_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_UNDEFINEDCONTENTS_H__

#include "Lib/Common/Boxes/InMemoryDBox.h"


class UndefinedContents : public InMemoryDBox {
 protected:
  std::vector<std::byte> byte_array;

 public:
  UndefinedContents(const std::vector<std::byte>& byte_array)
      : byte_array(byte_array) {
  }


  UndefinedContents(std::vector<std::byte>&& byte_array)
      : byte_array(std::move(byte_array)) {
  }


  UndefinedContents(const UndefinedContents& other)
      : byte_array(other.byte_array) {
  }


  UndefinedContents(UndefinedContents&& other)
      : byte_array(std::move(other.byte_array)) {
  }


  UndefinedContents() = default;


  virtual UndefinedContents* clone() const override;


  virtual bool is_equal(const DBox& other) const override;


  ~UndefinedContents() = default;


  uint64_t size() const noexcept override;


  bool operator==(const UndefinedContents& other) const;


  bool operator!=(const UndefinedContents& other) const;


  void set_bytes(const std::vector<std::byte>&& bytes);


  void set_bytes(const std::vector<std::byte>& bytes);


  void add_bytes(const std::vector<std::byte>& bytes);


  virtual std::vector<std::byte> get_bytes() const noexcept override;
};


#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_UNDEFINEDCONTENTS_H__ */