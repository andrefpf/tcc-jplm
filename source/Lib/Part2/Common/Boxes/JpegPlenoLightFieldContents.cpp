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

/** \file     JpegPlenoLightFieldContents.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-10
 */
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldContents.h"


const JpegPlenoLightFieldHeaderBox &
JpegPlenoLightFieldContents::get_ref_to_light_field_header_box() const {
  return *jpeg_pleno_light_field_header_box;
}


JpegPlenoLightFieldHeaderBox &
JpegPlenoLightFieldContents::get_ref_to_light_field_header_box() {
  return *jpeg_pleno_light_field_header_box;
}


void JpegPlenoLightFieldContents::add_contiguous_codestream_box(
    std::unique_ptr<ContiguousCodestreamBox> &&contiguous_codestream_box) {
  this->contiguous_codestream_box = std::move(contiguous_codestream_box);
}


ContiguousCodestreamBox &
JpegPlenoLightFieldContents::get_ref_to_contiguous_codestream_box() {
  if (!contiguous_codestream_box) {
    throw JpegPlenoLightFieldBoxExceptions::
        UninitializedContigousCodestreamException();
  }
  return *contiguous_codestream_box;
}


const ContiguousCodestreamBox &
JpegPlenoLightFieldContents::get_ref_to_contiguous_codestream_box() const {
  if (!contiguous_codestream_box) {
    throw JpegPlenoLightFieldBoxExceptions::
        UninitializedContigousCodestreamException();
  }
  return *contiguous_codestream_box;
}


void JpegPlenoLightFieldContents::add_jpeg_pleno_light_field_reference_view_box(
    std::unique_ptr<JpegPlenoLightFieldReferenceViewBox>
        &&jpeg_pleno_lf_reference_view_box) {
  this->jpeg_pleno_lf_reference_view_box =
      std::move(jpeg_pleno_lf_reference_view_box);
}


void JpegPlenoLightFieldContents::
    add_jpeg_pleno_light_field_normalized_disparity_view_box(
        std::unique_ptr<JpegPlenoLightFieldNormalizedDisparityViewBox>
            &&jpeg_pleno_lf_normalized_disparity_view_box) {
  this->jpeg_pleno_lf_normalized_disparity_view_box =
      std::move(jpeg_pleno_lf_normalized_disparity_view_box);
}


void JpegPlenoLightFieldContents::
    add_jpeg_pleno_light_field_intermediate_view_box(
        std::unique_ptr<JpegPlenoLightFieldIntermediateViewBox>
            &&jpeg_pleno_lf_intermediate_view_box) {
  this->jpeg_pleno_lf_intermediate_view_box =
      std::move(jpeg_pleno_lf_intermediate_view_box);
}


JpegPlenoLightFieldContents *JpegPlenoLightFieldContents::clone() const {
  return new JpegPlenoLightFieldContents(*this);
}


uint64_t JpegPlenoLightFieldContents::size() const noexcept {
  uint64_t size =
      profile_and_level_box->size() + jpeg_pleno_light_field_header_box->size();
  if (jpeg_pleno_thumbnail_box) {
    size += jpeg_pleno_thumbnail_box->size();
  }
  if (contiguous_codestream_box) {
    size += contiguous_codestream_box->size();
  }
  if (jpeg_pleno_lf_reference_view_box) {
    size += jpeg_pleno_lf_reference_view_box->size();
  }
  if (jpeg_pleno_lf_normalized_disparity_view_box) {
    size += jpeg_pleno_lf_normalized_disparity_view_box->size();
  }
  if (jpeg_pleno_lf_intermediate_view_box) {
    size += jpeg_pleno_lf_intermediate_view_box->size();
  }
  return size;
}


bool JpegPlenoLightFieldContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other =
      dynamic_cast<const JpegPlenoLightFieldContents &>(other);
  return *this == cast_other;
}


bool JpegPlenoLightFieldContents::operator==(
    const JpegPlenoLightFieldContents &) const noexcept {  //other
  //! \todo implement here
  return false;
}


bool JpegPlenoLightFieldContents::operator!=(
    const JpegPlenoLightFieldContents &other) const noexcept {
  return !this->operator==(other);
}


std::ostream &JpegPlenoLightFieldContents::write_to(
    std::ostream &stream) const {
  stream << *profile_and_level_box;  //required
  if (jpeg_pleno_thumbnail_box) {
    stream << *jpeg_pleno_thumbnail_box;
  }
  stream << *jpeg_pleno_light_field_header_box;  //required
  if (contiguous_codestream_box) {
    stream << *contiguous_codestream_box;
  }
  if (jpeg_pleno_lf_reference_view_box) {
    stream << *jpeg_pleno_lf_reference_view_box;
  }
  if (jpeg_pleno_lf_normalized_disparity_view_box) {
    stream << *jpeg_pleno_lf_normalized_disparity_view_box;
  }
  if (jpeg_pleno_lf_intermediate_view_box) {
    stream << *jpeg_pleno_lf_intermediate_view_box;
  }
  return stream;
}


JpegPlenoLightFieldContents::JpegPlenoLightFieldContents(
    std::unique_ptr<ProfileAndLevelBox> &&profile_and_level_box,
    std::unique_ptr<JpegPlenoLightFieldHeaderBox>
        &&jpeg_pleno_light_field_header_box)
    : profile_and_level_box(std::move(profile_and_level_box)),
      jpeg_pleno_light_field_header_box(
          std::move(jpeg_pleno_light_field_header_box)) {
}


JpegPlenoLightFieldContents::JpegPlenoLightFieldContents(
    const ProfileAndLevelBox &profile_and_level_box,
    const JpegPlenoLightFieldHeaderBox &jpeg_pleno_light_field_header_box)
    : profile_and_level_box(
          std::make_unique<ProfileAndLevelBox>(profile_and_level_box)),
      jpeg_pleno_light_field_header_box(
          std::make_unique<JpegPlenoLightFieldHeaderBox>(
              jpeg_pleno_light_field_header_box)) {
}


JpegPlenoLightFieldContents::JpegPlenoLightFieldContents(
    const JpegPlenoLightFieldContents &other)
    : profile_and_level_box(
          std::make_unique<ProfileAndLevelBox>(*(other.profile_and_level_box))),
      jpeg_pleno_light_field_header_box(
          std::make_unique<JpegPlenoLightFieldHeaderBox>(
              *(other.jpeg_pleno_light_field_header_box))),
      contiguous_codestream_box(other.contiguous_codestream_box
                                    ? std::make_unique<ContiguousCodestreamBox>(
                                          *(other.contiguous_codestream_box))
                                    : nullptr) {
}
