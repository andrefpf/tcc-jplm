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

/** \file     JPLMCodecFactory.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */

#include "Lib/Common/JPLMCodecFactory.h"

std::unique_ptr<JPLMCodec> JPLMCodecFactory::get_light_field_encoder(
    std::unique_ptr<JPLMEncoderConfigurationLightField> &&configuration) {
  if (configuration->get_compression_type() ==
      CompressionTypeLightField::prediction_mode) {
    return std::make_unique<JPLM4DPredictionModeLightFieldEncoder<uint16_t>>(
        std::unique_ptr<JPLMEncoderConfigurationLightField4DPredictionMode>(
            static_cast<JPLMEncoderConfigurationLightField4DPredictionMode *>(
                configuration.release())));
  }
  assert(configuration->get_compression_type() ==
         CompressionTypeLightField::transform_mode);

  return std::make_unique<JPLM4DTransformModeLightFieldEncoder<uint16_t>>(
      std::unique_ptr<JPLMEncoderConfigurationLightField4DTransformMode>(
          static_cast<JPLMEncoderConfigurationLightField4DTransformMode *>(
              configuration.release())));
}

std::unique_ptr<JPLMCodec> JPLMCodecFactory::get_encoder(
    std::unique_ptr<JPLMEncoderConfiguration> &&configuration) {
  switch (configuration->get_jpeg_pleno_part()) {
    case JpegPlenoPart::LightField: {
      return get_light_field_encoder(
          std::move(std::unique_ptr<JPLMEncoderConfigurationLightField>(
              static_cast<JPLMEncoderConfigurationLightField *>(
                  configuration.release()))));
    }
    default: {
      std::cerr << "Unknown part" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return nullptr;
}

std::unique_ptr<JPLMCodec> JPLMCodecFactory::get_lightfield_decoder(
    std::shared_ptr<JPLFile> jpl_file,
    const std::unique_ptr<JpegPlenoCodestreamBox> &codestream,
    const std::string &output_filename) {
  const auto &codestream_as_part2 =
      static_cast<JpegPlenoLightFieldBox &>(*codestream);
  // const auto& contiguous = codestream_as_part2.get_ref_to_contents().get_ref_to_contiguous_codestream_box();
  const auto mode = codestream_as_part2.get_ref_to_contents()
                        .get_ref_to_light_field_header_box()
                        .get_ref_to_contents()
                        .get_ref_to_light_field_header_box()
                        .get_ref_to_contents()
                        .get_compression_type();
  switch (mode) {
    case CompressionTypeLightField::transform_mode: {
      std::cout << "Transform Mode codestream" << std::endl;

      return std::make_unique<JPLM4DTransformModeLightFieldDecoder<uint16_t>>(
          jpl_file, codestream_as_part2, output_filename);
      break;
    }
    case CompressionTypeLightField::prediction_mode: {
      std::cerr << "prediction_mode not supported yet" << std::endl;
      break;
    }
  }
}

std::vector<std::unique_ptr<JPLMCodec>> JPLMCodecFactory::get_decoders(
    std::shared_ptr<JPLFile> jpl_file, const std::string &output_filename) {
  if (!jpl_file->has_codestream()) {
    std::cerr << "The input jpl file has no codestream" << std::endl;
  }

  auto decoders = std::vector<std::unique_ptr<JPLMCodec>>();
  auto &codestreams = jpl_file->get_reference_to_codestreams();
  for (const auto &codestream : codestreams) {
    switch (codestream->get_type()) {
      case JpegPlenoCodestreamBoxTypes::LightField: {
        std::cout << "There is a lightfield in this box" << std::endl;
        decoders.push_back(JPLMCodecFactory::get_lightfield_decoder(
            jpl_file, codestream, output_filename));
        break;
      }
      case JpegPlenoCodestreamBoxTypes::PointCloud: {
        std::cout << "point cloud" << std::endl;
        std::cerr << "Not defined yet..." << std::endl;
        break;
      }
      case JpegPlenoCodestreamBoxTypes::Hologram: {
        std::cout << "hologram" << std::endl;
        std::cerr << "Not defined yet..." << std::endl;
        break;
      }
    }
  }
  return decoders;
}
