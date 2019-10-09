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

/** \file     JPLM4DTransformModeLightFieldCodec.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-10-09
 */

#ifndef JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__
#define JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__

#include <cstdint>
#include <iostream>
#include "Lib/Part2/Common/JPLMLightFieldCodec.h"
#include "Lib/Part2/Common/TransformMode/DCT4DCoefficientsManager.h"
#include "Lib/Part2/Common/TransformMode/LightFieldTransformMode.h"
#include "Lib/Utils/Image/ColorSpaces.h"

template<typename T = uint16_t>
class JPLM4DTransformModeLightFieldCodec
    : public virtual JPLMLightFieldCodec<T> {
 protected:
  int extension_length_t = 0;
  int extension_length_s = 0;
  int extension_length_v = 0;
  int extension_length_u = 0;
  bool needs_block_extension = false;

 public:
  JPLM4DTransformModeLightFieldCodec() : JPLMLightFieldCodec<T>(nullptr) {
    //this ctr will probably never be called, but it is required to compile...
  }


  virtual ~JPLM4DTransformModeLightFieldCodec() = default;


  void setup_transform_coefficients(bool forward,
      const std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>& max_sizes,
      const std::tuple<double, double, double, double>& scalings) {
    const auto& [t_size, s_size, v_size, u_size] = max_sizes;
    const auto& [t_scaling, s_scaling, v_scaling, u_scaling] = scalings;
    DCT4DCoefficientsManager::get_instance(forward).set_transform_max_sizes(
        u_size, v_size, s_size, t_size);

    DCT4DCoefficientsManager::get_instance(forward).set_transform_gains(
        u_scaling, v_scaling, s_scaling, t_scaling);
  }


  void initialize_extension_lenghts() {
    // extension_length_t = parameter_handler.number_of_vertical_views%parameter_handler.transform_length_t;
    // extension_length_s = parameter_handler.number_of_horizontal_views%parameter_handler.transform_length_s;
    // extension_length_v = decoded_lightfield.mNumberOfViewLines%parameter_handler.transform_length_v;
    // extension_length_u = decoded_lightfield.mNumberOfViewColumns%parameter_handler.transform_length_u;

    // if(parameter_handler.extension_method != SHRINK_TO_FIT && extension_length_t+extension_length_s+extension_length_v+extension_length_u > 0)
    //     needs_block_extension=true;
  }

  virtual void run() override {
    std::cout << "Run LF transfom mode codec." << std::endl;
    this->run_for_block_4d();
    //! \todo implement run method for jpl lightfield encoder
  }

  virtual void run_for_block_4d() = 0;
};


#endif /* end of include guard: JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__ */