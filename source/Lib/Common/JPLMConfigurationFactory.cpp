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

/** \file     JPLMConfigurationFactory.cpp
 *  \brief
 *  \details
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-06
 */

#include "Lib/Common/JPLMConfigurationFactory.h"


// \todo: refactor this function and split it into specialized functions
std::unique_ptr<JPLMConfiguration>
JPLMConfigurationFactory::get_encoder_configuration(
    int argc, const char** argv) {
  using Type = CompressionTypeLightField;

  auto basic_config = JPLMEncoderConfiguration(argc, const_cast<char**>(argv));

  if (basic_config.is_help_mode()) {
    //the configurations are instantiated only for printing their help (which is checked in the constructor)
    [[maybe_unused]] auto light_field_configuration =
        JPLMEncoderConfigurationLightField(argc, const_cast<char**>(argv));
    [[maybe_unused]] auto light_field_transform_mode_configuration =
        JPLMEncoderConfigurationLightField4DTransformMode(
            argc, const_cast<char**>(argv));
    exit(0);
  }

  switch (basic_config.get_jpeg_pleno_part()) {
    case JpegPlenoPart::Undefined: {
      throw JPLMConfigurationExceptions::UndefinedPartException();
    }
    case JpegPlenoPart::LightField: {
      auto light_field_configuration =
          JPLMEncoderConfigurationLightField(argc, const_cast<char**>(argv));
      auto type = light_field_configuration.get_type();
      if (type == Type::transform_mode) {
        return make_unique<JPLMEncoderConfigurationLightField4DTransformMode>(
            argc, const_cast<char**>(argv));
      } else {
        //<! \todo after implementing the prediction mode in JPLM, needs to return Prediction mode configuration
        throw JPLMConfigurationExceptions::UnsuportedPredictionMode();
      }
    }
      // case JpegPlenoPart::PointCloud: {
      // ...
      // break;
      // }
      // case JpegPlenoPart::Hologram: {
      // ...
      // break;
      // }
    case JpegPlenoPart::Framework: {
      std::cerr
          << "The reference software needs a more specialized part to operate. "
             "Although Part 1 defines the Framework that is also used in other "
             "parts, ";
      show_invalid_part_common_message();
      break;
    }
    case JpegPlenoPart::ReferenceSoftware: {
      std::cerr
          << "Part 4 defines the Reference Software of JPEG Pleno. This is the "
             "reference software, so there is no need to inform this part. "
             "Thus, ";
      show_invalid_part_common_message();
      break;
    }
    case JpegPlenoPart::ConformanceTest: {
      //<! \todo This case may trigger a help that shows how to run a conformance test
      std::cerr
          << "Part 3 defines the Conformace Tests. Although the reference "
             "software may be used for conformance testing, ";
      show_invalid_part_common_message();
      break;
    }
    default: {
      throw JPLMConfigurationExceptions::NotImplementedYetPartException();
    }
  }
}

std::unique_ptr<JPLMConfiguration>
JPLMConfigurationFactory::get_decoder_configuration(int argc, const char **argv) {
  auto basic_config =
      make_unique<JPLMDecoderConfiguration>(argc, const_cast<char**>(argv));
  if (basic_config->is_help_mode()) {
    exit(0);
  }
  return basic_config;
}

void JPLMConfigurationFactory::show_invalid_part_common_message() {
  std::cerr
      << "the encoder must be called passing a valid part that is "
         "expected to define a format to encode a given plenoptic image "
         "modality (such as Light Fields, Point Clouds and Holograms). To "
         "obtain help, run the encoder again with --help option. "
      << std::endl;
  throw JPLMConfigurationExceptions::InvalidPartException();
}

