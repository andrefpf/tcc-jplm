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
#include "Lib/Part1/Common/JPLMCodec.h"
#include "Lib/Part1/Common/JPLMConfiguration.h"
#include "Lib/Part2/Encoder/JPLM4DPredictionModeLightFieldEncoder.h"
#include "Lib/Part2/Encoder/JPLM4DTransformModeLightFieldEncoder.h"
#include "Lib/Part2/Encoder/JPLMEncoderConfigurationLightField.h"
#include "Lib/Part2/Encoder/JPLMEncoderConfigurationLightField4DPredictionMode.h"
#include "Lib/Part2/Encoder/JPLMEncoderConfigurationLightField4DTransformMode.h"


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
  static std::unique_ptr<JPLMCodec> get_decoder(
     [[maybe_unused]] std::unique_ptr<JPLMConfiguration>&& configuration) {
    return nullptr;
  }
};

#endif /* end of include guard: JPLMCODECFACTORY_H__ */