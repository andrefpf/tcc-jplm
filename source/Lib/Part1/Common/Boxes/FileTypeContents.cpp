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

/** \file     FileTypeContents.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-07
 */

#include "Lib/Part1/Common/Boxes/FileTypeContents.h"


FileTypeContents *FileTypeContents::clone() const {
  return new FileTypeContents(*this);
}


uint64_t FileTypeContents::size() const noexcept {
  constexpr auto brand_and_minor_version_size = 2 * sizeof(uint32_t);
  return brand_and_minor_version_size + CL.size() * sizeof(uint32_t);
}


uint32_t FileTypeContents::get_brand() const noexcept {
  return BR;
}


uint32_t FileTypeContents::get_minor_version() const noexcept {
  return MinV;
}


const std::vector<uint32_t>
    &FileTypeContents::get_reference_to_compatibility_list() const {
  return CL;
}


void FileTypeContents::add_compatible_standard_to_list(uint32_t standard_code) {
  CL.push_back(standard_code);
}


bool FileTypeContents::is_the_file_compatible_with(uint32_t standard_code) const
    noexcept {
  auto result = std::find(CL.begin(), CL.end(), standard_code);
  if (result != CL.end())
    return true;
  return false;
}


std::size_t FileTypeContents::get_number_of_compatible_standards() const
    noexcept {
  return CL.size();
}


bool FileTypeContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other = dynamic_cast<const FileTypeContents &>(other);
  return *this == cast_other;
}


bool FileTypeContents::operator==(const FileTypeContents &other) const
    noexcept {
  return (std::tie(this->BR, this->MinV) == std::tie(other.BR, other.MinV)) &&
         (this->CL == other.CL);
}


bool FileTypeContents::operator!=(const FileTypeContents &other) const
    noexcept {
  return !this->operator==(other);
}


std::vector<std::byte> FileTypeContents::get_bytes() const {
  auto bytes = std::vector<std::byte>();
  bytes.reserve(this->size());

  BinaryTools::append_big_endian_bytes(bytes, BR);
  BinaryTools::append_big_endian_bytes(bytes, MinV);

  for (const auto &compatible_code : CL) {
    BinaryTools::append_big_endian_bytes(bytes, compatible_code);
  }
  return bytes;
}
