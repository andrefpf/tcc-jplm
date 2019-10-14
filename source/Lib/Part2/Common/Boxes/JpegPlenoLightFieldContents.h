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

/** \file     JpegPlenoLightFieldContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */

#ifndef JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDCONTENTS_H__


#include <memory>
#include <tuple>  //std::tie
#include <vector>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamBox.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Part1/Common/Boxes/JpegPlenoThumbnailBox.h"
#include "Lib/Part2/Common/Boxes/CommonExceptions.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldHeaderBox.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldIntermediateViewBox.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldNormalizedDisparityViewBox.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldReferenceViewBox.h"
#include "Lib/Part2/Common/Boxes/ProfileAndLevelBox.h"


class JpegPlenoLightFieldContents : public SuperBoxDBox {
 protected:
  std::unique_ptr<ProfileAndLevelBox> profile_and_level_box;  //required
  std::unique_ptr<JpegPlenoThumbnailBox> jpeg_pleno_thumbnail_box;  //optional;
  std::unique_ptr<JpegPlenoLightFieldHeaderBox>
      jpeg_pleno_light_field_header_box;  //required
  std::unique_ptr<ContiguousCodestreamBox>
      contiguous_codestream_box;  //optional
  std::unique_ptr<JpegPlenoLightFieldReferenceViewBox>
      jpeg_pleno_lf_reference_view_box = nullptr;
  std::unique_ptr<JpegPlenoLightFieldNormalizedDisparityViewBox>
      jpeg_pleno_lf_normalized_disparity_view_box = nullptr;
  std::unique_ptr<JpegPlenoLightFieldIntermediateViewBox>
      jpeg_pleno_lf_intermediate_view_box = nullptr;

 public:
  JpegPlenoLightFieldContents(const ProfileAndLevelBox& profile_and_level_box,
      const JpegPlenoLightFieldHeaderBox& jpeg_pleno_light_field_header_box)
      : profile_and_level_box(
            std::make_unique<ProfileAndLevelBox>(profile_and_level_box)),
        jpeg_pleno_light_field_header_box(
            std::make_unique<JpegPlenoLightFieldHeaderBox>(
                jpeg_pleno_light_field_header_box)) {
  }


  /**
   * \brief      Constructs the object from lvalue refs
   *
   * \param      profile_and_level_box              The profile and level box
   * \param      jpeg_pleno_light_field_header_box  The jpeg pleno light field header box
   */
  JpegPlenoLightFieldContents(
      std::unique_ptr<ProfileAndLevelBox>&& profile_and_level_box,
      std::unique_ptr<JpegPlenoLightFieldHeaderBox>&&
          jpeg_pleno_light_field_header_box)
      : profile_and_level_box(std::move(profile_and_level_box)),
        jpeg_pleno_light_field_header_box(
            std::move(jpeg_pleno_light_field_header_box)) {
  }


  /**
   * \brief      Copy constructor of JpegPlenoLightFieldContents
   *
   * \param[in]  other  The other JpegPlenoLightFieldContents
   * 
   * profile_and_level_box and jpeg_pleno_light_field_header_box are required fields and thus should not be nullptr
   */
  JpegPlenoLightFieldContents(const JpegPlenoLightFieldContents& other)
      : profile_and_level_box(std::make_unique<ProfileAndLevelBox>(
            *(other.profile_and_level_box))),
        jpeg_pleno_light_field_header_box(
            std::make_unique<JpegPlenoLightFieldHeaderBox>(
                *(other.jpeg_pleno_light_field_header_box))),
        contiguous_codestream_box(
            other.contiguous_codestream_box
                ? std::make_unique<ContiguousCodestreamBox>(
                      *(other.contiguous_codestream_box))
                : nullptr) {
  }


  const JpegPlenoLightFieldHeaderBox& get_ref_to_light_field_header_box() const {
    return *jpeg_pleno_light_field_header_box;
  }


  JpegPlenoLightFieldHeaderBox& get_ref_to_light_field_header_box() {
    return *jpeg_pleno_light_field_header_box;
  }


