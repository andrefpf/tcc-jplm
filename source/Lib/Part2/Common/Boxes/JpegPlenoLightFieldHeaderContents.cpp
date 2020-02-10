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

/** \file     JpegPlenoLightFieldHeaderContents.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-10
 */
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldContents.h"


JpegPlenoLightFieldHeaderContents::JpegPlenoLightFieldHeaderContents(
    const LightFieldHeaderBox &lhdr,
    const std::vector<std::unique_ptr<ColourSpecificationBox>> &colr)
    : lhdr(std::make_unique<LightFieldHeaderBox>(lhdr)) {
  auto number_of_colour_specification_boxes = colr.size();
  if (number_of_colour_specification_boxes == 0) {
    //throw
  }
  this->colr.reserve(number_of_colour_specification_boxes);
  for (const auto &colour_specification_box : colr) {
    this->colr.emplace_back(
        std::make_unique<ColourSpecificationBox>(*colour_specification_box));
  }
}


LightFieldHeaderBox &
JpegPlenoLightFieldHeaderContents::get_ref_to_light_field_header_box() {
  return *lhdr;
}


const LightFieldHeaderBox &
JpegPlenoLightFieldHeaderContents::get_ref_to_light_field_header_box() const {
  return *lhdr;
}


const LightFieldHeaderBox &
JpegPlenoLightFieldHeaderContents::get_const_ref_to_lhdr() const noexcept {
  return *lhdr;
}


const BitsPerComponentBox &
JpegPlenoLightFieldHeaderContents::get_const_ref_to_bpcc() const {
  if (!bpcc) {
    //! \todo throw exception
  }
  return *bpcc;
}


std::optional<BitsPerComponentBox>
JpegPlenoLightFieldHeaderContents::get_copy_of_bits_per_component_box() const {
  if (bpcc)
    return *bpcc;
  return std::nullopt;
}


const ChannelDefinitionBox &
JpegPlenoLightFieldHeaderContents::get_const_ref_to_cdef() const {
  if (!cdef) {
    //! \todo throw exception
  }
  return *cdef;
}


std::optional<ChannelDefinitionBox>
JpegPlenoLightFieldHeaderContents::get_copy_of_channel_definition_box() const {
  if (cdef)
    return *cdef;
  return std::nullopt;
}


std::vector<ColourSpecificationBox>
JpegPlenoLightFieldHeaderContents::get_copy_of_colour_specification_boxes()
    const {
  auto ret_vector = std::vector<ColourSpecificationBox>();
  ret_vector.reserve(colr.size());
  for (const auto &colour_specification_box : colr) {
    ret_vector.emplace_back(*colour_specification_box);
  }
  return ret_vector;
}


JpegPlenoLightFieldHeaderContents::JpegPlenoLightFieldHeaderContents(
    const LightFieldHeaderBox &lhdr, const BitsPerComponentBox &bpcc,
    const std::vector<std::unique_ptr<ColourSpecificationBox>> &colr)
    : lhdr(std::make_unique<LightFieldHeaderBox>(lhdr)),
      bpcc(std::make_unique<BitsPerComponentBox>(bpcc)) {
  auto number_of_colour_specification_boxes = colr.size();
  if (number_of_colour_specification_boxes == 0) {
    //throw
  }
  this->colr.reserve(number_of_colour_specification_boxes);
  for (const auto &colour_specification_box : colr) {
    this->colr.emplace_back(
        std::make_unique<ColourSpecificationBox>(*colour_specification_box));
  }
}


JpegPlenoLightFieldHeaderContents::JpegPlenoLightFieldHeaderContents(
    const JpegPlenoLightFieldHeaderContents &other)
    : lhdr(std::make_unique<LightFieldHeaderBox>(*(other.lhdr))),
      bpcc(other.bpcc ? std::make_unique<BitsPerComponentBox>(*(other.bpcc))
                      : nullptr),
      cdef(other.cdef ? std::make_unique<ChannelDefinitionBox>(*(other.cdef))
                      : nullptr) {
  auto number_of_colour_specification_boxes = other.colr.size();
  this->colr.reserve(number_of_colour_specification_boxes);
  for (const auto &colour_specification_box : other.colr) {
    this->colr.emplace_back(
        std::make_unique<ColourSpecificationBox>(*colour_specification_box));
  }
}


JpegPlenoLightFieldHeaderContents::JpegPlenoLightFieldHeaderContents(
    std::unique_ptr<LightFieldHeaderBox> &&lhdr,
    std::unique_ptr<BitsPerComponentBox> &&bpcc,
    std::vector<std::unique_ptr<ColourSpecificationBox>> &&colr,
    std::unique_ptr<ChannelDefinitionBox> &&cdef)
    : lhdr(std::move(lhdr)), bpcc(bpcc ? std::move(bpcc) : nullptr),
      colr(std::move(colr)), cdef(cdef ? std::move(cdef) : nullptr) {
}


JpegPlenoLightFieldHeaderContents *JpegPlenoLightFieldHeaderContents::clone()
    const {
  return new JpegPlenoLightFieldHeaderContents(*this);
}


uint64_t JpegPlenoLightFieldHeaderContents::size() const noexcept {
  auto size = lhdr->size();
  if (bpcc) {
    size += bpcc->size();
  }
  for (const auto &colour_specification_box : colr) {
    size += colour_specification_box->size();
  }
  if (cdef) {
    size += cdef->size();
  }
  return size;
}


bool JpegPlenoLightFieldHeaderContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other =
      dynamic_cast<const JpegPlenoLightFieldHeaderContents &>(other);
  return *this == cast_other;
}


bool JpegPlenoLightFieldHeaderContents::operator==(
    const JpegPlenoLightFieldHeaderContents &other) const noexcept {
  if (*(this->lhdr) == *(other.lhdr) && (this->colr == other.colr)) {
    if ((this->bpcc && other.bpcc) && (*(this->bpcc) == *(other.bpcc))) {
      if ((this->cdef && other.cdef) && (*(this->cdef) == *(other.cdef))) {
        return true;
      }
    }
  }
  return false;
}


bool JpegPlenoLightFieldHeaderContents::operator!=(
    const JpegPlenoLightFieldHeaderContents &other) const noexcept {
  return !this->operator==(other);
}


std::ostream &JpegPlenoLightFieldHeaderContents::write_to(
    std::ostream &stream) const {
  stream << *lhdr;
  if (bpcc) {
    stream << *bpcc;
  }
  for (const auto &colour_specification_box : colr) {
    stream << *colour_specification_box;
  }
  if (cdef) {
    stream << *cdef;
  }
  return stream;
}
