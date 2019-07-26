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

/** \file     UUIDBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-25
 */

#ifndef JPLM_LIB_PART1_COMMON_UUIDBOX_H__
#define JPLM_LIB_PART1_COMMON_UUIDBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"
#include "UniversalUniqueIdentifier.h"

//UUID


class UUIDBoxContents {
 protected:
  UniversalUniqueIdentifier id;
  std::vector<uint8_t> data;

 public:
  UUIDBoxContents() = default;


  ~UUIDBoxContents() = default;


  uint64_t size() const noexcept {
    std::cout << "elements in data: " << data.size() << std::endl;
    return id.size() + data.size() * sizeof(uint8_t);
  }


  bool operator==(const UUIDBoxContents& other) const {
    return (this->id == other.id) && (this->data == other.data);
  }


  bool operator!=(const UUIDBoxContents& other) const {
    return !this->operator==(other);
  }


  void add_data(const std::vector<uint8_t>& data_to_add) {
    data.reserve(data_to_add.size());
    data.insert(data.end(), data_to_add.begin(), data_to_add.end());
    std::cout << "the elem: " << data.size() << std::endl;
  }
};


class UUIDDBox : public DBox {
 public:
  UUIDDBox(const UUIDBoxContents& contents)
      : DBox(std::make_any<UUIDBoxContents>(contents)) {
  }


  UUIDDBox(const UUIDDBox& other)
      : DBox(std::make_any<UUIDBoxContents>(
            std::any_cast<UUIDBoxContents>(other.contents))) {
  }


  ~UUIDDBox() = default;


  uint64_t size() const noexcept override {
    std::cout << "getting size" << std::endl;
    return std::any_cast<UUIDBoxContents>(this->contents).size();
  }


  UUIDDBox* clone() const override {
    return new UUIDDBox(*this);
  }


  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<UUIDBoxContents>(this->get_ref_to_contents()) ==
            std::any_cast<UUIDBoxContents>(other.get_ref_to_contents()));
  }

  void add_data(const std::vector<uint8_t>& data) {
    auto& ref = std::any_cast<UUIDBoxContents&>(this->contents);
    ref.add_data(data);
  }
};


class UUIDBox : public Box {
 public:
  UUIDBox(const UUIDBoxContents& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::UUIDBoxType)),
            UUIDDBox(contents)){};
  ~UUIDBox() = default;

  void add_data(const std::vector<uint8_t>& data) {
    static_cast<UUIDDBox&>(*this->d_box).add_data(data);
  }
};


#endif /* end of include guard: JPLM_LIB_PART1_COMMON_UUIDBOX_H__ */