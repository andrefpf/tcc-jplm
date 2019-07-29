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

/** \file     UUIDInfoBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-26
 */

#ifndef JPLM_LIB_PART1_COMMON_UUIDINFOBOX_H__
#define JPLM_LIB_PART1_COMMON_UUIDINFOBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"
#include "UniversalUniqueIdentifier.h"
#include "UUIDListBox.h"
#include "DataEntryURLBox.h"

class UUIDInfoBoxContents {
 protected:
  UUIDListBox u_list;
  DataEntryURLBox de;

 public:
  UUIDInfoBoxContents() = default;
  ~UUIDInfoBoxContents() = default;

  uint64_t size() const noexcept {
    return u_list.size() + de.size();
  }


  bool operator==(const UUIDInfoBoxContents& other) const {
    return (this->u_list == other.u_list) && (this->de == other.de);
  }


  bool operator!=(const UUIDInfoBoxContents& other) const {
    return !this->operator==(other);
  }
};


class UUIDInfoDBox : public DBox {
 public:
  UUIDInfoDBox(const UUIDInfoBoxContents& contents)
      : DBox(std::make_any<UUIDInfoBoxContents>(contents)) {
  }


  UUIDInfoDBox(const UUIDInfoDBox& other)
      : DBox(std::make_any<UUIDInfoBoxContents>(
            std::any_cast<UUIDInfoBoxContents>(other.contents))) {
  }


  ~UUIDInfoDBox() = default;


  uint64_t size() const noexcept override {
    return std::any_cast<UUIDInfoBoxContents>(this->contents).size();
  }


  UUIDInfoDBox* clone() const override {
    return new UUIDInfoDBox(*this);
  }


  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<UUIDInfoBoxContents>(this->get_ref_to_contents()) ==
            std::any_cast<UUIDInfoBoxContents>(other.get_ref_to_contents()));
  }
  

  virtual std::vector<uint8_t> get_bytes() const noexcept override {
    return std::any_cast<UUIDInfoBoxContents>(this->get_ref_to_contents())
        .get_bytes();
  }


};


class UUIDInfoBox : public Box {
 public:
  UUIDInfoBox(const UUIDInfoBoxContents& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::UUIDInfoBoxType)),
            UUIDInfoDBox(contents)){};


  ~UUIDInfoBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_UUIDINFOBOX_H__ */