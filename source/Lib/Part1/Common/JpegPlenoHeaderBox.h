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

/** \file     JpegPlenoHeaderBox.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-24
 */

#ifndef JPLM_LIB_PART1_COMMON_JPEGPLENOHEADERBOX_H__
#define JPLM_LIB_PART1_COMMON_JPEGPLENOHEADERBOX_H__

#include <algorithm>
#include "Box.h"
#include "DefinedBoxes.h"

class JpegPlenoHeaderContents {
 protected:
  // uint32_t n_lf;
  // uint32_t n_pc;
  // uint32_t n_ho;
  std::vector<uint64_t> plfb;  //ptrs to light field box
  std::vector<uint64_t> ppcb;  //ptrs to point cloud box
  std::vector<uint64_t> phob;  //ptrs to hologram box

 public:
  uint32_t get_n_lf() const noexcept {
    return plfb.size();
  }


  uint32_t get_n_pc() const noexcept {
    return ppcb.size();
  }


  uint32_t get_n_ho() const noexcept {
    return phob.size();
  }

  bool operator==(const JpegPlenoHeaderContents& other) const {
    return std::tie(this->plfb, this->ppcb, this->phob) ==
           std::tie(other.plfb, other.ppcb, other.phob);
  }

  bool operator!=(const JpegPlenoHeaderContents& other) const {
    return !this->operator==(other);
  }


  void add_pointer_to_light_field_box(uint64_t ptr) {
    //the ptr is w.r.t the byte starting after the
    //JpegPlenoHeaderDBox
    plfb.push_back(ptr);
  }


  void add_pointer_to_point_cloud_box(uint64_t ptr) {
    //the ptr is w.r.t the byte starting after the
    //JpegPlenoHeaderDBox
    ppcb.push_back(ptr);
  }


  void add_pointer_to_hologram_box(uint64_t ptr) {
    //the ptr is w.r.t the byte starting after the
    //JpegPlenoHeaderDBox
    phob.push_back(ptr);
  }


  uint64_t size() const noexcept {
    constexpr auto size_of_n_lf_n_pc_n_ho = 3 * sizeof(uint32_t);
    return size_of_n_lf_n_pc_n_ho +
           ((get_n_lf() + get_n_pc() + get_n_ho()) * sizeof(uint64_t));
  }
};


class JpegPlenoHeaderDBox : public DBox {
 public:
  JpegPlenoHeaderDBox(const JpegPlenoHeaderContents& contents)
      : DBox(std::make_any<JpegPlenoHeaderContents>(contents)) {
  }


  JpegPlenoHeaderDBox(const JpegPlenoHeaderDBox& other)
      : DBox(std::make_any<JpegPlenoHeaderContents>(
            std::any_cast<JpegPlenoHeaderContents>(other.contents))) {
  }


  ~JpegPlenoHeaderDBox() = default;


  uint64_t size() const noexcept override {
    return std::any_cast<JpegPlenoHeaderContents>(this->contents).size();
  }


  JpegPlenoHeaderDBox* clone() const override {
    return new JpegPlenoHeaderDBox(*this);
  }


  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (
        std::any_cast<JpegPlenoHeaderContents>(this->get_ref_to_contents()) ==
        std::any_cast<JpegPlenoHeaderContents>(other.get_ref_to_contents()));
  }


  virtual std::vector<uint8_t> get_bytes() const noexcept override {
    return std::any_cast<JpegPlenoHeaderContents>(this->get_ref_to_contents())
        .get_bytes();
  }
};


class JpegPlenoHeaderBox : public Box {
 public:
  JpegPlenoHeaderBox(const JpegPlenoHeaderDBox& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::JPEGPlenoHeaderBoxType)),
            contents){};


  ~JpegPlenoHeaderBox() = default;
};


#endif /* end of include guard: JPLM_LIB_PART1_COMMON_JPEGPLENOHEADERBOX_H__ */