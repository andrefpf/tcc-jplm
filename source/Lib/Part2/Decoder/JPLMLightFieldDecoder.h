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

/** \file     JPLMLightFieldDecoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-09
 */

#ifndef JPLM_LIB_PART2_DECODER_JPLMLIGHTFIELDDECODER_H__
#define JPLM_LIB_PART2_DECODER_JPLMLIGHTFIELDDECODER_H__

#include "Lib/Common/JPLMDecoderConfiguration.h"
#include "Lib/Part2/Common/JPLMLightFieldCodec.h"

/**
 * @brief      This class describes a jplm light field decoder.
 *
 * @tparam     PelType  type used to store pixels of the decoded Lightfield.
 */
template<typename PelType = uint16_t>
class JPLMLightFieldDecoder : public virtual JPLMLightFieldCodec<PelType> {
 protected:
  const JPLMDecoderConfiguration&
      decoder_configuration;  //<! the decoder configuration

 public:
  /**
   * @brief      Constructs a new instance.
   *
   * @param[in]  configuration  The configuration
   */
  JPLMLightFieldDecoder(const JPLMDecoderConfiguration& configuration)
      : JPLMLightFieldCodec<PelType>(configuration),
        decoder_configuration(configuration) {
  }


  /**
   * @brief      Destroys the object.
   */
  virtual ~JPLMLightFieldDecoder() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_DECODER_JPLMLIGHTFIELDDECODER_H__ */