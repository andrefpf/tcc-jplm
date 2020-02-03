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

/** \file     JPLM4DPredictionModeLightFieldEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-09
 */

#ifndef JPLM_LIB_PART2_ENCODER_JPLM4DPREDICTIONMODELIGHTFIELDENCODER_H__
#define JPLM_LIB_PART2_ENCODER_JPLM4DPREDICTIONMODELIGHTFIELDENCODER_H__

#include "Lib/Common/CommonExceptions.h"
#include "Lib/Part2/Encoder/JPLMLightFieldEncoder.h"
#include "source/Lib/Common/JPLMEncoderConfigurationLightField4DPredictionMode.h"

template<typename PelType = uint16_t>
class JPLM4DPredictionModeLightFieldEncoder
    : public JPLMLightFieldEncoder<PelType> {
 protected:
  std::unique_ptr<JPLMEncoderConfigurationLightField4DPredictionMode>
      prediction_mode_configuration;

 public:
  JPLM4DPredictionModeLightFieldEncoder(
      std::unique_ptr<JPLMEncoderConfigurationLightField4DPredictionMode>&&
          configuration)
      : JPLMLightFieldCodec<PelType>(
            std::make_unique<LightfieldFromFile<PelType>>(
                configuration->get_lightfield_io_configurations())),
        JPLMLightFieldEncoder<PelType>(*configuration),
        prediction_mode_configuration(std::move(configuration)) {
  }

  virtual ~JPLM4DPredictionModeLightFieldEncoder() = default;

  virtual void run() override {
    //! \todo implement run method for jpl lightfield encoder
    throw CommonExceptions::NotImplementedException(
        "JPLM4DPredictionModeLightFieldEncoder::run()");
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_JPLM4DPREDICTIONMODELIGHTFIELDENCODER_H__ */
