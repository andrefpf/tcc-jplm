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

#include "Lib/Common/JPLMDecoderConfiguration.h"
#include "Lib/Common/JPLMEncoderConfiguration.h"
#include "Lib/Common/JPLMEncoderConfigurationLightField.h"
#include "Lib/Part1/Common/Boxes/JpegPlenoCodestreamBox.h"
#include "Lib/Part1/Common/JPLMCodec.h"


/**
 * @brief      Front-end to create jplm codec objects.
 */
class JPLMCodecFactory {
 protected:
  /**
   * @brief      Gets the light field encoder.
   *
   * @param      configuration  The configuration
   *
   * @return     The light field encoder.
   */
  static std::unique_ptr<JPLMCodec> get_light_field_encoder(
      std::unique_ptr<JPLMEncoderConfigurationLightField>&& configuration);

 public:
  /**
   * @brief      Gets the encoder.
   *
   * @param      configuration  The configuration
   *
   * @return     The encoder given the desired configuration.
   */
  static std::unique_ptr<JPLMCodec> get_encoder(
      std::unique_ptr<JPLMEncoderConfiguration>&& configuration);


  /**
   * @brief      Gets the hologram decoder.
   *
   * @param[in]  jpl_file         The jpl file
   * @param[in]  codestream       The codestream
   * @param[in]  output_filename  The output filename
   * @param[in]  configuration    The configuration
   *
   * @return     The hologram decoder.
   */
  static std::unique_ptr<JPLMCodec> get_hologram_decoder(
      std::shared_ptr<JPLFile> jpl_file,
      const std::unique_ptr<JpegPlenoCodestreamBox>& codestream,
      const std::string& output_filename,
      std::shared_ptr<JPLMDecoderConfiguration> configuration);


  /**
   * @brief      Gets the point cloud decoder.
   *
   * @param[in]  jpl_file         The jpl file
   * @param[in]  codestream       The codestream
   * @param[in]  output_filename  The output filename
   * @param[in]  configuration    The configuration
   *
   * @return     The point cloud decoder.
   */
  static std::unique_ptr<JPLMCodec> get_point_cloud_decoder(
      std::shared_ptr<JPLFile> jpl_file,
      const std::unique_ptr<JpegPlenoCodestreamBox>& codestream,
      const std::string& output_filename,
      std::shared_ptr<JPLMDecoderConfiguration> configuration);


  /**
   * @brief      Gets the lightfield decoder.
   *
   * @param[in]  jpl_file         The jpl file
   * @param[in]  codestream       The codestream
   * @param[in]  output_filename  The output filename
   * @param[in]  configuration    The configuration
   *
   * @return     The lightfield decoder.
   */
  static std::unique_ptr<JPLMCodec> get_lightfield_decoder(
      std::shared_ptr<JPLFile> jpl_file,
      const std::unique_ptr<JpegPlenoCodestreamBox>& codestream,
      const std::string& output_filename,
      std::shared_ptr<JPLMDecoderConfiguration> configuration);


  /**
   * @brief      Gets the all the decoders available to decode codestreams in the jpl file.
   *
   * @param[in]  jpl_file         The jpl file
   * @param[in]  output_filename  The output filename
   * @param[in]  configuration    The configuration
   *
   * @return     The decoders.
   */
  static std::vector<std::unique_ptr<JPLMCodec>> get_decoders(
      std::shared_ptr<JPLFile> jpl_file, const std::string& output_filename,
      std::shared_ptr<JPLMDecoderConfiguration> configuration);
};

#endif /* end of include guard: JPLMCODECFACTORY_H__ */
