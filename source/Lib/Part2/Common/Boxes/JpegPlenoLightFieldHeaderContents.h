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

/** \file     JpegPlenoLightFieldHeaderContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_BOXES_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__

#include <memory>
#include <tuple>  //std::tie
#include <vector>
#include "Lib/Common/Boxes/Generic/BitsPerComponentBox.h"
#include "Lib/Common/Boxes/Generic/ChannelDefinitionBox.h"
#include "Lib/Common/Boxes/Generic/ColourSpecificationBox.h"
#include "Lib/Common/Boxes/SuperBoxDBox.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderBox.h"

class JpegPlenoLightFieldHeaderContents : public SuperBoxDBox {
 protected:
  std::unique_ptr<LightFieldHeaderBox> lhdr;  //light_field_header_box
  std::unique_ptr<BitsPerComponentBox>
      bpcc;  //bits per component box; ! optional !
  //! \todo check the condition: "there shall be at least one Colour Specification box within the JPEG Pleno Header Box"
  std::vector<std::unique_ptr<ColourSpecificationBox>>
      colr;  //colour specification boxes; ! optionals?? !
  std::unique_ptr<ChannelDefinitionBox>
      cdef;  //channel definition box ! optional !
 public:
  //! \todo use always the same constructor
  JpegPlenoLightFieldHeaderContents(const LightFieldHeaderBox& lhdr,
      const std::vector<std::unique_ptr<ColourSpecificationBox>>& colr)
      : lhdr(std::make_unique<LightFieldHeaderBox>(lhdr)) {
    auto number_of_colour_specification_boxes = colr.size();
    if (number_of_colour_specification_boxes == 0) {
      //throw
    }
    this->colr.reserve(number_of_colour_specification_boxes);
    for (const auto& colour_specification_box : colr) {
      this->colr.emplace_back(
          std::make_unique<ColourSpecificationBox>(*colour_specification_box));
    }
  }


  LightFieldHeaderBox& get_ref_to_light_field_header_box() {
    return *lhdr;
  }


  const LightFieldHeaderBox& get_ref_to_light_field_header_box() const {
    return *lhdr;
  }


  const auto& get_const_ref_to_lhdr() const noexcept {
    return *lhdr;
  }


  const auto& get_const_ref_to_bpcc() const {
    if (!bpcc) {
      //! \todo throw exception
    }
    return *bpcc;
  }


  std::optional<BitsPerComponentBox> get_copy_of_bits_per_component_box()
      const {
    if (bpcc)
      return *bpcc;
    return std::nullopt;
  }


  const auto& get_const_ref_to_cdef() const {
    if (!cdef) {
      //! \todo throw exception
    }
    return *cdef;
  }


  std::optional<ChannelDefinitionBox> get_copy_of_channel_definition_box()
      const {
    if (cdef)
      return *cdef;
    return std::nullopt;
  }


  std::vector<ColourSpecificationBox> get_copy_of_colour_specification_boxes()
      const {
    auto ret_vector = std::vector<ColourSpecificationBox>();
    ret_vector.reserve(colr.size());
    for (const auto& colour_specification_box : colr) {
      ret_vector.emplace_back(*colour_specification_box);
    }
    return ret_vector;
  }


  JpegPlenoLightFieldHeaderContents(const LightFieldHeaderBox& lhdr,
      const BitsPerComponentBox& bpcc,
      const std::vector<std::unique_ptr<ColourSpecificationBox>>& colr)
      : lhdr(std::make_unique<LightFieldHeaderBox>(lhdr)),
        bpcc(std::make_unique<BitsPerComponentBox>(bpcc)) {
    auto number_of_colour_specification_boxes = colr.size();
    if (number_of_colour_specification_boxes == 0) {
      //throw
    }
    this->colr.reserve(number_of_colour_specification_boxes);
    for (const auto& colour_specification_box : colr) {
      this->colr.emplace_back(
          std::make_unique<ColourSpecificationBox>(*colour_specification_box));
    }
  }


  JpegPlenoLightFieldHeaderContents(
      const JpegPlenoLightFieldHeaderContents& other)
      : lhdr(std::make_unique<LightFieldHeaderBox>(*(other.lhdr))),
        bpcc(other.bpcc ? std::make_unique<BitsPerComponentBox>(*(other.bpcc))
                        : nullptr),
        cdef(other.cdef ? std::make_unique<ChannelDefinitionBox>(*(other.cdef))
                        : nullptr) {
    auto number_of_colour_specification_boxes = other.colr.size();
    this->colr.reserve(number_of_colour_specification_boxes);
    for (const auto& colour_specification_box : other.colr) {
      this->colr.emplace_back(
          std::make_unique<ColourSpecificationBox>(*colour_specification_box));
    }
  }


  JpegPlenoLightFieldHeaderContents(std::unique_ptr<LightFieldHeaderBox>&& lhdr,
      std::unique_ptr<BitsPerComponentBox>&& bpcc,
      std::vector<std::unique_ptr<ColourSpecificationBox>>&& colr = {}, //chck
      std::unique_ptr<ChannelDefinitionBox>&& cdef = nullptr)
      : lhdr(std::move(lhdr)), bpcc(bpcc ? std::move(bpcc) : nullptr),
        colr(std::move(colr)), cdef(cdef ? std::move(cdef) : nullptr) {
  }


  virtual JpegPlenoLightFieldHeaderContents* clone() const override {
    return new JpegPlenoLightFieldHeaderContents(*this);
  }


  uint64_t size() const noexcept override {
    auto size = lhdr->size();
    if (bpcc) {
      size += bpcc->size();
    }
    for (const auto& colour_specification_box : colr) {
      size += colour_specification_box->size();
    }
    if (cdef) {
      size += cdef->size();
    }
    return size;
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const JpegPlenoLightFieldHeaderContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const JpegPlenoLightFieldHeaderContents& other) const
      noexcept {
    if (*(this->lhdr) == *(other.lhdr) && (this->colr == other.colr)) {
      if ((this->bpcc && other.bpcc) && (*(this->bpcc) == *(other.bpcc))) {
        if ((this->cdef && other.cdef) && (*(this->cdef) == *(other.cdef))) {
          return true;
        }
      }
    }
    return false;
  }


  bool operator!=(const JpegPlenoLightFieldHeaderContents& other) const
      noexcept {
    return !this->operator==(other);
  }

  virtual ~JpegPlenoLightFieldHeaderContents() = default;


  std::ostream& write_to(std::ostream& stream) const final {
    stream << *lhdr;
    if (bpcc) {
      stream << *bpcc;
    }
    for (const auto& colour_specification_box : colr) {
      stream << *colour_specification_box;
    }
    if (cdef) {
      stream << *cdef;
    }
    return stream;
  }


};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__ */