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

/** \file     ColourSpecificationBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-24
 */

#ifndef JPLM_LIB_PART1_COMMON_COLOURSPECIFICATIONBOX_H__
#define JPLM_LIB_PART1_COMMON_COLOURSPECIFICATIONBOX_H__

#include <stdexcept>
#include "Box.h"
#include "DefinedBoxes.h"

class ICCProfile {
  //not implemented
 public:
  uint64_t size() {
    return 0;  //not implemented
  }
};

class ColourSpecificationContents : public DBoxContents {
 protected:
  uint8_t meth;  //specification method, shall be 1 or 2
  int8_t prec;  //precedence. Reserved for ISO use and value shall be 0;
  uint8_t
      approx;  //Coulourspace approximation; shall be set to 0; other values for ISO use;
  std::optional<uint16_t> EnumCS;  //enumerated colourspace
  std::optional<ICCProfile> profile;

 public:
  ColourSpecificationContents(uint16_t EnumCS)
      : meth(1), prec(0), approx(0), EnumCS(EnumCS) {
  }


  ColourSpecificationContents(ICCProfile profile)
      : meth(2), prec(0), approx(0), profile(profile) {
  }


  ~ColourSpecificationContents();


  uint64_t size() {
    constexpr auto initial_size = 3 * sizeof(uint8_t);
    if (EnumCS) {
      return initial_size + sizeof(uint16_t);
    }
    if (profile) {
      return initial_size + profile->size();
    }
    throw std::runtime_error(
        "Either EnumCS or ICCProfile must be present in "
        "ColourSpecificationBox");
  }


  bool operator==(const ColourSpecificationContents& other) const {
    //! \todo Add ICCProfile if needed
    return std::tie(meth, prec, approx, EnumCS) ==
           std::tie(other.meth, other.prec, other.approx, other.EnumCS);
  }


  bool operator!=(const ColourSpecificationContents& other) const {
    return !this->operator==(other);
  }
};


class ColourSpecificationDBox : public DBox {
 public:
  ColourSpecificationDBox(const ColourSpecificationContents& contents)
      : DBox(std::make_any<ColourSpecificationContents>(contents)) {
  }


  ColourSpecificationDBox(const ColourSpecificationDBox& other)
      : DBox(std::make_any<ColourSpecificationContents>(
            std::any_cast<ColourSpecificationContents>(other.contents))) {
  }


  ~ColourSpecificationDBox() = default;


  uint64_t size() const noexcept override {
    return std::any_cast<ColourSpecificationContents>(this->contents).size();
  }


  ColourSpecificationDBox* clone() const override {
    return new ColourSpecificationDBox(*this);
  }


  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<ColourSpecificationContents>(
                this->get_ref_to_contents()) ==
            std::any_cast<ColourSpecificationContents>(
                other.get_ref_to_contents()));
  }


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    return std::any_cast<ColourSpecificationContents>(
        this->get_ref_to_contents())
        .get_bytes();
  }
};


class ColourSpecificationBox : public Box {
 public:
  ColourSpecificationBox(const std::vector<uint8_t>& bits_per_component_vector)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::ColourSpecificationBoxType)),
            CharArrayDBox(bits_per_component_vector)){};


  ~ColourSpecificationBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_COLOURSPECIFICATIONBOX_H__ */