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

/** \file     JpegPlenoSignatureContents.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Ismael Seidel <i.seidel@samsung.com> 
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-07
 */

#include "Lib/Part1/Common/Boxes/JpegPlenoSignatureContents.h"


JpegPlenoSignatureContents *JpegPlenoSignatureContents::clone() const {
  return new JpegPlenoSignatureContents(*this);
}


uint64_t JpegPlenoSignatureContents::size() const noexcept {
  return 4;
}


bool JpegPlenoSignatureContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other =
      dynamic_cast<const JpegPlenoSignatureContents &>(other);
  return *this == cast_other;
}


bool JpegPlenoSignatureContents::operator==(
    const JpegPlenoSignatureContents &other) const {
  return (this->signature == other.signature);
}


bool JpegPlenoSignatureContents::operator!=(
    const JpegPlenoSignatureContents &other) const {
  return !this->operator==(other);
}


bool JpegPlenoSignatureContents::is_valid(const std::vector<std::byte> &bytes) {
  if (bytes.size() == 4) {
    if ((bytes[0] == std::byte{0x0d}) && (bytes[1] == std::byte{0x0a}) &&
        (bytes[2] == std::byte{0x87}) && (bytes[3] == std::byte{0x0a})) {
      return true;
    }
  }
  return false;
}


const std::array<std::byte, 4>
    &JpegPlenoSignatureContents::get_ref_to_signature() const noexcept {
  return signature;
}


std::vector<std::byte> JpegPlenoSignatureContents::get_bytes() const {
  return std::vector<std::byte>(signature.begin(), signature.end());
}
