/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     UndefinedContents.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-06
 */
#include "Lib/Common/Boxes/Generic/UndefinedContents.h"


UndefinedContents *UndefinedContents::clone() const {
  return new UndefinedContents(*this);
}


bool UndefinedContents::is_equal(const DBox &other) const {
  if (typeid(*this) == typeid(other)) {
    const auto &cast_other = dynamic_cast<const UndefinedContents &>(other);
    return *this == cast_other;
  }
  return false;
}


  uint64_t UndefinedContents::size() const noexcept {
    return byte_array.size();
  }


  bool UndefinedContents::operator==(const UndefinedContents &other) const {
    return this->byte_array == other.byte_array;
  }


  bool UndefinedContents::operator!=(const UndefinedContents &other) const {
    return !this->operator==(other);
  }


  void UndefinedContents::set_bytes(const std::vector<std::byte> &&bytes) {
    byte_array = std::move(bytes);
  }


  void UndefinedContents::set_bytes(const std::vector<std::byte> &bytes) {
    byte_array = bytes;
  }


  void UndefinedContents::add_bytes(const std::vector<std::byte> &bytes) {
    byte_array.insert(byte_array.end(), bytes.begin(), bytes.end());
  }


  std::vector<std::byte> UndefinedContents::get_bytes() const noexcept {
    return byte_array;
  }
