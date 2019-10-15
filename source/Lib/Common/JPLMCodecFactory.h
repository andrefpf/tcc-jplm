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

/** \file     JPLMCodecFactory.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-11
 */

#ifndef JPLMCODECFACTORY_H__
#define JPLMCODECFACTORY_H__

#include <memory>
#include "JPLMEncoderConfigurationLightField.h"
#include "JPLMEncoderConfigurationLightField4DPredictionMode.h"
#include "JPLMEncoderConfigurationLightField4DTransformMode.h"
#include "Lib/Common/JPLMConfiguration.h"
#include "Lib/Part1/Common/JPLMCodec.h"
#include "Lib/Part2/Encoder/JPLM4DPredictionModeLightFieldEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/JPLM4DTransformModeLightFieldEncoder.h"
#include "Lib/Part2/Decoder/TransformMode/JPLM4DTransformModeLightFieldDecoder.h"


class JPLMCodecFactory {
 protected:
  static std::unique_ptr<JPLMCodec> get_light_field_encoder(
      std::unique_ptr<JPLMEncoderConfigurationLightField>&& configuration) {
    if (configuration->get_compression_type() ==
        CompressionTypeLightField::prediction_mode) {
      return std::make_unique<JPLM4DPredictionModeLightFieldEncoder<uint16_t>>(
          std::unique_ptr<JPLMEncoderConfigurationLightField4DPredictionMode>(
              static_cast<JPLMEncoderConfigurationLightField4DPredictionMode*>(
                  configuration.release())));
    }
    assert(configuration->get_compression_type() ==
           CompressionTypeLightField::transform_mode);

    return std::make_unique<JPLM4DTransformModeLightFieldEncoder<uint16_t>>(
        std::unique_ptr<JPLMEncoderConfigurationLightField4DTransformMode>(
            static_cast<JPLMEncoderConfigurationLightField4DTransformMode*>(
                configuration.release())));
  }


 public:
  static std::unique_ptr<JPLMCodec> get_encoder(
      std::unique_ptr<JPLMEncoderConfiguration>&& configuration) {
    switch (configuration->get_jpeg_pleno_part()) {
      case JpegPlenoPart::LightField: {
        return get_light_field_encoder(
            std::move(std::unique_ptr<JPLMEncoderConfigurationLightField>(
                static_cast<JPLMEncoderConfigurationLightField*>(
                    configuration.release()))));
      }
      default: {
        std::cerr << "Unknown part" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    return nullptr;
  }


  // static std::unique_ptr<JPLMCodec> get_decoder(
  //     [[maybe_unused]] std::unique_ptr<JPLMConfiguration>&& configuration) {
  //   return nullptr;
  // }

///###############################################################################
///############################ TEMPORARY ########################################
///###############################################################################
static uint16_t read_int_from_codestream_code(
    const ContiguousCodestreamCode& codestream_code) {
  auto bytes = std::vector<std::byte>();
  auto byte_0 = codestream_code.get_next_byte();
  auto byte_1 = codestream_code.get_next_byte();
  bytes.push_back(byte_0);
  bytes.push_back(byte_1);
  return BinaryTools::get_value_from_big_endian_byte_vector<uint16_t>(bytes);
}
///###############################################################################
///############################ TEMPORARY ########################################
///###############################################################################


  static std::vector<std::unique_ptr<JPLMCodec>> get_decoders(
      std::shared_ptr<JPLFile> jpl_file, const std::string& output_filename) {
    if (!jpl_file->has_codestream()) {
      std::cerr << "The input jpl file has no codestream" << std::endl;
    }

    auto decoders = std::vector<std::unique_ptr<JPLMCodec>>();
    auto& codestreams = jpl_file->get_reference_to_codestreams();
    for (const auto& codestream : codestreams) {
      switch (codestream->get_type()) {
        case JpegPlenoCodestreamBoxTypes::LightField: {
          std::cout << "There is a lightfield in this box" << std::endl;
          const auto& codestream_as_part2 =
              static_cast<JpegPlenoLightFieldBox&>(*codestream);
          // const auto& contiguous = codestream_as_part2.get_ref_to_contents().get_ref_to_contiguous_codestream_box();
          const auto mode = codestream_as_part2.get_ref_to_contents()
                                .get_ref_to_light_field_header_box()
                                .get_ref_to_contents()
                                .get_ref_to_light_field_header_box()
                                .get_ref_to_contents()
                                .get_compression_type();
          switch (mode) {
            case CompressionTypeLightField::transform_mode: {
              std::cout << "Transform Mode codestream"
                        << std::endl;

              //####################### temporary until implementing the markers ############################
              auto& codestream_code =
                  codestream_as_part2.get_ref_to_contents()
                      .get_ref_to_contiguous_codestream_box()
                      .get_ref_to_contents()
                      .get_ref_to_code();
              [[maybe_unused]] auto superior_bit_plane =
                  read_int_from_codestream_code(codestream_code);
              // //reads the maximum transform sizes
              auto transform_length_t =
                  read_int_from_codestream_code(codestream_code);
              auto transform_length_s =
                  read_int_from_codestream_code(codestream_code);
              auto transform_length_v =
                  read_int_from_codestream_code(codestream_code);
              auto transform_length_u =
                  read_int_from_codestream_code(codestream_code);
              auto block_4d_size = LightfieldDimension<uint32_t>(
                  transform_length_t, transform_length_s, transform_length_v,
                  transform_length_u);
              codestream_code.rewind(10);
              //####################### temporary until implementing the markers ############################


              decoders.push_back(
                  std::make_unique<JPLM4DTransformModeLightFieldDecoder<uint16_t>>(*jpl_file,
                      codestream_as_part2, output_filename,
                      block_4d_size));
              break;
            }
            case CompressionTypeLightField::prediction_mode: {
              std::cerr << "prediction_mode not supported yet" << std::endl;
              break;
            }
          }
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
};

#endif /* end of include guard: JPLMCODECFACTORY_H__ */