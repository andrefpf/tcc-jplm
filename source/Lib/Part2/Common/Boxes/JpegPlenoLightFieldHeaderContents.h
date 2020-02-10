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
      const std::vector<std::unique_ptr<ColourSpecificationBox>>& colr);


  LightFieldHeaderBox& get_ref_to_light_field_header_box();


  const LightFieldHeaderBox& get_ref_to_light_field_header_box() const;


  const LightFieldHeaderBox& get_const_ref_to_lhdr() const noexcept;


  const BitsPerComponentBox& get_const_ref_to_bpcc() const;


  [[nodiscard]] std::optional<BitsPerComponentBox>
  get_copy_of_bits_per_component_box() const;


  [[nodiscard]] const ChannelDefinitionBox& get_const_ref_to_cdef() const;


  std::optional<ChannelDefinitionBox> get_copy_of_channel_definition_box()
      const;


  std::vector<ColourSpecificationBox> get_copy_of_colour_specification_boxes()
      const;


  JpegPlenoLightFieldHeaderContents(const LightFieldHeaderBox& lhdr,
      const BitsPerComponentBox& bpcc,
      const std::vector<std::unique_ptr<ColourSpecificationBox>>& colr);


  JpegPlenoLightFieldHeaderContents(
      const JpegPlenoLightFieldHeaderContents& other);


  JpegPlenoLightFieldHeaderContents(std::unique_ptr<LightFieldHeaderBox>&& lhdr,
      std::unique_ptr<BitsPerComponentBox>&& bpcc,
      std::vector<std::unique_ptr<ColourSpecificationBox>>&& colr = {},  //chck
      std::unique_ptr<ChannelDefinitionBox>&& cdef = nullptr);


  JpegPlenoLightFieldHeaderContents* clone() const override;


  uint64_t size() const noexcept override;


  bool is_equal(const DBox& other) const override;


  bool operator==(const JpegPlenoLightFieldHeaderContents& other) const
      noexcept;


  bool operator!=(const JpegPlenoLightFieldHeaderContents& other) const
      noexcept;

  ~JpegPlenoLightFieldHeaderContents() override = default;


  std::ostream& write_to(std::ostream& stream) const final;
};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_JPEGPLENOLIGHTFIELDHEADERCONTENTS_H__ */