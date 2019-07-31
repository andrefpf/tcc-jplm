
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

/** \file     DataEntryURLBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-26
 */

#ifndef JPLM_LIB_PART1_COMMON_DATAENTRYURLBOX_H__
#define JPLM_LIB_PART1_COMMON_DATAENTRYURLBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"
#include "UniversalUniqueIdentifier.h"


struct my_uint24_t {
  uint8_t hi;
  uint16_t lo;

  bool operator==(const my_uint24_t& other) const {
    return (this->hi == other.hi) &&  (this->lo == other.lo);
  }

   bool operator!=(const my_uint24_t& other) const {
    return !this->operator==(other);
  }
};

class DataEntryURLBoxContents : public DBoxContents {
 protected:
  uint8_t vers;  //version number
  my_uint24_t flag;  //flags
  std::string loc;  //location (the url)

 public:
  DataEntryURLBoxContents() = default;
  ~DataEntryURLBoxContents() = default;

  uint64_t size() const noexcept {
    return 4 + loc.size() + 1;
    //4 for ver and location + the size of the string + the null termination char
  }


  bool operator==(const DataEntryURLBoxContents& other) const {
    return std::tie(this->vers, this->flag, this->loc) ==
           std::tie(other.vers, other.flag, other.loc);
  }


  bool operator!=(const DataEntryURLBoxContents& other) const {
    return !this->operator==(other);
  }

  uint8_t get_version_number() const noexcept {
    return vers;
  }

  my_uint24_t get_flag() const noexcept {
    return flag;
  }

  const char* get_location() const noexcept {
    return loc.c_str();
  }
};


class DataEntryURLDBox : public DBox {
 public:
  DataEntryURLDBox(const DataEntryURLBoxContents& contents)
      : DBox(std::make_any<DataEntryURLBoxContents>(contents)) {
  }


  DataEntryURLDBox(const DataEntryURLDBox& other)
      : DBox(std::make_any<DataEntryURLBoxContents>(
            std::any_cast<DataEntryURLBoxContents>(other.contents))) {
  }


  ~DataEntryURLDBox() = default;


  uint64_t size() const noexcept override {
    return std::any_cast<DataEntryURLBoxContents>(this->contents).size();
  }


  DataEntryURLDBox* clone() const override {
    return new DataEntryURLDBox(*this);
  }


  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (
        std::any_cast<DataEntryURLBoxContents>(this->get_ref_to_contents()) ==
        std::any_cast<DataEntryURLBoxContents>(other.get_ref_to_contents()));
  }


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    return std::any_cast<DataEntryURLBoxContents>(this->get_ref_to_contents()).get_bytes();
  }
};


class DataEntryURLBox : public Box {
 public:
  DataEntryURLBox(const DataEntryURLBoxContents& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::DataEntryURLBoxType)),
            DataEntryURLDBox(contents)){};


  ~DataEntryURLBox() = default;
};


#endif /* end of include guard: JPLM_LIB_PART1_COMMON_DATAENTRYURLBOX_H__ */