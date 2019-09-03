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
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldHeaderContents.h"
#include "Lib/Part2/Common/Boxes/ProfileAndLevelContents.h"
#include "Lib/Common/Boxes/Generic/DefinedBoxes.h"
#include "Lib/Part1/Common/Boxes/DefinedBoxes.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"
// #include "ProfileAndLevelBox.h"

class JpegPlenoLightFieldContents : public SuperBoxDBox {
 protected:
  std::unique_ptr<ProfileAndLevelBox> profile_and_level_box;  //required
  //! \todo here (after (profile_and_level_box and jpeg_pleno_light_field_header_box) it is possible to have a pleno thumbnail box
  std::unique_ptr<JpegPlenoLightFieldHeaderBox>
      jpeg_pleno_light_field_header_box;  //required
  std::unique_ptr<ContiguousCodestreamBox>
      contiguous_codestream_box;  //optional

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


  void add_contiguous_codestream_box(std::unique_ptr<ContiguousCodestreamBox>&& contiguous_codestream_box) {
    this->contiguous_codestream_box=std::move(contiguous_codestream_box);
  }


  virtual ~JpegPlenoLightFieldContents() = default;


  virtual JpegPlenoLightFieldContents* clone() const override {
    return new JpegPlenoLightFieldContents(*this);
  }


  uint64_t size() const noexcept override {
    uint64_t required_boxes_size = profile_and_level_box->size() +
                                   jpeg_pleno_light_field_header_box->size();

    return required_boxes_size;
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const JpegPlenoLightFieldContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const JpegPlenoLightFieldContents& ) const noexcept { //other
    //! \todo implement here
    return false;
  }


  bool operator!=(const JpegPlenoLightFieldContents& other) const noexcept {
    return !this->operator==(other);
  }


  std::ostream& write_to(std::ostream& stream) const final {
    stream << *profile_and_level_box << *jpeg_pleno_light_field_header_box;
    if (contiguous_codestream_box) {
      stream << *contiguous_codestream_box;
    }
    return stream;
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_JPEGPLENOLIGHTFIELDCONTENTS_H__ */