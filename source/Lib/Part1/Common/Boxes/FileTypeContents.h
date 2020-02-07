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

#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Utils/Stream/BinaryTools.h"
#include <vector>
#include <algorithm>
#include <tuple>


class FileTypeContents : public InMemoryDBox {
  uint32_t BR;  //!< brand
  uint32_t MinV;  //!< minor version
  std::vector<uint32_t> CL;  //!< compatibility list

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


  virtual FileTypeContents* clone() const override;


  uint64_t size() const noexcept override;


  uint32_t get_brand() const noexcept;


  uint32_t get_minor_version() const noexcept;


  const std::vector<uint32_t>& get_reference_to_compatibility_list() const;


  void add_compatible_standard_to_list(uint32_t standard_code);


  bool is_the_file_compatible_with(uint32_t standard_code) const noexcept;


  std::size_t  get_number_of_compatible_standards() const noexcept;


  virtual bool is_equal(const DBox& other) const override;


  bool operator==(const FileTypeContents& other) const noexcept;


  bool operator!=(const FileTypeContents& other) const noexcept;


  virtual std::vector<std::byte> get_bytes() const override;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXES_FILETYPECONTENTS_H__ */