  /**
   * \brief      Adds a contiguous codestream box to the JPEG Pleno Light Field Contents.
   *
   * \param      contiguous_codestream_box  The contiguous codestream box
   * \warning    If a contiguous codestream box is already present it will be overwritten
   */
  void add_contiguous_codestream_box(
      std::unique_ptr<ContiguousCodestreamBox>&& contiguous_codestream_box) {
    std::cout << "added contiguous codestream box" << std::endl;
    this->contiguous_codestream_box = std::move(contiguous_codestream_box);
  }

  
  ContiguousCodestreamBox& get_ref_to_contiguous_codestream_box() {
    if(!contiguous_codestream_box) {
      throw JpegPlenoLightFieldBoxExceptions::UninitializedContigousCodestreamException();
    }
    return *contiguous_codestream_box; 
  }

  
  const ContiguousCodestreamBox& get_ref_to_contiguous_codestream_box() const {
    if(!contiguous_codestream_box) {
      throw JpegPlenoLightFieldBoxExceptions::UninitializedContigousCodestreamException();
    }
    return *contiguous_codestream_box; 
  }



  /**
   * \brief      Adds a jpeg pleno light field reference view box.
   *
   * \param      jpeg_pleno_lf_reference_view_box  The jpeg pleno light field reference view box
   * \warning    If a jpeg pleno light field reference view box is already present it will be overwritten
   */
  void add_jpeg_pleno_light_field_reference_view_box(
      std::unique_ptr<JpegPlenoLightFieldReferenceViewBox>&&
          jpeg_pleno_lf_reference_view_box) {
    this->jpeg_pleno_lf_reference_view_box =
        std::move(jpeg_pleno_lf_reference_view_box);
  }


  /**
   * \brief      Adds a jpeg pleno light field normalized disparity view box.
   *
   * \param      jpeg_pleno_lf_normalized_disparity_view_box  The jpeg pleno light field normalized disparity view box
   * \warning    If a jpeg pleno light field normalized disparity view box is already present it will be overwritten
   */
  void add_jpeg_pleno_light_field_normalized_disparity_view_box(
      std::unique_ptr<JpegPlenoLightFieldNormalizedDisparityViewBox>&&
          jpeg_pleno_lf_normalized_disparity_view_box) {
    this->jpeg_pleno_lf_normalized_disparity_view_box =
        std::move(jpeg_pleno_lf_normalized_disparity_view_box);
  }


  /**
   * \brief      Adds a jpeg pleno light field intermediate view box.
   *
   * \param      jpeg_pleno_lf_intermediate_view_box  The jpeg pleno line feed intermediate view box
   * \warning    If a jpeg pleno light field intermediate view box is already present it will be overwritten
   */
  void add_jpeg_pleno_light_field_intermediate_view_box(
      std::unique_ptr<JpegPlenoLightFieldIntermediateViewBox>&&
          jpeg_pleno_lf_intermediate_view_box) {
    this->jpeg_pleno_lf_intermediate_view_box =
        std::move(jpeg_pleno_lf_intermediate_view_box);
  }


  virtual ~JpegPlenoLightFieldContents() = default;


  virtual JpegPlenoLightFieldContents* clone() const override {
    return new JpegPlenoLightFieldContents(*this);
  }


  uint64_t size() const noexcept override {
    uint64_t size = profile_and_level_box->size() +
                    jpeg_pleno_light_field_header_box->size();
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


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const JpegPlenoLightFieldContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const JpegPlenoLightFieldContents&) const noexcept {  //other
    //! \todo implement here
    return false;
  }


  bool operator!=(const JpegPlenoLightFieldContents& other) const noexcept {
    return !this->operator==(other);
  }


  //! [Overridden write_to in JpegPlenoLightFieldContents]
  std::ostream& write_to(std::ostream& stream) const final {
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
  //! [Overridden write_to in JpegPlenoLightFieldContents]
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDCONTENTS_H__ */