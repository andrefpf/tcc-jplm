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

/** \file     JPLMLightFieldEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-09
 */

#ifndef JPLM_LIB_PART2_ENCODER_JPLMLIGHTFIELDENCODER_H__
#define JPLM_LIB_PART2_ENCODER_JPLMLIGHTFIELDENCODER_H__

#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldBox.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldHeaderBox.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderBox.h"
#include "Lib/Part2/Common/Boxes/ProfileAndLevelBox.h"
#include "Lib/Part2/Common/JPLMLightFieldCodec.h"
#include "Lib/Part2/Common/LightfieldFromPPMFile.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "source/Lib/Common/JPLMEncoderConfigurationLightField.h"


template<typename T = uint16_t>
class JPLMLightFieldEncoder : public JPLMLightFieldCodec<T> {
 protected:
  std::unique_ptr<const JPLMEncoderConfigurationLightField> configuration;

  void add_pleno_lf_box() {
    auto profile_and_level_box = std::make_unique<ProfileAndLevelBox>();


    auto lf_header_contents = LightFieldHeaderContents(
        this->light_field->template get_dimensions<uint32_t>(),
        this->light_field->get_number_of_channels_in_view(),
        this->light_field->get_views_bpp(),
        configuration->get_compression_type());
    auto lightfield_header_box =
        std::make_unique<LightFieldHeaderBox>(std::move(lf_header_contents));
    auto colour_specification_boxes =
        std::vector<std::unique_ptr<ColourSpecificationBox>>();
    colour_specification_boxes.emplace_back(std::make_unique<ColourSpecificationBox>());

    auto jpeg_pleno_light_field_header_box =
        std::make_unique<JpegPlenoLightFieldHeaderBox>(
            JpegPlenoLightFieldHeaderContents(std::move(lightfield_header_box),
                nullptr, std::move(colour_specification_boxes)));

    auto jpeg_pleno_light_field_box = std::make_unique<JpegPlenoLightFieldBox>(
        JpegPlenoLightFieldContents(std::move(profile_and_level_box),
            std::move(jpeg_pleno_light_field_header_box)));

    this->jpl_file->add_codestream_box(std::move(jpeg_pleno_light_field_box));
  }


 public:
  JPLMLightFieldEncoder(
      std::unique_ptr<const JPLMEncoderConfigurationLightField>&& configuration)
      : JPLMLightFieldCodec<T>(
            std::move(std::make_unique<LightfieldFromPPMFile<T>>(
                configuration->get_lightfield_io_configurations()))),
        configuration(std::move(configuration)) {
    add_pleno_lf_box();
  }


  virtual ~JPLMLightFieldEncoder() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_JPLMLIGHTFIELDENCODER_H__ */