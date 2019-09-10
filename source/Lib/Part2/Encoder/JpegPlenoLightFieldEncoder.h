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

/** \file     JpegPlenoLightFieldEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-09
 */

#ifndef JPLM_LIB_PART2_ENCODER_JPEGPLENOLIGHTFIELDENCODER_H__
#define JPLM_LIB_PART2_ENCODER_JPEGPLENOLIGHTFIELDENCODER_H__

#include "Lib/Part2/Common/JpegPlenoLightFieldCodec.h"
#include "Lib/Part2/Common/LightfieldFromPPMFile.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderBox.h"

#include "Lib/Part2/Common/Boxes/CompressionTypeLightField.h"
//stub
class LightFieldEncoderConfiguration {
 protected:
  std::string path;

 public:
  LightFieldEncoderConfiguration(const std::string& path) : path(path) {
  }


  LightfieldDimension<uint32_t> get_lightfield_dimensions() const;
  auto  get_lightfield_io_configurations() const {
    LightfieldDimension<std::size_t> size(3, 3, 32, 32);
    LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);
    return LightfieldIOConfiguration(path, initial, size);
  }


  auto get_compression_type() const {
    return CompressionTypeLightField::transform_mode;
  }
};

template<typename T = uint16_t>
class JpegPlenoLightFieldEncoder : public JpegPlenoLightFieldCodec<T> {
 protected:
  std::unique_ptr<const LightFieldEncoderConfiguration> configuration;

 public:
  JpegPlenoLightFieldEncoder(
      std::unique_ptr<const LightFieldEncoderConfiguration>&& configuration)
      : JpegPlenoLightFieldCodec<T>(
            std::move(std::make_unique<LightfieldFromPPMFile<T>>(
                configuration->get_lightfield_io_configurations()))),
        configuration(std::move(configuration)) {
          

    auto lf_header_contents = LightFieldHeaderContents(
        this->light_field->template get_dimensions<uint32_t>(),
        this->light_field->get_number_of_channels_in_view(),
        this->light_field->get_views_bpp(), configuration->get_compression_type());

    // auto jpeg_pleno_lf_header_contents = JpegPlenoLightFieldHeaderContents(

    //   );


  }


  virtual ~JpegPlenoLightFieldEncoder() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_JPEGPLENOLIGHTFIELDENCODER_H__ */