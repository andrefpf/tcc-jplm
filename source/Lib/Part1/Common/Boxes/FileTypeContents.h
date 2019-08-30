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

/** \file     FileTypeContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_PART1_COMMON_BOXES_FILETYPECONTENTS_H__
#define JPLM_LIB_PART1_COMMON_BOXES_FILETYPECONTENTS_H__

#include <algorithm>  //std::find
#include <tuple>  //std::tie
#include "source/Lib/Part1/Common/BinaryTools.h"
#include "source/Lib/Common/Boxes/InMemoryDBoxContents.h"

class FileTypeContents : public InMemoryDBoxContents {
  uint32_t BR;  //! brand
  uint32_t MinV;  //! minor version
  std::vector<uint32_t> CL;  //! compatibility list

 public:
  FileTypeContents(uint32_t brand, uint32_t minor_version,
      const std::vector<uint32_t>& compatibility_list)
      : BR(brand), MinV(minor_version), CL(compatibility_list) {
  }


  FileTypeContents(uint32_t brand, uint32_t minor_version,
      std::vector<uint32_t>&& compatibility_list)
      : BR(brand), MinV(minor_version), CL(std::move(compatibility_list)) {
  }


  FileTypeContents(const FileTypeContents& other)
      : BR(other.BR), MinV(other.MinV), CL(other.CL) {
  }


  FileTypeContents(FileTypeContents&& other)
      : BR(other.BR), MinV(other.MinV), CL(std::move(other.CL)) {
  }


  virtual ~FileTypeContents() = default;


  virtual FileTypeContents* clone() const override {
    return new FileTypeContents(*this);
  }


  uint64_t size() const noexcept override {
    constexpr auto brand_and_minor_version_size = 2 * sizeof(uint32_t);
    return brand_and_minor_version_size + CL.size() * sizeof(uint32_t);
  }


  uint32_t get_brand() const noexcept {
    return BR;
  }


  uint32_t get_minor_version() const noexcept {
    return MinV;
  }


  const std::vector<uint32_t>& get_reference_to_compatibility_list() const {
    return CL;
  }


  void add_compatible_standard_to_list(uint32_t standard_code) {
    CL.push_back(standard_code);
  }


  bool is_the_file_compatible_with(uint32_t standard_code) const noexcept {
    auto result = std::find(CL.begin(), CL.end(), standard_code);
    if (result != CL.end())
      return true;
    return false;
  }


  auto get_number_of_compatible_standards() const noexcept {
    return CL.size();
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const FileTypeContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const FileTypeContents& other) const noexcept {
    return (std::tie(this->BR, this->MinV) == std::tie(other.BR, other.MinV)) &&
           (this->CL == other.CL);
  }


  bool operator!=(const FileTypeContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());

    BinaryTools::append_big_endian_bytes(bytes, BR);
    BinaryTools::append_big_endian_bytes(bytes, MinV);

    for (const auto& compatible_code : CL) {
      BinaryTools::append_big_endian_bytes(bytes, compatible_code);
    }
    return bytes;
  }
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXES_FILETYPECONTENTS_H__ */